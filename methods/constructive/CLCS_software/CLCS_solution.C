#include <algorithm>
#include <assert.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <mh_util.h>
#include <mh_log.h>
#include <mh_param.h>
#include "BeamSearch.h"
#include "AStar.h"
#include "CLCS_solution.h"
#include <set>
#include <unordered_set>
#include <new>

using namespace std;
using namespace mh;

namespace clcs {

/** choose algorithm to execute (external mhlib parameter **/
int_param algorithm("algorithm", "1: GH (including HHBS); 3: BS;", 0, 0, 3);
/** execution time allowed for algorithms (default time limit is set to 900 sec) **/
double_param time("time", "ttime: time-limit  ", 900, 0, 10000);
int_param feasibility("feasibility", "feasbility cut-off", 0, 0, 1); 

template<typename T>                                                       
std::vector<std::vector<std::vector<T>>> make_3d_vector(int z, int y, int x, T value = T{})
{
    return std::vector<std::vector<std::vector<T>>>(z, std::vector<std::vector<T>>(y, std::vector<T>(x, value)));
}
 
/** Run BS (check BeamSearch.h) */
void CLCS_solution::BS()
{
	BeamSearch bs = BeamSearch(this);
       
        cout << "BS start with performing ..." << endl;
	bs.startSearch(); 
	//s = bs.getSolution();
}

/** Run BS (check BeamSearch.h) */
void CLCS_solution::A_Star()
{
        AStar a_star = AStar(this);
	a_star.startSearch();
	a_star.printStatistics();
}

/** Run Greedy (for m-CLCS problem) */
void CLCS_solution::Greedy()
{         
	BeamSearch* bs = new BeamSearch(this);
	bs->startGreedy();   // cout << "solution---->" << inst->S[0][0] << endl;
	//s = bs->getSolution();
}
 
/** termination criterion: time limit */
bool CLCS_solution::terminate()
{
	if (time() >= 0 && time() <= (mhwctime() - timeStart))
		return true;
	return false;
}
/** Copy constructor of mh_solution object */
void CLCS_solution::copy(const mh_solution &sol)
{
	const CLCS_solution &ss = cast(sol);
	assert(inst == ss.inst);
	UB = ss.UB;
	mh::mh_solution::copy(ss);
	s = ss.s;
	invalidate();
}

string CLCS_solution::getSolution()
{
	string Sol;
	for(int a : s)
	     Sol += inst->int2Char[a];
	if( Sol == "")
	    Sol = "-";
       cout << Sol << "\n" << endl;	
	return Sol;
}
/** convert solution --> from character to corresp. <int> type */
void CLCS_solution::decodeSolution(const string &LCS)
{
	UB = inst->UB;
	s.clear();
	if (LCS == "-" || LCS == "")
		return; // empty solution

	for (int j = 0; j < int(LCS.size()); j++)
	     s.push_back(inst->char2Int.at(LCS[j]));
	invalidate();
}

CLCS_solution &CLCS_solution::operator=(const CLCS_solution &solution)
{
	s = solution.s;
	UB = solution.UB;
	inst = solution.inst;

	return *this;
}

/** printing stats into an .out file */
void CLCS_solution::write(std::ostream &os, int detailed)
{
//	os << "|s|=" << s.size();
//	os << " s=";
	if (s.size() == 0)
		os << "-";
	bool first = true;
	/*for (int a : s) {
		if (first)
		    first = false;
		else
		    os << " ";
		os << a;
	}
*/
	//if (detailed == 1)
	//    os << "CLCS=" << getSolution();

	os << "\nvalue: " << objective() << endl;
	double timeEnd = mh::mhwctime() ? mh::mhwctime() : mh::mhcputime();
	out() << "ttime: " << (timeEnd - timeStart) << endl; // print runtime

}

void CLCS_solution::save(const std::string &fname)
{
	if (fname != "NULL") {
		ofstream ostr(fname);
		if (!ostr)
		    mherror("Cannot open solution file for writing", fname);
		ostr << getSolution() << endl;
	}
}

void CLCS_solution::load(const std::string &fname)
{
	ifstream istr(fname);
	if (!istr)
	    mherror("Cannot open solution file for reading", fname);
	string P;
	istr >> P;
	decodeSolution(P);
}

/** Returns true if str1 is a subsequence of str2 */
static bool is_SubSequence(const vector<int> &str1, const vector<int> &str2)
{
	int j = 0;
	for (unsigned int i = 0; i < str2.size() && j < (int)str1.size(); i++)
             if (str1[j] == str2[i])
		 j++;
	
	// Returns true if all characters of str1 were found in str2
	if( j < str1.size()) 
           cout << "matched just " << j << " chars " << endl;
	return (j == (int)str1.size());
}
/** solution feasibility check: if unfeasible; an error occur */
void CLCS_solution::checkFeasibility()
{
	if (s.empty())
	    return;
	assert(UB <= inst->UB);

	// convert solution from int16_t to int
	vector<int> sol_int = vector<int>();
	for (short & it : s) {
	     sol_int.push_back(it);
	}
	// check feasibility
	bool feasible = true;
	for (int i = 0; i < inst->m && feasible; i++) {
		std::vector<int> s_i = inst->S[i];
		if (!is_SubSequence(sol_int, s_i)) // solution
		{
		    feasible = false;
		    cout << i << "-th string infeasible " << endl;
		}
	}
        // check for P_i-s
        for(int j = 0; j < inst->k; ++j) 
        {
	    if (!is_SubSequence(inst->P[ j ], sol_int)) {
		feasible = false;
	   	cout << "Constraint string P_" << j << " is not part of the solution" << endl;
	    }
        }
       //	assert(feasible != false);
       if(feasible == false) 
       {  
	       cout << " Not feasible "; s.clear(); assert(true==false);
	       //s.clear();
       }else{
          
	      cout << "Solution is feasible " << endl;
       
       }
       
	       //s.clear();
}



bool CLCS_solution::feasNode(vector<int16_t> pL, vector<int16_t> QL)
{  


      for(int i = 0; i < pL.size(); ++i) 
         if( pL[ i ] > inst->S[ i ].size())
            return false; 

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
               if( num_match == inst->P[ px ].size() - QL[ px ] )
                   us = true; 
           } 
            if( index + 1 < pL[ i ] ) 
            {
                return false;
            }
        }
      }
      return true; 
}


void  CLCS_solution::update_embed_column_pix( int lett, int pix, int p_u)
{
      vector<int16_t> pos;
      for(int i = 0; i < inst->m; ++i) 
      {   
          int ix = inst->P[ pix ].size() - 1; bool us = false; int num_match = 0;  int16_t index = inst->n; //cout << "index " << index << endl;
          for(int j = inst->S[ i ].size() - 1; j >= 0 && !us && ix >= 0; --j) 
          {   // << pix << " " << ix << endl;
               if(inst->P[ pix ][ ix ] == inst->S[i][ j ] ){
                  index = j;
                  ix--; num_match++;
               }  
               if( num_match ==  inst->P[ pix ].size() - p_u   )
                   us = true; 
          }      
          if(us){
             pos.push_back( index  + 1 );  
          }
          else
             pos.push_back( inst->n + 1 ); // -1 : unfeasible
        
      }
      map<int, vector<int16_t>> tpix;
      if(temp.find(lett) == temp.end()) 
         temp.insert(std::pair<int, map<int, vector<int16_t>> > (lett, tpix));

         temp[lett].insert( std::make_pair(pix,  pos));
}

/** return true if remaining Pi-s cannot be embedded into remaining subproblem; otherwise return false **/
/* params: next_position to consider its feasibility; i: s_i (i-th input string) **/

bool CLCS_solution::feas_embed_check(int position_next, int i, vector<int16_t>& lambda )
{
     for(int p = 0; i < inst->P.size(); ++p)
     {
         if(position_next > inst->Embed[ p ][ i ][ lambda[ p ] ] )
            return true;
     }
     return false;
} 


/** Retrival feasible characters for extending node @v : TODO -- update */
map<int, pair< vector<int16_t>,  vector<int16_t>>> CLCS_solution::findFeasibleSigmaBS(const Node &v)
{

        map<int, pair< vector<int16_t>,  vector<int16_t>>> feasibleSigma;
        for(int i = 0;  i < v.pL.size(); ++i)  
        {
            if(v.pL[ i  ] > inst->S[ i ].size()) 
            { 
               return   feasibleSigma;
            }
        }
	for (int a = 0; a < inst->sigma; a++) // check if letter is feasible...
        {
             if(feas(a, v)) //not feasible letter
	     {	
		bool is_feasible_letter = true; bool add_feas_check = false;
		vector<int16_t> pL_a = std::vector<int16_t>();	// new position vector for extending node by letter a
		vector<int16_t> QL_a = std::vector<int16_t>();	// new position vector for patters
                //map<int, vector<int16_t>> temp;  // temporary changes in Embed structure...
                for(int ix = 0; ix < inst->k; ++ix)
                {
                    
                    int p_u;
                    if(v.QL[ ix ] < (int)inst->P[ ix ].size())
                    {
                       if(a == inst->P[ ix ][ v.QL[ ix ] ]) 
		       {
			  p_u =  v.QL[ ix ] + 1;
			  add_feas_check = true; 
		       }
		       else
                          p_u = v.QL[ ix ];
                    }
                    else
                       p_u = v.QL[ ix ];
                    QL_a.push_back( p_u );
                }

                // check if for letter a we can embed the rest of pattern strings (inst->successors[i][a][v.pL[i] - 1] > inst->embed_end[ p ][i][p_u], forall p \in [1, k] )
		for (int i = 0; i < inst->m && is_feasible_letter; i++) { 
                     if ( inst->successors[i][a][v.pL[i] - 1] > (int)inst->S[i].size() && feas_embed_check( inst->successors[i][a][ v.pL[i] - 1 ], i, QL_a ) )
                     {
	                  is_feasible_letter = false;
	                  break;
                     }
    
		     pL_a.push_back(inst->successors[i][a][v.pL[i] - 1] + 1);
		}
	        	
		//if(feasibility() == 0) 
		//{ 
                   if (is_feasible_letter) {
		       feasibleSigma[a] = std::make_pair(pL_a, QL_a);
		   }
		/*}
		else // ukljucen feasiblity check:
		{   
		    int over = true;
	            for(int i = 0; i < inst->k && over; ++i) 
		    {
		        if(v.QL[ i ] < inst->P[ i ].size())
			   over = false;
		    }
                    if(!add_feas_check)
			add_feas_check =  over;
	            if(is_feasible_letter and add_feas_check) 
		    {
		        feasibleSigma[a] = std::make_pair(pL_a, QL_a);
		    }
		}*/

             }
	}
	return feasibleSigma;
}

bool CLCS_solution::feas(int letter, const Node &v)
{
     for(int i = 0; i < inst->m; ++i) 
     {
         if(inst->occurance_positions[i][ letter ][v.pL[i] - 1] == 0) //not fesible letter
           return false;
     }
      return true; 
}
/** Retrival feasible characters for extending node @v : TODO -- update */
map<int, pair< vector<int16_t>,  vector<int16_t>>> CLCS_solution::findFeasibleSigma(const Node &v, std::vector<std::vector<int>> Embed)
{
        map<int, pair< vector<int16_t>,  vector<int16_t>>> feasibleSigma;
        for(int i = 0;  i < v.pL.size(); ++i) 
            if(v.pL[ i  ] > inst->S[ i ].size()) 
            { 
               return   feasibleSigma;
            }

	for (int a = 0; a < inst->sigma; a++) // check if letter is feasible...
        {
             if(feas(a, v)) //not feasible letter
	     {	
                bool is_feasible_letter = true; 
		vector<int16_t> pL_a = std::vector<int16_t>();	// new position vector for extending node by letter a
		vector<int16_t> QL_a = std::vector<int16_t>();	// new position vector for patters
                bool add_feas_check = false;
		//map<int, vector<int16_t>> temp;  // temporary changes in Embed structure...
                for(int ix = 0; ix < inst->k; ++ix)
                {
                    int p_u;
                    if(v.QL[ ix ] < (int)inst->P[ ix ].size())
                    {
                       if(a == inst->P[ ix ][ v.QL[ ix ] ]){
                          p_u =  v.QL[ ix ] + 1; 
                          update_embed_column_pix(a, ix, p_u); // update those positions for P_ix w.r.t. all strings from S --> saved in <temp>
                          add_feas_check = true;
		       }else
                          p_u = v.QL[ ix ];
                    }
		    else
		    {
                      
                       p_u = v.QL[ ix ];
		    }
                    QL_a.push_back( p_u );
                }
                // check if for letter a we can embed the rest of pattern strings
		for (int i = 0; i < inst->m && is_feasible_letter; i++) {
		     pL_a.push_back(inst->successors[i][a][v.pL[i] - 1] + 1);
		}
	        //if( feasibility() == 0) // no feas
		//{	
		    if (is_feasible_letter and feasNode(pL_a, QL_a) ) {
		        feasibleSigma[a] = std::make_pair(pL_a, QL_a); 
		    }
		  /*}else{
		                      int over = true;                                                                                                                                                                                    for(int i = 0; i < inst->k && over; ++i)                                                                                                                                                                           {                                                                                                                                                                                                                      if(v.QL[ i ] < inst->P[ i ].size())                                                                                                                                                                                   over = false;                                                                                                                                                                                               }
                   if(!add_feas_check) 
	               add_feas_check =  over;   
		    if(is_feasible_letter and feasNode(pL_a, QL_a) and add_feas_check)
		    {
                        feasibleSigma[ a ] = std::make_pair(pL_a, QL_a); 
	            }
		
		}*/
                 pL_a.clear(); QL_a.clear(); 
             }
	}
	return feasibleSigma;
}
/** Retriving the set of non-dominating characters for extending node @v
    return @sigma_nd: (map of (char -> left position)): TODO -- update temp i snot properly updated */
map<int, pair<vector<int16_t>, vector<int16_t>>> CLCS_solution::findSigmaNd(const Node &v, std::vector<std::vector<int>> Embed)
{
	map<int, pair<vector<int16_t>, vector<int16_t>>> sigma_nd;
	map<int, pair< vector<int16_t>,  vector<int16_t>>> sigma_feasible = findFeasibleSigma(v, Embed);
 
	for (auto it1 = sigma_feasible.begin(); it1 != sigma_feasible.end(); ++it1) {
		int letter1 = it1->first;
		pair<vector<int16_t>, vector<int16_t>> pL_1_QL_1 = it1->second;
		bool letter1_is_dominated = false;
		for (auto it2 = sigma_feasible.begin(); it2 != sigma_feasible.end() && !letter1_is_dominated; ++it2) {
			int letter2 = it2->first;
			pair<vector<int16_t>, vector<int16_t>>  pL_2_QL_2 = it2->second;

			if (letter1 != letter2) {
			    letter1_is_dominated = check_domination(pL_1_QL_1.first, pL_1_QL_1.second, pL_2_QL_2.first, pL_2_QL_2.second);
			}
		}
		if (!letter1_is_dominated) {
			sigma_nd[letter1] = pL_1_QL_1;
		}
	}
	// give me only strong matches when feasibility() == 1 or if empty, give me the whole sigma_nd
        if( feasibility() == 1 ) 
        {   
            // filter only strong-extensions...
            map<int, pair<vector<int16_t>, vector<int16_t>>> sigma_nd_strong;
            for(auto x : sigma_nd)
            {
                bool add_feas_check = false;  
                for(int ix = 0; ix < inst->k && !add_feas_check; ++ix)
                {

                    if(v.QL[ ix ] < (int)inst->P[ ix ].size())
                    {
                        if(x.first == inst->P[ ix ][ v.QL[ ix ] ]){ 
                           int p_u =  v.QL[ ix ] + 1; 
                           update_embed_column_pix(x.first, ix, p_u); // update those positions for P_ix w.r.t. all strings from S --> saved in <temp
                           add_feas_check = true;
		        } 
                    }
                }
                if(add_feas_check){
                   sigma_nd_strong[x.first] = x.second; 
                }
            }
            if(sigma_nd_strong.size() == 0)
            {
               //cout << "  return sigma_nd ------------------------>" << endl;
               return sigma_nd;
           
            } else{
                //cout << "return sigma_nd_strong" << endl; 
                return sigma_nd_strong;
            } 
        }
        else 
	    return sigma_nd;
}

map<int, pair<vector<int16_t>, vector<int16_t>>> CLCS_solution::findSigmaNdBS(const Node &v )
{
	map<int, pair<vector<int16_t>, vector<int16_t>>> sigma_nd;
	map<int, pair< vector<int16_t>,  vector<int16_t>>> sigma_feasible = findFeasibleSigmaBS(v); 

	for (auto it1 = sigma_feasible.begin(); it1 != sigma_feasible.end(); ++it1) {
		int letter1 = it1->first;
		pair<vector<int16_t>, vector<int16_t>> pL_1_QL_1 = it1->second;
		/*bool letter1_is_dominated = false;
		for (auto it2 = sigma_feasible.begin(); it2 != sigma_feasible.end() && !letter1_is_dominated; ++it2) {
			int letter2 = it2->first;
			pair<vector<int16_t>, vector<int16_t>>  pL_2_QL_2 = it2->second;

			if (letter1 != letter2) {
			    letter1_is_dominated = check_domination(pL_1_QL_1.first, pL_1_QL_1.second, pL_2_QL_2.first, pL_2_QL_2.second);
			}
		}*/
		// if (!letter1_is_dominated) { 
			sigma_nd[letter1] = pL_1_QL_1;
                 //}
	}

        // give me only strong matches when feasibility() == 1 or if empty, give me the whole sigma_nd
        if( feasibility() == 1 ) 
        {   
            // filter only strong-extensions...
            map<int, pair<vector<int16_t>, vector<int16_t>>> sigma_nd_strong;
            for(auto x : sigma_nd)
            {
                bool add_feas_check = false;  
                for(int ix = 0; ix < inst->k && !add_feas_check; ++ix)
                {

                    if(v.QL[ ix ] < (int)inst->P[ ix ].size())
                    {
                        if(x.first == inst->P[ ix ][ v.QL[ ix ] ]){ 
                           int p_u =  v.QL[ ix ] + 1; 
                           //update_embed_column_pix(x.first, ix, p_u); // update those positions for P_ix w.r.t. all strings from S --> saved in <temp
                           add_feas_check = true;
		        } 
                    }
                }
                if(add_feas_check){
                   sigma_nd_strong[x.first] = x.second; 
                }
            }
            if(sigma_nd_strong.size() == 0)
            {
               //cout << "  return sigma_nd" << endl;
               return sigma_nd;
           
            } else{
                //cout << "return sigma_nd_strong" << endl; 
                return sigma_nd_strong;
            } 
        }
        else 
	    return sigma_nd;
}

/** domination check acc. to two left position vectors @pL_1 and @pL_2  and @QL_1 and @QL_2: */
bool CLCS_solution::check_domination( vector<int16_t> &pL_1,  vector<int16_t> &QL_1,  vector<int16_t> &pL_2, vector<int16_t> &QL_2) const
{
	int match_pos = 0;
	for (int j = 0; j < inst->m; ++j)
	     if (pL_1[j] >= pL_2[j])
		 match_pos++;

        for (int j = 0; j < inst->k; ++j)
	     if (QL_1[j] <= QL_2[j])
		 match_pos++;

        if( match_pos == inst->m  + inst->k )
            return true;  // pL_1 is dominated by pL_2
        else
            return false;  
}

/** Extending partail solution of node @parent with letter @letter whose; parameter @pL denotes already precalculated
    left positions of the new child node 
    return: @child: a new node created */
shared_ptr<Node> CLCS_solution::expandNode(const shared_ptr<Node>& parent, int letter, vector<int16_t>& pL, vector<int16_t>& QL) const
{
	shared_ptr<Node> child = std::make_shared<Node>(vector<int16_t>(parent->pL.size(), 0 ), vector<int16_t>(parent->QL.size(), 0 ));
	child->parent = parent;
	child->l_v = parent->l_v + 1;
        bool strong_match = false;
        for(int ix = 0; ix < inst->k; ++ix)
        {
	    if (parent->QL[ ix ] < (int) inst->P[ ix ].size())
            {
                if(child->QL[ ix ] ==  parent->QL[ ix ] + 1) 
                {  strong_match = true; }
	        child->QL[ ix ] = (inst->P[ ix ][parent->QL[ ix ]] == letter) ? parent->QL[ ix ] + 1 : parent->QL[ ix ];
            }
	    else
	        child->QL[ ix ] = parent->QL[ ix ];
        }

	child->pL.reserve(pL.size());
	child->pL.assign(pL.begin(), pL.end());

	child->QL.reserve(QL.size());
	child->QL.assign(QL.begin(), QL.end()); 

        /*if(strong_match)
           for(int i = 0; i < inst->m; ++i)
           child->additional_letters.push_back(child->pL[ i ]);  //assign(parent->pL.begin(), parent->pL.end());
        else{
           for(int i = 0; i < inst->m; ++i)
               child->additional_letters.push_back(parent->additional_letters[ i ]);    // assign( parent->additional_letters.begin(), parent->additional_letters.end());
        }*/
	return child;
}
/** from the generated search graph, retreve partial solution which correponds 
    to the given node @v */
void CLCS_solution::deriveSolution(Node *v) 
{       
	cout << "deriving..." << endl;
	//vector<int16_t> sx = vector<int16_t>();
	while (v->parent != nullptr) {
                //cout<< v->pL[0] - 2 << "-----> " << inst->S[0][ v->pL[0] - 2 ] << endl;  //<< inst->S[0][v->pL[0] - 2] << endl;
	       //sx.push_back((int16_t) (inst->S[0][v->pL[0] - 2]));
               s.insert(s.begin(), (int16_t) (inst->S[0][v->pL[0] - 2]) );
	       v = v->parent.get(); // v->print(); 
	} //cout << "Soo " << inst->S[0][0] << endl;
	string S="";
	for(int x: s) 
	   S += inst->int2Char[ x ];
    	cout << "\n" << S << endl;
	//std::reverse(sx.begin(), sx.end());cout << "reversing..." << endl;
	//return sx;
}

} // namespace clcs
