#include <mh_log.h>
#include <mh_gvns.h>
#include "AStar.h"

using namespace mh; 
using namespace std;
namespace clcs {


bool  AStar:: complete(const std::shared_ptr<clcs::Node>& v)
{

     for(int i = 0; i < v->QL.size(); ++i) 
         if(v->QL[ i ] < (int) inst->P[ i ].size()) 
            return false;

     return  true; //cpl1; 
}

void AStar::getSolution()
{  
      sol->deriveSolution(s_best.get());
}
// if ub_v == l_v + max (p_i) ask if P_{i_0} is included into each s_i

bool AStar::satisfied(const shared_ptr<Node>& v, int i_0)
{
      
    for(int i = 0; i < inst->m; ++i)
    {
	bool included = false; int pointer = v->QL[ i_0 ];
        for(int j = v->pL[ i ] - 1; j < inst->S[ i ].size() && !included && pointer < v->QL.size(); ++j) 
	{ 
            if( inst->P[ i_0 ][ pointer ] == inst->S[ i ][ j ] )
            {
                pointer++;
	    }		  
	    if(pointer == inst->S[i].size() - v->pL[ i ] + 1)
               included = true;
	}
	if( pointer < inst->S[i].size() - v->pL[ i ] + 1)
            return false;
    }	    
    return true;
}

void AStar::startSearch()
{
	// create root node and add it to N and Q
	shared_ptr<Node> root = std::make_shared<Node>();
	root->pL = std::vector<int16_t>(inst->m, 1);
        root->QL = std::vector<int16_t>(inst->k, 0);
	root->parent = nullptr;
	addNode(root);

	try {
		// run the main loop
		int iter = 0;  double t_time = 0;
		while (!Q.empty() and t_time < ttime()) {
			const shared_ptr<Node> v = Q.top().node;
			int ub_v = Q.top().ub;
			Q.pop(); //v->print();  
			
			//if(iter % 2 == 0)
			int max_p = 0; int i0 = 0;
			for(int i = 0; i < inst->k; ++i) 
			    if( max_p < inst->P[i].size() - v->QL[ i ] ) 
			    {	max_p = inst->P[i].size() - v->QL[ i ]; i0 = i; }

                        if(ub_v < v->l_v + max_p && max_p > 0)
	                   continue; 
                         /*if(ub_v == v->l_v + max_p && max_p > 0) 
			 {  // cout << "equal " << endl; 
	                    if(!satisfied(v, i0 ))
			    { // cout << "go further..." << endl; 
			       continue;
			    }
			   // else
			   //    cout << "optimality proven" << endl;    
                         }*/			       
			//cout << "UB: " << ub_v << endl;
			// check if node is outdated (required because priority_queue in c++ doesn't support removal of arbitrary elements)
			auto N_known = N.find(std::make_pair(v->pL, v->QL));
			if (N_known != N.end()) {
				bool outdated = false;
				auto l_v_known = N_known->second; // l_v

				if ((l_v_known > v->l_v)) {
				     outdated = true;
		                }
		
				if (outdated) 
				    continue;
			}
//			mh::out() << "Expanding "; printNode(v, false);
			// find non-dominated sigma
			//map<int, vector<int16_t>> sigma_nd 
                        map<int, pair<vector<int16_t>, vector<int16_t>>> sigma_nd = sol->findSigmaNdBS( (*v));
			// check if we arrived at a complete node (i.e., an optimal solution was found), false
			if (sigma_nd.empty() == true and complete(v) and ub_v == v->l_v) {
			   cout <<"UB of opt: " << ub(*v, inst) << endl;
			   cout << "opt found " << endl; cout << "opt node: " << endl; v->print(); s_best = v; getSolution(); 
                           for(int16_t val : sol->s)
                           {
                               cout << val << " "; 
                           }
                           sol->write(mh::out(), 1);
                           return;
			}
			// create an expansion for every letter from sigma_nd
			++stat_nodes_expanded;
			for (auto &it : sigma_nd) {
				int letter = it.first;
				std::pair< vector<int16_t>, vector<int16_t> > pL_QL = it.second;

				shared_ptr<Node> v_ext = sol->expandNode(v, letter, pL_QL.first, pL_QL.second);
				auto N_rel = N.find( std::make_pair(pL_QL.first, pL_QL.second) );
                                addNode(v_ext);
			}
			iter++;
			t_time = (double)mh::mhcputime();
		}
		if(!Q.empty())
		    cout <<"UB: " << Q.top().ub << endl;
        // when memory is exceeded, catch the proven UB value on the problem
	} catch (std::bad_alloc &ex) {
		mh::out() << "Out of memory!" << std::endl;
		mh::out() << "UB: " << Q.top().ub << std::endl;
	}
	//return std::vector<int16_t>(); // there is no solution
}

void AStar::printStatistics()
{
	mh::out() << "Node Stats (created, expanded, ignored, merged): ";
	mh::out() << stat_nodes_created << ", " << stat_nodes_expanded << ", ";
	mh::out() << stat_nodes_ignored << ", " << stat_nodes_merged << std::endl;
	mh::out() << "Sizes (N, Q): " << N.size() << ", " << Q.size() << std::endl;
	mh::out() << "Max Nested (N_rel): " << stat_max_nested << std::endl;
}

/**  @v added into corresp. N_rel; 
     if N_rel is empty -> create a linked list and add node @v
     or update corresp. entry in N_rel  at given position @pos
 */
void AStar::addNode(const shared_ptr<Node> &v)
{
        // mh::out() << " creating "; printNode(v);
	Q.push(QNode(v, ub(*v, inst)));
	++stat_nodes_created;
	if (N.find( std::make_pair( v->pL, v->QL ) ) == N.end() ) {
	    N.insert({ std::make_pair( v->pL, v->QL ), v->l_v  });
	} else { // update node possibly or cut-off
	   auto  it = N.find(std::make_pair( v->pL, v->QL ));
           it->second = v->l_v;
	}
}
/** print partial solution that corresp. to node @v into an .out file */
void AStar::printNode(const shared_ptr<Node> &v, bool withParent)
{
	if (v->l_v > 0 && withParent) {
		mh::out() << "from ((";
		for (int i = 0; i < inst->m; i++) {
		     mh::out() << v->parent->pL[ i ];
		     if (i != inst->m - 1 ) {
			 mh::out() << ",";
		     }
		}
		mh::out() << "), " << v->parent->l_v << ", ";
                //for(int i: v->QL
		//mh::out() << v->parent->u_v;
		mh::out() << ") with '";
		mh::out() << inst->int2Char.at(inst->S[ 0 ][ v->pL[0] - 2 ]) << "' -> ";
	}

	mh::out() << "Node ((";
	for (int i = 0; i < inst->m; i++) {
		mh::out() << v->pL[i];
		if (i != inst->m - 1 ) {
			mh::out() << ",";
		}
	}
	mh::out() << "), ";
	mh::out() << v->l_v << ", ";
	mh::out() << ")"; // v->u_v 
	if (!withParent) {
		mh::out() << std::endl;
	}
}

}
