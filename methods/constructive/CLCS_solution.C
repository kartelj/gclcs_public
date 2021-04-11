#include <algorithm>
#include <assert.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <mh_util.h>
#include <mh_log.h>
#include <mh_param.h>
#include "AStar.h"
#include "BeamSearch.h"
#include "structures/BoundedHeap.h"
#include "CLCS_solution.h"
#include <set>
#include <unordered_set>
#include <new>

using namespace std;
using namespace mh;

namespace clcs {

/** choose algorithm to execute (external mhlib parameter **/
int_param algorithm("algorithm", "0: Approximation algorithm (Gotthilf) 1: GH (including HHBS); 2: RGH; 3: BS; 4: DP-Chin; 5: DP-AE; 6: DP-IR, 7:A*, 8: DP-Dia, 9: DP-Deo", 0, 0, 9);
/** execution time allowed for algorithms (default time limit is set to 900 sec) **/
double_param time("time", "ttime: time-limit  ", 900, 0, 10000);

/** Approximation algorithm (for m-CLCS problem) */
void CLCS_solution::Approx()
{
	std::vector<std::vector<int>> start(inst->m, std::vector<int>());
	std::vector<std::vector<int>> end(inst->m, std::vector<int>());

	// scan through input strings to compute start and end values
	for (int i = 0; i < inst->m; i++) {
		int p_u = 0;
		std::vector<int> s_i = inst->S[i];
		for (int j = 0; j < (int)s_i.size() && p_u < inst->p; j++) {
			if (s_i[j] == inst->P[p_u]) {
				start[i].push_back(j);
				p_u++;
			}
		}
	}
	for (int i = 0; i < inst->m; i++) {
		int p_u = inst->p - 1;
		std::vector<int> s_i = inst->S[i];
		for (int j = s_i.size() - 1; j >= 0 && p_u >= 0; j--) {
			if (s_i[j] == inst->P[p_u]) {
				end[i].push_back(j);
				p_u--;
			}
		}
		std::reverse(end[i].begin(), end[i].end());
	}

	// start with the actual Approx. algorithm
	int occ = 0;
	int b_loc = 0;
	int letter = -1;
	for (int j = 0; j < inst->p - 1; j++) {
	     // find best letter for insertion at current position (calculate C*)
	     int c_max = 0;
	     int max_letter = -1;
	     for (int a = 0; a < inst->sigma; a++) {
		  int c_min = INT_MAX;
		  for (int i = 0; i < inst->m; i++) {
		       auto begin_ij = inst->S[i].begin() + start[i][j] + 1;
		       auto end_ij = inst->S[i].begin() + end[i][j + 1] - 1;
		       int c_ai = 0;
		       if (begin_ij < end_ij) {
			   c_ai = std::count(begin_ij, end_ij, a);
		       }
		       if (c_ai < c_min) {
			   c_min = c_ai;
		       }
		  }
		  if (c_min > c_max) {
		      c_max = c_min;
		      max_letter = a;
		  }
	     }

	     // memorize best option
	     if (c_max > occ) {
		 letter = max_letter;
		 occ = c_max;
		 b_loc = j + 1;
	     }
	}
	// debug output for testing
	std::cout << "Results:\nbest letter: " << letter << ", occurrences: " << occ << ", bLoc: " << b_loc << "\n";

	// create the solution
	s = std::vector<int16_t>();
	s.insert(s.end(), inst->P.begin(), inst->P.begin() + b_loc);
	for (int i = 0; i < occ; i++) {
		s.push_back(letter);
	}
	s.insert(s.end(), inst->P.begin() + b_loc, inst->P.end());
}

template<typename T>                                                       
std::vector<std::vector<std::vector<T>>> make_3d_vector(int z, int y, int x, T value = T{})
{
    return std::vector<std::vector<std::vector<T>>>(z, std::vector<std::vector<T>>(y, std::vector<T>(x, value)));
}


/** Chin et al. algorithm */
int CLCS_solution::DP_Chin()
{
     // save in vector 
     auto M3d = make_3d_vector<int>( inst->P.size() + 1, inst->S[0].size() + 1, inst->S[1].size() + 1,  0);
     // boundary conditions (initializations):   
     for(int i = 0; i < (int)inst->S[ 0 ].size() + 1; ++i) 
         M3d[ 0 ][ i ][ 0 ]  = 0;
     
     for(int j = 0; j < (int)inst->S[ 1 ].size() + 1; ++j) 
         M3d[ 0 ][ 0 ][ j ]  = 0;

     for(int i = 0; i < (int) inst->S[ 0 ].size() + 1; ++i){
         for(int k = 1; k < (int) inst->P.size() + 1; ++k) 
             M3d[ k ][ i ][ 0 ]  = INT_MIN;
     }

     for(int j = 0; j < (int)inst->S[ 1 ].size() + 1; ++j){
         for(int k = 1; k < (int) inst->P.size() + 1; ++k) 
             M3d[ k ][ 0 ][ j ]  = INT_MIN;
     }
     // execute the DP recursion: 
     for( int i = 0; i <= (int) inst->S[ 0 ].size(); ++i){
          for(int j = 0; j <= (int) inst->S[ 1 ].size(); ++j){
             for(int k = 0; k <= (int) inst->P.size(); ++k){
                 if( ( i > 0 and j > 0 and k > 0 )
                    and  inst->S[ 0 ][ i - 1 ]  == inst->S[ 1 ][ j - 1 ] 
                    and  inst->S[ 1 ][ j - 1 ]  == inst->P[ k - 1 ] ) 
                    M3d[ k ][ i ][ j ] = M3d[ k - 1 ][ i - 1 ][ j - 1 ] + 1;
                 else
                 if( (i > 0 and j > 0)
                      and inst->S[ 0 ][ i - 1 ] == inst->S[ 1 ][ j - 1 ] 
                      and (k == 0 or inst->S[ 0 ][ i - 1 ] != inst->P[ k - 1 ]) )
                      M3d[ k ][ i ][ j ] = M3d[ k ][ i - 1 ][ j - 1 ] + 1;
                 else
                 if( (i > 0 and j > 0) and ( inst->S[ 0 ][ i - 1 ] != inst->S[ 1 ][ j - 1 ]) )
                      M3d[ k ][ i ][ j ]  =  std::max (  M3d[ k ][ i - 1 ][ j ], M3d[ k ][ i ][ j - 1 ]);
             }
          }
     }
     // TODO: retrieving optimal solution
     int k = inst->P.size(); int i = (int) inst->S[ 0 ].size(); 
     int j = (int) inst->S[ 1 ].size(); int len = 0;
     //cout << "optimal solution " << M3d[ k ][ i ][ j ]  << " " << M3d[ 0 ][ 0 ][ 0 ] <<  endl;
     while(!(i == 0 and j == 0 and k == 0))
     {    
             if( ( i > 0 and j > 0 and k > 0 )
                    and  inst->S[ 0 ][ i - 1 ]  == inst->S[ 1 ][ j - 1 ] 
                    and  inst->S[ 1 ][ j - 1 ]  == inst->P[ k - 1 ] ){
                    s.push_back(inst->S[ 0 ][ i - 1 ]); i--; j--; k--; len++;
              }
              else if((i > 0 and j > 0)
                      and inst->S[ 0 ][ i - 1 ] == inst->S[ 1 ][ j - 1 ] 
                      and (k == 0 or inst->S[ 0 ][ i - 1 ] != inst->P[ k - 1 ]) ){

                      s.push_back(inst->S[ 0 ][ i - 1 ]); i--; j--; len++;
              }
              else if(j > 0 and i > 0){
                       
                     if (M3d[ k ][ i - 1 ][ j ] > M3d[ k ][ i ][ j - 1 ])
                         i--;
                     else
                         j--;
                  }
               else 
                  break;
     }
     // reverse the vector to get the optimum
     std::reverse(s.begin(), s.end());
     return M3d[ (int) inst->P.size() ][ (int) inst->S[ 0 ].size() ][ inst->S[ 1 ].size() ];
}

int CLCS_solution:: DP_AE()
{
     //Initialize data structures: 
     auto M3d = make_3d_vector<int>( inst->P.size() + 1, inst->S[0].size() + 1, inst->S[1].size() + 1,  0); 
     auto M3d_prime = make_3d_vector<int>( inst->P.size() + 1, inst->S[0].size() + 1, inst->S[1].size() + 1,  0);
     auto M3d_sec = make_3d_vector<int>( inst->P.size() + 1, inst->S[0].size() + 1, inst->S[1].size() + 1,  0);
     auto M3d_ter = make_3d_vector<int>( inst->P.size() + 1, inst->S[0].size() + 1, inst->S[1].size() + 1,  0);
     //further initial steps: 
     for(int i = 0; i <= (int) inst->S[ 0 ].size(); ++i){
         for(int j = 0; j <= (int) inst->S[ 1 ].size(); ++j){
             for(int k = 0 ; k < (int) inst->P.size(); ++k){
                 M3d[ k ][ i ][ 0 ] = 0; 
                 M3d[ k ][ 0 ][ j ] = 0;
             }
         }
     }
     //cout << " run recursion..." << endl;
     // fill M''
     for(int i = 1; i <= (int) inst->S[ 0 ].size(); ++i)
         for(int j = 1; j <= (int) inst->S[ 1 ].size(); ++j)
             for(int k = 0; k <= (int) inst->P.size(); ++k){
                 // M''
                 if( ( k == 1 or ( k > 1 and M3d[ k - 1 ][ i - 1 ][ j - 1 ] > 0 ) ) 
                     and inst->S[ 0 ][ i - 1 ] == inst->S[ 1 ][ j - 1 ] and inst->S[ 1 ][ j - 1] == inst->P[ k - 1 ] ){
                         M3d_sec[ k ][ i ][ j ] = M3d[ k - 1 ][ i - 1 ][ j - 1 ] + 1; 
                 }
                 else 
                     M3d_sec[ k ][ i ][ j ] = 0;   
                 // M'''
                 if( ( k == 0 or M3d[ k ][ i - 1 ][ j - 1 ] > 0 ) and inst->S[ 0 ][ i - 1 ] == inst->S[ 1 ][ j - 1 ] ) 
                       M3d_ter[ k ][ i ][ j ] = M3d[ k ][ i - 1 ][ j - 1 ] + 1; 
                 else  
                       M3d_ter[ k ][ i ][ j ] = 0;
                 // M'
                 M3d_prime[ k ][ i  ][ j ] = std::max ( M3d_sec[ k ][ i ][ j ], M3d_ter[ k ][ i ][ j ]);
                 //M:
                 M3d[ k ][ i ][ j ] = std::max( M3d_prime[ k ][ i ][ j ], std::max( M3d[ k ][ i - 1 ][ j ], M3d[ k ][ i ][ j - 1 ]));
             } 
     cout << "Retrieving results.." << endl;
     int k = inst->P.size(); int i = (int) inst->S[ 0 ].size(); 
     int j = (int) inst->S[ 1 ].size(); int len = 0;
     while(!(i == 0 and j == 0 and k == 0))
     {    
             if( ( i > 0 and j > 0 and k > 0 )
                    and  inst->S[ 0 ][ i - 1 ]  == inst->S[ 1 ][ j - 1 ] 
                    and  inst->S[ 1 ][ j - 1 ]  == inst->P[ k - 1 ] ){
                    s.push_back(inst->S[ 0 ][ i - 1 ]); i--; j--; k--; len++;
              }
              else if((i > 0 and j > 0)
                      and inst->S[ 0 ][ i - 1 ] == inst->S[ 1 ][ j - 1 ] 
                      and (k == 0 or inst->S[ 0 ][ i - 1 ] != inst->P[ k - 1 ]) ){

                      s.push_back(inst->S[ 0 ][ i - 1 ]); i--; j--; len++;
              }
              else if(j > 0 and i > 0){
                       
                     if (M3d[ k ][ i - 1 ][ j ] > M3d[ k ][ i ][ j - 1 ])
                         i--;
                     else
                         j--;
                  }
               else 
                  break;
     }
     // reverse the vector to get an optimum
     std::reverse(s.begin(), s.end()); // final reverting --> optimal solution
     return M3d[ inst->P.size() ][ inst->S[ 0 ].size() ][ inst->S[ 1 ].size() ];
}

/** Diagonal-based approach  **/

void CLCS_solution:: display(vector<Btriple>& D)
{
     for(auto& di: D)
     {
         cout << di.x << " " << di.y << " " << di.z << endl;
     }
}

/** Hung et al. approach for 2-CLCS problem; 
    it takes two input strings @A and @B and pattern @T, 
    lenght of pattern @Tlen, and max size of strings @size 
    return: @lenght_of_optimal_solution
*/
int CLCS_solution:: Diagonal(vector< int> T, vector< int> A, vector< int>B, int Tlen, int size) {

    int locationA = 0;
    int locationB = 0; 
    // for statistics
    int stat_nodes_created = 0;
    int Alen = inst->S[0].size();
    int Blen = inst->S[1].size();
    // Next_A and Next_B structure (see Hung et al. paper) 
    vector< vector< int>> nextA(size + 1, vector<int>(Alen + 1, 0));
    vector< vector< int>> nextB(size + 1, vector<int>(Blen + 1, 0));
    //construct array of nextA
    for (int i = 1; i <= size; ++i) {
        locationA = Alen + 1;
        for (int j = Alen; j > 0; --j) {
            if (j == Alen)
                nextA[i][j] = locationA;
            if (A[j-1] == i) {
                locationA = j;
                nextA[i][j - 1] = locationA;
            }
            else {
                nextA[i][j - 1] = locationA;
            }
        }
    }
    //construct array for next_B
    for (int i = 0; i <= size - 1; ++i) {
        locationB = Blen + 1;
        for (int j = Blen; j > 0; --j) {
            if (j == Blen)
                nextB[i][j] = locationB;
            if (B[ j - 1 ] == i) {
                locationB = j;
                nextB[i][j - 1] = locationB;
            }
            else {
                nextB[i][j - 1] = locationB;
            }
        }
    }
    //limit_B structure (see Hung et al.)
    vector<int> limitB(Tlen + 1, -1);
    for(int j = Blen; j >= 1; j--) 
    {
        if(T[ Tlen - 1 ] == B[ j - 1 ]){         
           limitB[ Tlen ] = j;
           break;
        }
    }
    //remaining indices:
    for(int k = Tlen - 1; k >= 1; k--)
    {
        for(int j = Blen - 1; j >= 0; j--)
        {
            if(T[ k -  1 ] == B[ j - 1 ]  and j < limitB[ k + 1 ]) {
               limitB[ k ] = j; 
               break;
            }
        }
    }

    int L = 0;
    Btriple bt = Btriple(0, 0, Tlen);
    vector<vector<vector<Btriple>>> D; //(Alen+1, vector<vector<Btriple>>(Alen+1, vector<Btriple>(1, bt) ) );
    // initialize D_{i,l}
    for(int i = 0; i <= Alen; ++i)
    {
        vector<vector<Btriple>> ci;
        for(int j = 0; j < Alen; ++j)
        {
            vector<Btriple> cij;
            ci.push_back(cij); 
        }
        D.push_back(ci);
    } 
   
    for(int k = 1; k < Tlen; ++k)
    {  
        if( limitB[ k ] <= 0 ){
            cout << "no solution" << endl;
            return 0;
        }
    }

    for( int t = 1; t <= Alen; ++t ){

         D[ t - 1 ][ 0 ].push_back(bt);
         int l = 1;
         for(int i = t; i <= Alen; ++i)
         {
             auto iter_begin = D[ i - 1 ][ l - 1 ].begin();
             auto iter_end = D[ i - 1 ][ l - 1 ].end();
 
             for(auto it = iter_begin; it != iter_end; ++it)
                 Expand(*it, nextB, limitB, i, Tlen, D[ i ][ l ]);
              //check domination in union, store in D[i][l]; sort and then compare  
              Domination(D[ i ][ l  ], D[ i - 1 ][l], Tlen);
              stat_nodes_created += ( D[ i ][ l ].size() - D[ i - 1 ][l].size() );
              if(D[i][l].empty() or D[i][l][0].z == -1 )
                 break;
              //find largest L:
              for(int x = 0; x <= (int) D[i].size() - 1; ++x)
              {
                  if(!D[i][x].empty() and D[i][x][0].z == 0) //sort-out the elements of D[i,x] acc. to h-value, asc. order     
                  {
                     L = x;// max L
                     break;
                  }
              }  
              l = l + 1;
         }
         if( (Alen - t) <= L ){
              retrieve_diagonal(D, L);
              mh::out() << "Node Stats (created, expanded, ignored, merged): ";
              mh::out() << stat_nodes_created << ", 0, 0, 0 " <<  std::endl; //stat
              return L;
         }
    }
    // print the stats for Hung's algorithm
    mh::out() << "Node Stats (created, expanded, ignored, merged): ";
	mh::out() << stat_nodes_created << ", 0, 0, 0 " <<  std::endl; //stat
    for(int i = 0; i < Alen; ++i)
    {   for(int l = L; l >= 0; --l)
        {
            if(!D[ i ][ l ].empty() and D[i][l][0].z == 0){
               retrieve_diagonal(D, l);
               return l;   
            }
        }
    }
    return 0;    
} 

void  CLCS_solution::retrieve_diagonal(vector<vector<vector<Btriple>>>& D, int L)
{
      cout << "retrieving optimal solution in Hung's algorithm..." << endl; 
      int k_i = 0; int k_j = 0; int k_z = 0; 
      bool complete = false; int ix = L;  
       
      while(ix <= (int)inst->S[0].size()) 
      {
          for(Btriple& cx: D[ix][ L ]){  

             if( cx.z > -1)
             {     
                 k_i = cx.x; 
                 k_j = cx.y;
                 k_z = cx.z; 
                 if(k_z == 0)
                    complete = true;
                 break;
             }
         }
         ix++; 
         if(k_i != 0) // found ix
            break;
      }
      
      if(complete) // is complete
         s.push_back(inst->S[ 0 ][ k_i - 1 ]);
      L--;  ix--; // update L
      Btriple btr = Btriple();
      while(ix >= 0 and L > 0)
      {     
            bool found = false;
            for(auto cx: D[ ix ][ L ]){

                if(cx.x < k_i and cx.y < k_j and (cx.z == k_z or (cx.z == k_z + 1)))
                {
                   btr = cx;
                   k_i = cx.x; 
                   k_j = cx.y;
                   k_z = cx.z;
                   s.push_back(inst->S[ 0 ][ k_i - 1 ]);
                   L--; ix--; found = true;
                }      
            }
            //update ix --> ix - 1;
            if(!found)
            {   
               ix--;
               while(ix > 0)
               {  
                      bool move_up = false;
                      for(auto cx: D[ ix ][ L ]){
                          if(cx.x == k_i and cx.y == k_j and cx.z == k_z)
                             move_up = true;
                      }
                      if(move_up and ix > 0) 
                         ix--;
                      else
                         break;
               }
            }
      }
      //make reversed
      reverse(s.begin(), s.end());
}

void CLCS_solution::Expand(Btriple& c, vector<vector<int>>& nextB, vector<int>& limitB, int i, int Tlen, vector<Btriple>& D_il)
{
      int i_prime = c.x; // is given already by i
      int j_prime = c.y;
      int h_prime = c.z;
      // the coordinates of expanded nodes:
      int j = nextB[ inst->S[ 0 ][ i - 1 ] ][ j_prime ]; // i next letter --> i+1  
      int k = Tlen - h_prime + 1;
      if(j <= (int) inst->S[ 1 ].size() and h_prime == 0) // complete node
      {
         D_il.push_back(Btriple(i, j, 0));  
      }
      else
      if( j <= (int) inst->S[ 1 ].size() and j <= limitB[k] ){ // valid expansion
                   
          if(inst->S[ 0 ][ i - 1 ] == inst->P[ k - 1 ]){
             D_il.push_back(Btriple(i, j, h_prime - 1)); 
          }
          if(inst->S[ 0 ][i - 1] != inst->P[ k - 1 ] ){
             D_il.push_back(Btriple(i, j, h_prime));
          }
      }else
          return;
}


void CLCS_solution::Domination(vector<Btriple>& set1, vector<Btriple>& set2, int Tlen)
{
    vector<Btriple> listV(Tlen + 1, Btriple());
    //union of both sets: Dx
    unordered_set<Btriple, BtripleHasher, BtripleComparator> Dx;
    sort(set1.begin(), set1.end());
    sort(set2.begin(), set2.end());
    std::vector<Btriple>::iterator it = set1.begin();
    // Iterate till the end of set 
    while (it != set1.end())
    {      
	   Dx.insert(*it);
	   it++; 
    }
    std::vector<Btriple>::iterator it2 = set2.begin();
    while(it2 != set2.end() )
    {
        Dx.insert(*it2);
        it2++;
    }
    //clear dominance set
    set1.clear();
    //filter dominated nodes
    for(auto ijh: Dx)
    {
        if(listV[ ijh.z ].z == -1) // not empty
           listV[ ijh.z ] = ijh;
        else 
        if(dominates( ijh, listV[ ijh.z ]) )  
        {
           listV[  ijh.z  ] = ijh;  
        }
    }
    // Dx.clear();
    int p = 0; int q = 1;
    while(p <= Tlen and q <= Tlen) 
    {
          if( (listV[ p ].z > -1 and listV[ q ].z > -1) and 
              dominates(listV[ p ], listV[ q ]) )
          {
              listV[ q ] = Btriple(); // become inactive
              q = q + 1; 
          }
          else{
              p = q; 
              q = q + 1;
          } 
    }
    set1.clear();  
    for(auto ijh : listV)
    {
        if(ijh.z > -1) //active
           set1.push_back(ijh); 
    }
    Dx.clear();
    //return Dx;
}

/** domination relation definition in D_{i,l} table (see Hung et al. paper) */
bool CLCS_solution::dominates(Btriple& c1, Btriple& c2){
     
     bool t =  (c1.y <= c2.y and c1.z <= c2.z ) 
               or (c1.x < c2.x and c1.y == c2.y and c1.z == c2.z); 
    return t; 
}

int CLCS_solution::Diagonal_based()
{
    int sol = 0;
    sol = Diagonal(inst->P, inst->S[ 0 ], inst->S[ 1 ], inst->P.size(), inst->sigma);
    out() << "obj: " << sol << endl;
    return sol;

}

/** end of diagonal-based appraoch **/

/** start with Illinopulos and Rahman algorithm */

int CLCS_solution::DP_IR()
{
	// references to create simple alias of input strings
	const std::vector<int> &s0 = inst->S[0];
	const std::vector<int> &s1 = inst->S[1];

	// matrix for results
	auto M3d = make_3d_vector<IR_Item>(inst->P.size() + 1, s0.size(), s1.size(), IR_Item());

	// preprocess matches
	auto M = std::vector<std::vector<tuple<int, int>>>(s0.size(), std::vector<tuple<int, int>>());
	for (int i = 0; i < (int) s0.size(); i++) {
		for (int j = 0; j < (int) s1.size(); j++) {
			if (s0[i] == s1[j]) {
				M[i].emplace_back(i, j);
			}
		}
	}

	// initialize bounded heap data structures
	IR_Item best = IR_Item();
	auto h1 = std::vector<std::unique_ptr<BoundedHeap>>(); // H_i
	auto h2 = std::vector<std::unique_ptr<BoundedHeap>>(); // H_i
	auto *h_prev = &h1;
	auto *h_cur = &h2;
	for (int k = 0; k <= inst->p; k++) {
		h_prev->push_back(std::unique_ptr<BoundedHeap>(new BoundedHeap(s1.size())));
		h_cur->push_back(std::unique_ptr<BoundedHeap>());
	}
	// process main loop of algorithm
	for (int i = 0; i < (int) s0.size(); i++) {
		for (int k = 0; k <= inst->p; k++) {
			// create new bounded heap with data copied from previous index
			auto h = std::unique_ptr<BoundedHeap>(new BoundedHeap(*h_prev->at(k)));
			// compute matrix value for every match by determining v1 and v2 (see DP formulation of Illinopulos and Rahman)
			for (tuple<int, int> match : M[i]) {
				int j = std::get<1>(match);
				IR_Item max1;
				if (k >= 1) {
					max1 = h_prev->at(k-1)->getMax(j);
				}
				IR_Item max2 = h_prev->at(k)->getMax(j);
				IR_Item v2 = IR_Item();
				if (k == 0 || max2.val > 0) {
					v2 = max2;
					v2.val++;
				}
				IR_Item v1 = IR_Item();
				if (k >= 1 && s0[i] == inst->P[k-1]) {
					if (k == 1 || (k > 1 && max1.val > 0)) {
						v1 = max1;
						v1.val++;
					}
				}
				IR_Item maxResult;
				if (v1.val > v2.val) {
					maxResult = v1;
				} else {
					maxResult = v2;
				}
				if (maxResult.val > 0) {
					// a feasible solution for the subproblem is found
					M3d[k][i][j] = maxResult;
					h->increase(j, maxResult.val, std::make_tuple(i, j, k));
				}
				if (k == inst->p && best.val < maxResult.val) {
					// store the solution of the globally longest subsequence found so far
					best = IR_Item(maxResult.val, i, j, k);
				}
			}
			// delete old heap and store current heap for next iteration
			(*h_cur)[k] = std::move(h);
		}
		/* we only need to store the heap for current and previous i, so at the end of each iteration,
		 * h_prev can be deleted and h_cur becomes the new h_prev; this can efficiently be done by swapping pointers */
		std::swap(h_cur, h_prev);
	}

	// determine CLCS
	s = std::vector<int16_t>();
	IR_Item e = best;
	while (int(s.size()) < best.val) {
		s.push_back(s0[e.x]);
		e = M3d[e.z][e.x][e.y];
	}
	std::reverse(s.begin(), s.end());

	return best.val;
}
/** end with IR algorithm  */

/** Start with Deorowicz algorithm */
int CLCS_solution::DP_Deo()
{
	// variables for stats
	long stat_nodes_created = 0;
	long stat_nodes_in_list = 0;

	// references to create simple alias of input strings
	const std::vector<int> &s0 = inst->S[0];
	const std::vector<int> &s1 = inst->S[1];
        
	// T and F matrix structure initialization
	std::vector<std::vector<int>> T = std::vector<std::vector<int>>(s1.size(), std::vector<int>(s0.size(), INT_MIN));
	auto F = make_3d_vector<Deo_Item>(inst->p + 1, s1.size(), s0.size());

	// compute positions of letters in first sequence
	std::vector<int> n_pos = std::vector<int>(inst->sigma, 0);
	std::vector<std::vector<int>> pos = std::vector<std::vector<int>>(inst->sigma, std::vector<int>(s0.size(), INT_MIN));
	for (int i = 0; i < (int) s0.size(); i++) {
		pos[s0[i]][n_pos[s0[i]]] = i;
		n_pos[s0[i]] = n_pos[s0[i]] + 1;
	}

	// main loop to traverse the matrices in a level-wise matter (following the pseudeocode of Deorowicz in the cooresp. paper)
	for (int k = 0; k <= inst->p; k++) {
        std::vector<Deo_Item> L0 = std::vector<Deo_Item>();
        L0.push_back(Deo_Item());
        if (k==0) {
            L0[0].len = 0;
        }
        int N0 = 1;
        int N1 = 0;
        for (int j = 0; j < (int) s1.size(); j++) {
            std::vector<Deo_Item> L1 = std::vector<Deo_Item>();
            L1.push_back(L0[0]);
            N1++;
            int p = 1;
            for (int s = 0; s < n_pos[s1[j]]; s++) {
                int i = pos[s1[j]][s];
                while (p < N0) {
                    if (L0[p].i >= i)
                        break;
                    else if (L1[N1 - 1].len < L0[p].len && L0[p].len > 0) {
                        L1.push_back(L0[p]);
                        N1++;
                    }
                    p++;
                }
                int v;
                if (k > 0 && s1[j] == inst->P[k-1]) {
                    v = T[j][i];
                    T[j][i] = L0[p-1].len + 1;
                } else {
                    v = L0[p-1].len + 1;
                    T[j][i] = v;
                }
                F[k][j][i] = Deo_Item(L0[p-1].i, L0[p-1].j, v);
                if (L1[N1-1].len < v) {
                    L1.push_back(Deo_Item(i, j, v));
                    N1++;
					stat_nodes_in_list++;
                }
                stat_nodes_created++;
            }
            while (p < N0 && L1[N1-1].len >= L0[p].len) {
                p++;
            }
            while (p < N0) {
                L1.push_back(L0[p]);
                N1++;
                p++;
            }
            L0 = L1;
            N0 = N1;
            N1 = 0;
        }

        if (k == inst->p) {
            // determine CLCS solution
	    s = std::vector<int16_t>();
	    Deo_Item item = L0[N0 - 1];
	    int k_sol = inst->p;
	    while (item.len > 1) {
		   s.push_back(s1[item.j]);
		   if (k_sol > 0 && s1[item.j] == inst->P[k_sol - 1]) {
		       k_sol--;
		   }
		   item = F[k_sol][item.j][item.i];
	    }
            std::reverse(s.begin(), s.end());

	    // print node stats
	    mh::out() << "Node Stats (created, expanded, ignored, merged): ";
	    mh::out() << stat_nodes_created << ", " << stat_nodes_in_list << ", 0, 0" << std::endl;
        }
     }
     return s.size();
}

/** Run BS (check BeamSearch.h) */
void CLCS_solution::BS()
{
	BeamSearch bs = BeamSearch(this);
        cout << "BS start " << endl;
	bs.startSearch();
	s = bs.getSolution();
}
/** Run BS (check AStar.h) */
void CLCS_solution::A_Star()
{
	AStar a_star = AStar(this);
	s = a_star.startSearch();
	a_star.printStatistics();
}
/** Run Greedy (for m-CLCS problem) */
void CLCS_solution::Greedy()
{
	BeamSearch bs = BeamSearch(this);
	bs.startGreedy();
	s = bs.getSolution();
}
/** Run Randomized Greedy (for m-CLCS problem) */
void CLCS_solution::RandomizedGreedy()
{
	BeamSearch bs = BeamSearch(this);
        cout << "Start randomized greedy (RGH)" << endl;
	bs.startGreedy(0.2);
	s = bs.getSolution();
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
	os << "|s|=" << s.size();
	os << " s=";
	if (s.size() == 0)
		os << "-";
	bool first = true;
	for (int a : s) {
		if (first)
		    first = false;
		else
		    os << " ";
		os << a;
	}

	if (detailed == 1)
	    os << " LCS=" << getSolution();

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
	if (!is_SubSequence(inst->P, sol_int)) {
		feasible = false;
		cout << "Constraint string is not part of the solution" << endl;
	}

	assert(feasible);
}

/** Retrival feasible characters for extending node @v */
map<int, vector<int16_t>> CLCS_solution::findFeasibleSigma(const Node &v) const
{
	map<int, vector<int16_t>> feasibleSigma;
	for (int a = 0; a < inst->sigma; a++) {
		bool is_feasible_letter = true;
		vector<int16_t> pL_a = std::vector<int16_t>();	// new position vector for extending node by letter a
		int p_u;
                if(v.u_v < (int)inst->P.size())
                   p_u = (a == inst->P[v.u_v]) ? v.u_v + 1: v.u_v;
                else
                   p_u = v.u_v;
		for (int i = 0; i < inst->m; i++) {
		     if ((inst->successors[i][a][v.pL[i] - 1] > inst->embed_end[i][p_u] )  || v.pL[i] > (int)inst->S[i].size()) {
			     is_feasible_letter = false;
			     break;
		     }
		     pL_a.push_back(inst->successors[i][a][v.pL[i] - 1] + 1);
		}
		if (is_feasible_letter) {
		    feasibleSigma[a] = pL_a;
		}
	}
	return feasibleSigma;
}
/** Retriving the set of non-dominating characters for extending node @v
    return @sigma_nd: (map of (char -> left position)) */
map<int, vector<int16_t>> CLCS_solution::findSigmaNd(const Node &v) const
{
	map<int, vector<int16_t>> sigma_nd;
	map<int, vector<int16_t>> sigma_feasible = findFeasibleSigma(v);
	for (auto it1 = sigma_feasible.begin(); it1 != sigma_feasible.end(); ++it1) {
		int letter1 = it1->first;
		vector<int16_t> pL_1 = it1->second;
		bool letter1_is_dominated = false;
		for (auto it2 = sigma_feasible.begin(); it2 != sigma_feasible.end() && !letter1_is_dominated; ++it2) {
			int letter2 = it2->first;
			vector<int16_t> pL_2 = it2->second;
			if (letter1 != letter2) {
				letter1_is_dominated = check_domination(pL_1, pL_2);
			}
		}
		if (!letter1_is_dominated) {
			sigma_nd[letter1] = pL_1;
		}
	}
	return sigma_nd;
}
/** domination check acc. to two left position vectors @pL_1 and @pL_2 */
bool CLCS_solution::check_domination(const vector<int16_t> &pL_1, const vector<int16_t> &pL_2) const
{
	int match_pos = 0;
	for (int j = 0; j < inst->m; ++j) {
	     if (pL_1[j] == pL_2[j])
		 match_pos++;
	     else 
             if (pL_1[j] < pL_2[j]) {
		 return false; // pL_1 is not dominated by pL_2
	     }
	}
	if (match_pos == (int) pL_2.size()) // the same pL-vector
	    return false;

	return true; // pL_1 is dominated by pL_2
}

/** Extending partail solution of node @parent with letter @letter whose; parameter @pL denotes already precalculated
    left positions of the new child node 
    return: @child: a new node created */
shared_ptr<Node> CLCS_solution::expandNode(const shared_ptr<Node>& parent, int letter, const vector<int16_t>& pL) const
{
	shared_ptr<Node> child = std::make_shared<Node>();
	child->parent = parent;
	child->l_v = parent->l_v + 1;
	if (parent->u_v < (int) inst->P.size())
	    child->u_v = (inst->P[parent->u_v] == letter) ? parent->u_v + 1 : parent->u_v;
	else
	    child->u_v = parent->u_v;
	child->pL.reserve(pL.size());
	child->pL.assign(pL.begin(), pL.end());
	return child;
}
/** from the generated search graph, retreve partial solution which correponds 
    to the given node @v */
vector<int16_t> CLCS_solution::deriveSolution(Node *v) const
{
	vector<int16_t> s = vector<int16_t>();
	while (v->parent != nullptr) {
	       s.push_back(inst->S[0][v->pL[0] - 2]);
	       v = v->parent.get();
	}
	std::reverse(s.begin(), s.end());
	return s;
}

} // namespace clcs
