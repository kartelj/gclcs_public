#include "CLCS_evaluation.h"

using namespace clcs;

namespace clcs_eval {

double greedy(const Node &v, const CLCS_inst *inst)
{

        Node *parent = v.parent.get();
	double g = 0.0; int l_min = 100000; 

	for (int i = 0; i < inst->m; i++) {
	     g += (v.pL[i] - v.pL[i] + 1) / (double) (inst->S[i].size() - v.pL[i] + 1); // parent->pL ===> additional_letters
             if( l_min > (v.pL[i] - v.pL[i] ) )
                 l_min = v.pL[i] - v.pL[ i ]; // pL ==> additional_letters
	}   
        
	return g;
}


long double h_prob(const Node &v, const CLCS_inst *inst, int p)
{
	long double h_val = 1.0;  //cout << " inst->P_m.size() " << inst->P_m.size() << endl; 
	for (int i = 0; i < inst->m; ++i) {
                // if(v.pL.size() > 0)
		   h_val *= inst->P_m[ p ][inst->S[i].size() - v.pL[i] + 1]; // v.pL[i] ==> v.additional_letters[i]
	}
	// check this?
	/*if ( (int)inst->P.size() - v.u_v  <= p)
		 h_val *= inst->P_m[ inst->P.size() - v.u_v ][ p ]; */

	return h_val;
}

long double h_prob_gen(const Node &v, const CLCS_inst *inst, int p) 
{ 
     long double h_val = 1;
     for(int i = 0; i < inst->m; ++i)  
     {  
	  for(int j = 0; j < inst->P.size(); ++j) 
          {
              if(inst->P[j].size() > v.pL[j]) //
	      {		      
                 std::unordered_map<int, long double, HashFunction> mapa = inst->LookheadMap[ j ][ v.QL[j] ][ i ];
                 // h_val *= inst->LookheadMap[ j ][ v.QL[j] ][ i ][ i ];
	         int index =   v.pL[i] - 1;
		 while(mapa.find(index) == mapa.end() and index < inst->S[i].size() )
		 {
		       index++;
		 }
		 if(index >= inst->S[i].size())
		      return 0.0;
		 else
		 {
		      h_val *= mapa[ index ]; 
		 }
	      }
	  }
     } 
     return h_val;
}

int ub1(const Node &v, const CLCS_inst *inst)
{
	int ub1 = 0;
	vector<int> sigma_s;
	vector<int> min_occurances_node((int) inst->sigma, (int) inst->S[0].size());

	for (int a = 0; a < inst->sigma; a++) {
		for (int i = 0; i < inst->m; i++) {
			if (v.pL[i] - 1 < (int16_t) inst->S[i].size()) //not feasible letter
			{
			    if (inst->occurance_positions[i][a][v.pL[i] - 1] < min_occurances_node[a])
				min_occurances_node[a] = inst->occurance_positions[i][a][v.pL[i] - 1];
			} 
                        else min_occurances_node[a] = 0; // some of indices of pL exceed max indices...
		}
	}
	for (unsigned int i = 0; i < min_occurances_node.size(); i++) {
		//if(min_occurances_node[i] >= 1)
		ub1 += min_occurances_node[i]; // sum of the c_a
	}
	return v.l_v + ub1;
}

int ub2(const Node &v, const CLCS_inst *inst)
{
	int ub2 = inst->S[0].size();
	for (int i = 0; i < inst->m - 1; ++i) {
	     if (ub2 > inst->M_lcs[i][v.pL[i] - 1][v.pL[i + 1] - 1])
		 ub2 = inst->M_lcs[i][v.pL[i] - 1][v.pL[i + 1] - 1];
	
	}
	return v.l_v + ub2;
}

/* LCS upper bound, min (UB_1, UB_2) */
int ub(const Node &v, const CLCS_inst *inst)
{
	int ub_1 = clcs_eval::ub1(v, inst);
	int ub_2 = INT_MAX;
	if(inst->M_lcs.size() > 0)
	   ub_2 = clcs_eval::ub2(v, inst);
        int ub_3 = 10000; //inst->M[ 0 ][ v.pL[ inst->c_x ] - 1 ][ v.pL[ inst->c_y ] - 1 ][v.QL[ inst->c_p ] ];
        //if( min(ub_1, ub_2) > v.l_v  + ub_3 )
	//
	////    cout << "------------------------------> " << std::min(ub_1, ub_2) << " " << ub_3 << endl;i
	double penality = 0.0;
	for(int i = 0; i < v.QL.size(); ++i) 
            penality += inst->P[ i ].size() - v.QL[i];
         //penality = (penality + 0.0) / std::sqrt(inst->sigma);
        if(inst->M_lcs.size() > 0 )
	   return  std::min(ub_1, std::min(ub_2, v.l_v + ub_3)); //+ penality; //std::min(ub_2, ub_3));
        else
          return   ub_1;
}

long double penality(const Node& v, const CLCS_inst * inst, int p = 0)
{   
	       double penalty = 0.0; float maxX = 0;
	       for(int i = 0; i < v.QL.size(); ++i)
	       {     penalty += inst->P[ i ].size() - v.QL[i];
		     if(maxX < inst->P[i].size() - v.QL[i])
			maxX = inst->P[i].size() - v.QL[i];
	       }
	       double avg = penalty / inst->P.size();
	       double std = 0.0;
	       for(int i = 0; i < v.QL.size(); ++i) 
                   std += std::pow( inst->P[i].size() - v.QL[i] - avg, 2);
            	      
	       return  (-1) * maxX +  (-1) * std;
               //return ub(v, inst) +  (-1) * penalty;             
}

long double probability(const Node& v,const CLCS_inst* inst, int p = 0) 
{  
    long double min = 10000000000; double avg = 0.0; long double std = 0.0;
    vector<long double> vals; 
    for(int i = 0; i < v.QL.size(); ++i) 
    {
        for(int j = 0; j < inst->S.size(); ++j)
	{   
	    if( v.QL[i] < inst->P[i].size())
	    {std::unordered_map<int, long double, HashFunction> FX = inst->LookheadMap[ i ][ v.QL[i] ][j]; 		
            int position_right = v.pL[j] - 1;
	    if( FX.size() > 0)   
	    { 
              while( position_right < inst->S[j].size() and FX.find(position_right) == FX.end() ) 
              {
                  position_right++;
              }
              if(position_right >= inst->S[j].size())
	        return 0.0;
	      avg += FX[ position_right ];
	      //vals.push_back(FX[ position_right ]); 
	      if( FX[ position_right ] < min ) 
	          min = FX[ position_right ];
	    }
	    } 
	}
    }
    /*for(long double xx: vals)
    {
         std += pow((xx-avg),2); 
    } 
    std = std::sqrt(std);
    return std;*/
    //if( min > 0 ) 
   // cout << "min: " << min << endl;
    //return avg /  (v.QL.size() * v.pL.size());
    return min; 

}
long double k_norm(const Node &v, const CLCS_inst *inst, int p = 0)
{
	long double k_norm = 0;
        int val = 0; 
        for(int i = 0; i < inst->P.size(); ++i) 
            if(inst->P[ i ].size() - v.QL[ i ] + 1 > val) 
               val = inst->P[ i ].size() - v.QL[ i ] + 1; 
            
	for (int i = 0; i < inst->m; i++) {
		k_norm += pow( (long double) ( inst->S[ i ].size() - v.pL[ i ] + 1 ) / ( val ), 0.5);
	}
	return k_norm;
}
}
