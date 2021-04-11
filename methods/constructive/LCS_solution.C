/*
 * LCPSsolution.cpp
 *
 *  Created on: Mart, 25th, 2019
 *      Author: djukanovic
 */

#include <assert.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <mh_util.h>
#include <mh_log.h>
#include <mh_param.h>
#include "LCS_solution.h"
#include <set>
#include <new>

using namespace std;  
using namespace mh;

namespace clcs { 

int_param algorithm("algorithm", "0:BS (including HHBS); 1: A* ", 0, 0, 1);
double_param time ("time", "ttime: time-limit  ", 900,0, 10000);

void CLCS_solution:: UB_1(Node* v)
{
      //TODO
}

void CLCS_solution:: UB_2(Node * v) /* has to be adapted */
{
     //TODO
}

vector<Node*> CLCS_solution:: expansion(Node* v, int guide){

       //TODO
     
}

void CLCS_solution:: BS()
{ 
     //TODO
}
  
   
bool CLCS_solution:: terminate() {

	 if (time() >= 0 &&  time() <= ( mhwctime() - timeStart)  )
	     return true;
	 return false;
}

void CLCS_solution:: copy(const mh_solution &sol) {

	const CLCS_solution &ss = cast(sol);
	assert(inst == ss.inst);
	UB = ss.UB;
	mh::mh_solution::copy(ss);
	s = ss.s;
	invalidate();
}

string CLCS_solution:: getSolution() {
    
    string Sol;
    for (int a : s)
         Sol += inst->int2Char[a];
    if (Sol =="")
    	Sol = "-";

    return Sol;
}

void CLCS_solution:: decodeSolution(const string &LCS) {

	UB = inst->UB;
	s.clear();
	if (LCS=="-" || LCS=="")
		return; // empty solution

	for (int j = 0; j<int(LCS.size()); j++)
		s.push_back(inst->char2Int.at(LCS[j]));
	invalidate();
}

CLCS_solution& CLCS_solution:: operator = ( const CLCS_solution& solution ) 
{
        s = solution.s;
        UB = solution.UB;
        inst = solution.inst;

        return *this;   
}

void CLCS_solution:: write(std::ostream &os, int detailed) {

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

/* save solution in a file */

void CLCS_solution:: save(const std::string &fname) {

	if (fname!="NULL") {
		ofstream ostr(fname);
		if (!ostr)
		    mherror("Cannot open solution file for writing", fname);
		ostr << getSolution() << endl;
	}
}

void CLCS_solution:: load(const std::string &fname) {
	ifstream istr(fname);
	if (!istr)
            mherror("Cannot open solution file for reading", fname);
	string Px;
	istr >> Px;
	decodeSolution( Px );
}

// Returns true if str1 is a subsequence of str2

static bool is_SubSequence(const vector<int16_t>& str1, const vector<int>& str2)
{
   int j = 0;
   for (unsigned int i = 0; i < str2.size() && j < (int)str1.size() ; i++)
        if ((int) str1[j] == str2[i])
           j++;
   // If all characters of str1 were found in str2
   return (j == (int) str1.size());
}

 void CLCS_solution::checkFeasibility() {

        if(s.size() == 0)
          return;
	    assert(UB <= inst->UB);
        /** checking the feasibility of solution */
        bool feasible = true;

        for (int i = 0; i < inst->m && feasible; i++)
        {
             std::vector<int> s_i =  inst->S[i];
             if (!is_SubSequence(s, s_i)) //solution
             {
            	 feasible = false;
                 cout << i<<"-th string infeasible " << endl;
             }
        }
        assert(feasible == true);// check feasibility of the given solution

 }

} // namespace lcps


