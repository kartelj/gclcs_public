#include <memory>
#include <random>
#include "BeamSearch.h"
#include <cstdlib>
#include <fstream>
#include <mh_util.h>
#include <mh_log.h>
#include <limits>
#include <mh_gvns.h>

using namespace std;
using namespace mh;

namespace clcs {

/** mhlib external parameters */
mh::int_param bw("bw", "beam width parameter", 10, 0, 1000000);
mh::int_param kbest("kbest", "(restricted) filtering parameter", 0, 0, 100000);
mh::int_param kext("kext", "pre-restriction parameter", 100000, 0, 100000);
mh::int_param guidance("guidance", "0: UB; 1: H_pro; 2: penalty ", 0, 0, 4);
mh::int_param filtering("filtering", "turn on Filter procedure", 1, 0, 1);
mh::int_param turn_cutoff("turn_cutoff", "turn cutoff", 0, 0, 1);
// mh::int_param feasiblity("feasibility",  "feasbility cut-off", 0, 0, 1);
long double BeamSearch::evaluate_cutoff( const Node &v )
{
      long double prob = 1.0;

      for(int i = 0; i < v.pL.size(); ++i) 
      {
           for(int j = 0; j < v.QL.size(); ++j) 
           {
               prob *= inst->P_m[ inst->P[ j ].size() - v.QL[ j ] ]
                                [ inst->S[ i ].size() - v.pL[ i ]  + 1 ];
           }
       }
       return prob;
}

void BeamSearch::checkAndAdd(vector<shared_ptr<Node>>& beam, std::shared_ptr<clcs::Node> & v )
{
       std::pair <vector<int16_t>, vector<int16_t>> foo; 
       foo = std::make_pair (v->pL, v->QL);
       if(N.find( foo ) == N.end())
       {
            beam.push_back( v );
            N[foo] =   true;   
       }

       return; 
}

void BeamSearch::startSearch()
{
	// create root node
	shared_ptr<Node> root = std::make_shared<Node>(std::vector<int16_t>(inst->m, 1), std::vector<int16_t>(inst->k, 0));
	root->parent = nullptr;
        root->additional_letters = std::vector<int16_t>(inst->m, 1); 
	s_best = root;
	// initialize beam and add root node to it
	auto beam = vector<shared_ptr<Node>>();
	beam.push_back(root);
        int iter = 0;     double time = (double)mhcputime(); cout << time << endl; 
	// run beam search until beam is empty
	while (!beam.empty() and time < ttime() )
	{       cout << "#interation " << iter << endl;
		s_best = beam[0];  //->print(); // cout << "h: " << beam[0]->h << endl;
	        auto extensions = vector<NodeExtension>(); //extensions.reserve(500000);
	       	extensions = extendAndEvaluate(beam);
  	        cout << "|Vext| " << extensions.size() << endl;
		
                if (pruning() == 1)
		    prune(extensions);
		if (filtering() == 1)
		    filter(extensions);
		beam.clear();  
		for(int i = 0; i < (int) extensions.size() && (int) beam.size() < k_bw; i++) {
                    checkAndAdd( beam, extensions[i].node) ;
		    //beam.push_back(extensions[i].node);
		} //cout << "added " << endl;
                iter++;N.clear(); 
		time = (double)mhcputime(); 
	} 
        s_best->print(); 

	  getSolution(); 
	  cout << "|s| = " << sol->s.size() << endl;
          sol->checkFeasibility();
          sol->write(out(), 1);
}
/** Extend all nodes from the set of given nodes @beam (priority values of each node are pre-calculated) **/
// this still not working... temp structure is not properly updated...

bool BeamSearch:: complete(const std::shared_ptr<clcs::Node>& v)
{

     /*bool cpl1 = false;
     for(int i = 0; i < v->pL.size() && !cpl1 ; ++i) 
        if(v->pL[ i ] > inst->S[ i ].size())
           cpl1 = true; */

     for(int i = 0; i < v->QL.size(); ++i) 
         if(v->QL[ i ] < (int) inst->P[ i ].size()) 
            return false;

     return  true; //cpl1; 
}

vector<NodeExtension> BeamSearch::extendAndEvaluate(const vector<shared_ptr<Node>>& beam)
{
	auto extensions = vector<NodeExtension>();
        //map<int, vector<int16_t>> temp;
	for (const shared_ptr<Node> &v : beam) { // v->print();
	           map<int, pair<vector<int16_t>, vector<int16_t>>> sigma_nd = sol->findSigmaNdBS(*v);
	           for (auto &it : sigma_nd) {
			int letter = it.first;
			pair<vector<int16_t>, vector<int16_t>> pL_QL = it.second;
			// create new child node by appending letter a and calculate greedy value
			NodeExtension extension = NodeExtension();
                        //cout << "ext " << letter << endl; 
			shared_ptr<Node> newChild = sol->expandNode(v, letter, pL_QL.first, pL_QL.second);
			extension.node = newChild;
			extension.h = clcs_eval::greedy(*extension.node, inst);
			extensions.push_back(extension);

  			// check if we found a new valid best solution
			if ( complete( newChild ) and newChild->l_v > s_best->l_v) {
			     s_best = newChild;
			}
	           }
	}
	// take best solutions according to the best-next heuristic
	std::sort(extensions.begin(), extensions.end());

	if ((int) extensions.size() > max_extensions) {
		extensions.resize(max_extensions);
	}

	// apply main heuristic and sort extensions
	int px = 0;
 	if (guidance() == (int) Heuristic::H_PROB) { // calculate p-value if the H_prob heuristic used
	     px = determine_p(extensions);  
	}  // cout << "PX ---------------------------> " << px << endl;
	for (NodeExtension &extension : extensions) {
	     if (guidance() > 0) {
		 extension.h = h(*extension.node, inst, px); 
		 if (h2 != nullptr) {
		     extension.h2 = h2(*extension.node, inst, px);  
                     if(turn_cutoff() == 1) 
                       extension.cut_off = evaluate_cutoff(*extension.node);
		 }
             } else{  // use UB as a guidance:
		    extension.h = ub(*extension.node, inst); 
		    if( h2 != nullptr) 
			extension.h2 = h2(*extension.node, inst, px);  
                    if(turn_cutoff() == 1) {
                       extension.cut_off = evaluate_cutoff(*extension.node);
                    }
               }
	}
	std::sort(extensions.begin(), extensions.end(), std::greater<NodeExtension>());
	return extensions;
}

/** Prune function in GBS framework */
void BeamSearch::prune(vector<NodeExtension> &extensions)
{        
	extensions.erase(std::remove_if(extensions.begin(), extensions.end(),
		   [this](const NodeExtension &e) { return ub(*e.node, inst) <= s_best->l_v; }), extensions.end());
}

bool BeamSearch::dominated_member(int index, vector<NodeExtension>& C, int k_fi) // return true if node is dominated by some nodes from k_best...
{
	int kf = (k_fi > 0) ? k_fi : kbest();
	Node *node = C[index].node.get();

	for (int i = 0; i < std::min(kf, (int) C.size()); i++)
	{
		if (i != index and isDominated(node, C)) {   // C[i] - from k_best dominates <node>
			return true;
		}
	}
	return false;
}

/** determine p-value in H_prob heuristic **/
int BeamSearch::determine_p( vector<NodeExtension> &extensions )
{
	vector<int> p_min = vector<int>(inst->P.size(), 100000);  
        int mx_sum = 10000;

	for (auto &ext : extensions) {
	       Node &v = *ext.node.get();
              
               for(int i = 0; i < inst->P.size(); ++i) 
               {
		   int h_v = inst->P[ i ].size() - v.QL[ i ];
		   if (h_v < p_min[i])
	               p_min[ i ] = h_v;
               }
	}
        int l_m = 0; 

        for(int val: p_min){
           //cout << "val: " << val << endl;
           l_m += val;
        }
        cout << "l_m: " << l_m << endl; 
	int fl_term = 100000;
	for (NodeExtension &ext : extensions) {
		Node &v = *ext.node.get();
		// min len:
		int l_mx = 100000;
		for (int i = 0; i < inst->m; ++i) {
			if (l_mx > (int) inst->S[i].size() - v.pL[i] + 1 - l_m)  // v.pL ==> v.additional_letters[i]
				l_mx = inst->S[i].size() - v.pL[i] + 1 - l_m;
		}
		int fl_term_v = std::floor((l_mx) / (inst->sigma + 0.0));
		if (fl_term_v < fl_term)
		    fl_term = fl_term_v;
	}

	if (fl_term == 0) /* p_min + fl_term == 0 */
		return 1;
	else
		return l_m + fl_term; /*p_min + fl_term */
}


/** pairwise Filter procedure in GBS framework; set up acc. to parameter kbest()  */
void BeamSearch::filter(vector<NodeExtension>& extensions)   // if k_filter() > C.size(): full filtering (for all elements) applied
{
	for (int i = 0; i < (int) extensions.size(); i++)
	{       //double time = (double)mhcputime();
		
		if (dominated_member(i, extensions, kbest())) // it is dominated w.r.t. some of best k_best nodes
		{
		    extensions.erase(extensions.begin() + i); // free the address
		    i--;
		}
		double time = (double) mhcputime();
		if(time > ttime() )
	           return;
	}
}

void BeamSearch::getSolution()
{  
      sol->deriveSolution(s_best.get());
}

bool BeamSearch::isDominated(Node* v,  vector<NodeExtension>& extensions)  
{
	for (int i = 0; i < (int)extensions.size() && i < k_best; i++)
	{
		Node &v2 = *extensions[i].node.get(); //address of the content
		if (v != &v2) {
		    if (sol->check_domination(v->pL, v->QL, v2.pL, v2.QL)) {
			return true;
		    }
		}
	}
	return false;
}

bool BeamSearch::branchOrNot( const Node& v)
{

     vector<int> min_occurances_node((int) sol->inst->sigma, (int) sol->inst->S[0].size());     
     for(int i = 0; i < sol->inst->sigma; ++i) 
     {
	for (int a = 0; a < sol->inst->sigma; a++) {
	     for (int i = 0; i < sol->inst->m; i++) {
		     if (v.pL[i] - 1 < (int16_t) inst->S[i].size()) //not feasible letter
		     {
			 if (sol->inst->occurance_positions[i][a][v.pL[i] - 1] < min_occurances_node[a])
			     min_occurances_node[a] = sol->inst->occurance_positions[i][a][v.pL[i] - 1];
		     } 
                     else min_occurances_node[a] = 0; // some of indices of pL exceed max indices...
	     }
	} 
    }
    // count number of letters in Pis (remainging part which has to be included) 
    vector<int> min_occurances_node_p((int) sol->inst->sigma, 0);     

    for(int a = 0; a < (int) sol->inst->sigma; ++a) 
    {
        
        for(int i = 0; i < v.QL.size(); ++i) 
        {   
            int count_a_i = 0; 
            if(v.QL[ i ] < sol->inst->P[ i ].size() )
            {
               for(int j = v.QL[ i ]; j < sol->inst->P[ i ].size(); ++j) 
               {
                   if( sol->inst->P[ i ][ j ] == a ) 
                       count_a_i++;
               }
               if(count_a_i > min_occurances_node_p[ a ])
                  min_occurances_node_p[ a ] = count_a_i;
            }
       }
    }
    // branch or not: 
    for(int a = 0; a < sol->inst->sigma; ++a) 
    { 
        if( min_occurances_node[ a ] > 0 or min_occurances_node_p[ a ] > 0 )
        {
            // cout << min_occurances_node_p[ a ] << " " <<  min_occurances_node[ a ] << endl;
            if(min_occurances_node_p[ a ] > min_occurances_node[ a ] ) {
               return true; 
            }
        }
    }
    return false;
}

void BeamSearch::update_embed_struct( const Node& child )  //map<int, vector<int16_t>> &  temp)
{ 
     //if(temp.size() == 0)
     //   return; 
     //for (std::pair<int, vector<int16_t>> ix : temp) { 
     //     int index = 0; cout << "pattern " <<  ix.first << " ==========> " << endl;
     //     vector<int16_t> ss = ix.second;
          
     for(int i = 0; i < inst->m; ++i) 
     {

         for(int px = 0; px < inst->P.size(); ++px) 
         {
           int ix = inst->P[ px ].size() - 1; bool us = false; int num_match = 0;  int16_t index = inst->n; //cout << "index " << index << endl;
           for(int j = inst->S[ i ].size() - 1; j >= 0 && !us && ix >= 0; --j) 
           {   
               if(inst->P[ px ][ ix ] == inst->S[i][ j ] ){
                  index = j;
                  ix--; num_match++;
               }  
               if( num_match == inst->P[ px ].size() - child.QL[ px ])
                   us = true; 
           } 
           if(us)
              Embed[ i ][ px ] = index + 1;
           else 
              Embed[ i ][ px ] = inst->n + 1;
        }
     }

        cout <<endl;
    // }
}

void BeamSearch::print_embed()
{
     for(int i = 0; i < Embed.size(); ++i) 
     {
        for(int val: Embed[ i ])
        {
           cout << val << " "; 
        }
        cout << endl;
     }
}
   


void BeamSearch::startGreedy()
{
	// initialize seed and random number generation (used only for randomized greedy)
	bool is_randomized = false;
	// initialize root node
	shared_ptr<Node> v = std::make_shared<Node>(std::vector<int16_t>(inst->m, 1), std::vector<int16_t>(inst->k, 0));
	//v->pL = std::vector<int16_t>(inst->m, 1);
	//v->QL = std::vector<int16_t>(inst->k, 0);
	v->parent = nullptr;
	s_best = v; cout << "Embed printing..." << endl; print_embed(); 
   
	// start actual greedy search
        map< int, vector<int16_t>> temp_best;
	map<int, pair<vector<int16_t>, vector<int16_t>>> sigma = sol->findSigmaNd(*v, Embed);
        cout << "sigma.size() " << sigma.size() << endl;
        bool stop = false; int iter = 0;  int branched = 0; 
	while(!sigma.empty() && !stop) {
                cout << "start..." << endl;
		bool skipHeuristic = false;int charNum = -1;
	        if (!skipHeuristic) { // skip this step in case we already took the letter forming a strong match
		    double bestVal = std::numeric_limits<double>::max();
		    shared_ptr<Node> bestChild = nullptr; 
		    for (auto &it : sigma) 
                    {           
				int letter = it.first;
				pair<vector<int16_t>, vector<int16_t>> pL_QL = it.second;
				shared_ptr<Node> child = sol->expandNode(v, letter, pL_QL.first, pL_QL.second);
				double g = clcs_eval::greedy(*child, inst);   
				if (g <= bestVal ) {  
				    bestVal = g;
				    bestChild = child;
                                    charNum = letter; // temp_best.clear(); 
                                    temp_best = sol->temp[ charNum ];  
				}
		    }
		    v = bestChild; update_embed_struct(*v);    //temp_best); 
                   temp_best.clear(); // update only necessary columns of Embed structure
		}    
                //cout << "expanded by letter: " << charNum << endl;
                if(sigma.size() > 0) 
                {   
                   sol->temp.clear(); sigma.clear();
		   sigma = sol->findSigmaNd(*v,  this->Embed); //cout << "sigmaND " << sigma.size() << endl;
                }else
                   stop = true;
                
                cout << " end iteration " << endl;
                 
	}     
	s_best = v; s_best->print(); //mh::out() << "derive greedy sol:" << endl;
        getSolution(); // cout << "|s| = " << sol->s.size() << endl; cout << "Branched: " << branched << endl;
        for(int16_t val : sol->s)
        {
             cout << val << " "; 
        }
	sol->checkFeasibility();
        sol->write(out(), 1);
       // sol->save(outStream::getFileName("sol1.txt", "sol"));
}
}


