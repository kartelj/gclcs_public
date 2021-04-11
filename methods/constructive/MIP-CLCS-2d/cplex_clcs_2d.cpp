/* the CPLEX modes for a specific 2-LCPS problem */
/* before compilation makes sure that CPLEX software path is correct */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Timer.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <vector>
#include <set>
#include <limits>
#include <utility>  
#include <unordered_map>
#include <unordered_set>
#include <map>

// the following "include" is necessary for the correct working/compilation of CPLEX. You should adapt this path to your installation of CPLEX
#include "/home/djukanovic/Desktop/projects/LCAPS_software/cplex-12.5/include/ilcplex/ilocplex.h"

ILOSTLBEGIN

// global variables concerning the random number generator
time_t t;

// vector for keeping the names of the input files
vector<string> inputFile;
vector<int> s1; vector<int> s2; vector<int> P;
int sigma = 0;
int mzn=0; //CP-instances (conversion to the .mzn format)
// time limit for CPLEX (can be changed at run time via the -t comand line parameter)
double time_limit = 900.0;
string outdir="/home/djukanovic/Desktop/projects/rflcsp/MIP-RFLCS-2d/CP-instances/"; // the directory where .out files will be saved

// structures for keys of Hash-mapwhich will store binary variables of the 2-LCPS model

struct Point2d // point for structure in 2D-LCPS
{

   public:
   int a,b;
   Point2d(): a(0), b(0){}; // default constructor...
   Point2d(int _a,int _b): a(_a), b(_b){};

   bool operator == (const Point2d & p2) const
   {
        return (a == p2.a and b == p2.b);
   }
   friend std::ostream&  operator << (std::ostream & os, const Point2d&  p)
   {
       os<< "(" << p.a << ", " << p.b << ") " << endl;
	   return os;
   }

};

class Hash2d  //hashing the point
{

// the hash-key function works for strings up to the lenght of 1000 
// wirks if the length of strings <= 5000
public:
	std::size_t operator()(const Point2d & pl ) const
	{
		return 5000*pl.a + + pl.b;  //(a,b,c,d), 1000 numeral system...
	}
};


class Hash2dInt  //hashing the point
{

// the hash-key function works for strings up to the lenght of 1000 
// wirks if the length of strings <= 5000
public:
	std::size_t operator()(const pair<Point2d, int> &pl ) const
	{
		return 5000* 5000 * pl.first.a + + 5000 * pl.first.b + pl.second;  //(a,b,c,d), 1000 numeral system...
	}
};

inline int stoi(string &s) {

  return atoi(s.c_str());
}

inline double stof(string &s) {

  return atof(s.c_str());
}

void read_parameters(int argc, char **argv) {

    int iarg = 1;

    while (iarg < argc) {
        if (strcmp(argv[iarg],"-i")==0) inputFile.push_back(argv[++iarg]);
        else if (strcmp(argv[iarg],"-t")==0) time_limit = atof(argv[++iarg]);
        else if (strcmp(argv[iarg],"-mzn")==0) mzn = atof(argv[++iarg]);
        iarg++;
    }
}

// write during the CPLEX call
ILOMIPINFOCALLBACK6(loggingCallback,
                    Timer&, timer,
                    vector<double>&, times,
                    vector<double>&, results,
                    vector<double>&, gaps,
                    int, iter,
                    IloNum,         lastIncumbent) {

    IloNum nv = getIncumbentObjValue();
    double newTime = timer.elapsed_time(Timer::VIRTUAL);
    double newGap = 100.0*getMIPRelativeGap();
    if (nv < lastIncumbent) {
        cout << "value " << nv << "\ttime " << newTime <<  "\tgap " << newGap << endl;
        results[iter] = nv;
        times[iter] = newTime;
        gaps[iter] = newGap;
    }
    lastIncumbent = nv;
}

bool conflict(const Point2d& p1, const Point2d& p2) 
{
     if( ( p1.a > p2.a and  p1.b > p2.b ) or (   p1.a < p2.a and  p1.b < p2.b ) )// not-in-conflict
	   return false;
     return true;
}


// class for hash function 
struct MyHashFunction {  // work only for n <= 1000 (has to be adapted when n larger...
    // id is returned as hash function 
    size_t operator()(const pair<Point2d, Point2d> &t) const
    { 
        return  1000 * 1000 * 1000 * t.first.a + 1000 * 1000 * t.first.b +   1000 * t.second.a + t.second.b; // return (std::hash<int>()(p.first) + std::hash<int>()(p.second)); 
    } 
};

void run_cplex_rflcs(Timer& timer, ofstream& myfileOut) {

  IloEnv env;
  env.setOut(env.getNullStream());
  try
  {// define a model...

   IloModel model(env);
   int vars_num = 0;
   IloObjective obj = IloMaximize(env);
   // defining the set of binary variables Z
   unordered_map<Point2d, IloNumVar, Hash2d> Z;
   unordered_set<std::pair<Point2d, Point2d>, MyHashFunction> conflict_relation;   
   unordered_set<Point2d, Hash2d> matchings;

   for (int i = 0; i < s1.size(); ++i){
       for (int j = 0; j < s2.size(); ++j){
            if(s1[i] == s2[j] ){ // 2d-matching...
               Point2d p(i, j);
               matchings.insert(p);
               IloNumVar myIntVar(env, 0, 1, ILOINT);
               Z[p] = myIntVar;
               obj.setLinearCoef(Z[p], 1.0); // define objective function
               ++vars_num;
            }
       }
   }
   // define a set of constraints:
   int constraints_num = 0;
   cout << "Define conflict relations" << endl;
   for(unordered_map<Point2d, IloNumVar, Hash2d >::iterator it1 = Z.begin(); it1 != Z.end(); ++it1){
       for(unordered_map<Point2d, IloNumVar, Hash2d >::iterator it2 = Z.begin(); it2 != Z.end(); ++it2){
           if(it1 != it2)
           {
              if(conflict( (*it1).first, (*it2).first ) ){// and  ! ((*it1).first == (*it2).first) conflict constraints
                 model.add( (*it1).second + (*it2).second <= 1);
                 constraints_num++;
                 conflict_relation.insert(make_pair((*it1).first, (*it2).first)); // conflict pairs 
              }
           }
       }
   }
   // define w variables: w_{i,j,k} first
   cout << "define w variables: w_{i,j,k} first " << endl;
   unordered_map<pair<Point2d, int>, IloNumVar, Hash2dInt> W;
   for(unordered_set<Point2d, Hash2d>::iterator it1 = matchings.begin(); it1 != matchings.end(); ++it1){
       for(int px = 0; px < P.size(); ++px){
           if(s1[ (*it1).a ] == P[ px ]) // strong match possibly
           {
              IloNumVar w_ijk(env, 0, 1, ILOINT); // binary variable
              W.insert({make_pair( *it1, px) , w_ijk });
              vars_num++;
           }
       }
   }
   // conflict between W-variables constraints:
   cout << "conflict between W-variables constraints: " << endl;
   for(unordered_map<std::pair<Point2d, int>, IloNumVar, Hash2dInt>::iterator it1 = W.begin(); it1 != W.end(); ++it1){
       for(unordered_map<std::pair<Point2d, int>, IloNumVar, Hash2dInt>::iterator it2 = W.begin(); it2 != W.end(); ++it2){
           if (it1 != it2){
               //w-variables in conflict
               if ((*it1).first.second == (*it2).first.second ){
                   model.add((*it1).second + (*it2).second <= 1);
                   constraints_num++;
               }
               else
               if(conflict_relation.find(std::make_pair( (*it1).first.first, (*it2).first.first) ) != conflict_relation.end() ){ // weak matching in conflict
                  model.add( (*it1).second + (*it2).second <= 1);
                  constraints_num++;
               }
               else{ // not in conflict (x, y)
                    if ((*it1).first.first.a > (*it2).first.first.a){
                       if((*it1).first.second <= (*it2).first.second) {// conflict 
                          model.add((*it1).second + (*it2).second <= 1);
                          constraints_num++;
                       }
                    }
                    else if (((*it1).first).first.a < ((*it2).first).first.a){
                             if((*it1).first.second >= (*it2).first.second){
                                 model.add((*it1).second + (*it2).second <= 1);
                                 constraints_num++;
                             }                                  
                    }
                    else{ // (*it1).first.first.a > (*it2).first.first.a
                         model.add((*it1).second + (*it2).second <= 1);
                         constraints_num++;
                    }
               }
           }   
       }        
   }
   // sum of values of variables in W == |P|
   cout << "sum of values of variables in W == |P|" << endl;
   IloExpr expr_p(env);
   for(unordered_map<std::pair<Point2d, int>, IloNumVar, Hash2dInt>::iterator it1 = W.begin(); it1 != W.end(); ++it1)
       expr_p += ((*it1).second);

   int p_len = (int) P.size();
   model.add(expr_p == p_len );
   constraints_num++;

   // connection between W and Z variables:
   cout << "wijk <= z_ij" << endl;
   for(unordered_map<std::pair<Point2d, int>, IloNumVar, Hash2dInt>::iterator it1 = W.begin(); it1 != W.end(); ++it1){
       IloNumVar z_ij = Z[ (*it1).first.first ];
       model.add((*it1).second <= z_ij);
       constraints_num++;
   }
   cout << "Vars: " << vars_num << endl;
   cout << "constraints_num: " <<   constraints_num << endl;
   cout << "solve-->" << endl;
   // solve the modelIloLinearNumExpr objective = cplex.linearNumExpr();
   model.add(obj);
   IloCplex cplex(model);
   //int time_limit = 900;
   // pass the time limit to CPLEX
   cplex.setParam(IloCplex::TiLim, time_limit);

   // the following two parameters should always be set in the way as shown
   cplex.setParam(IloCplex::NodeFileInd, 2);
   cplex.setParam(IloCplex::Threads, 1);

   IloNum lastObjVal = std::numeric_limits<double>::max();
   // tell CPLEX to make use of the function 'loggingCallback' for writing out information to the screen
   //cplex.use(loggingCallback(env, timer,/* times, results, gaps, iter,*/ lastObjVal));
   //W.clear();// Z.clear(); 
   conflict_relation.clear();
   cplex.solve();
  
   if (cplex.getStatus() == IloAlgorithm::Optimal or cplex.getStatus() == IloAlgorithm::Feasible)
   {
       if(cplex.getStatus() == IloAlgorithm::Optimal)
          cout << "CPLEX finds optimal" << endl;
       else
          cout << "CPLEX finds feasible solution" << endl;

       double lastVal = double(cplex.getObjValue());
       cout << "optimal: " << lastVal << endl;
       // print the objective point
       cout << "nodes/vertices in the solution: {" <<endl;
       bool first = true;
       for(unordered_map<Point2d, IloNumVar, Hash2d >::iterator it = Z.begin(); it != (Z.end()); ++it){
            IloNum xval = cplex.getValue((*it).second);
            // the reason for 'xval > 0.9' instead of 'xval == 1.0' will be explained in class
            if (xval > 0.9) {
                  cout << (*it).first;
                   myfileOut << (*it).first;
            }
       }
       cout << "}" << endl;
       cout << "value: " << lastVal << endl;
       cout << "dual bound: " << double(cplex.getBestObjValue()) << endl;
       // read into a file
       myfileOut << "value: " << lastVal << endl;
       myfileOut << "dual bound: " << double(cplex.getBestObjValue()) << endl;
       //save (export) LP model:
       //std::string file_name(inputFile[0].c_str());
   }
}
catch(IloException& e) {
        cerr  << " ERROR: " << e << endl;
}
env.end();
}


/**********
Main function
**********/


template<typename T>
void print_vector(vector<T>& vec)
{
     for(const T& c: vec)
         cout << c <<" ";
     cout << endl;  
}

int main( int argc, char **argv ) {
        cout << " Reading parameters " << endl;
        read_parameters(argc,argv);
    
        //setting the output format for doubles to 2 decimals after the comma
        std::cout << std::setprecision(2) << std::fixed;
        Timer timer;
        double cur_time = timer.elapsed_time(Timer::VIRTUAL);
        int iter, line_number;
        iter = line_number = 1;
        ifstream input(inputFile[0].c_str());
        string str1;  string str2; string Px; 
        int m = 2;// input strings
        int P_size = 0;
        //P.push_back(0); P.push_back(0); P.push_back(1);   P.push_back(1); P.push_back(2);   P.push_back(2); // arbitrary P string
        string line; int n1, n2;
        // opening the corresponding input file and reading the problem data


        ifstream FIC;
        FILE *fp;
        FIC.open(inputFile[0].c_str());
        if (FIC.fail() )
        {
            cout << "### Erreur open, File_Name " << inputFile[0].c_str() << endl;
            getchar();
            exit(0);
        }
        char StrReading[100];
        //Max_Vclique=300;
        // FIC >> StrReading;
        if (FIC.eof() )
        {
            cout << "### Error open, File_Name " << inputFile[0].c_str() << endl;
            exit(0);
        }
        FIC >> m >> sigma >> P_size;
        FIC >> P_size >> Px;
        FIC >> n1 >> str1;
        FIC >> n2 >> str2;
        FIC.close();
        // end of reading from file...
         cout << "End of input " << inputFile[0].c_str() << ".out" << endl;
         //cout << str1 << endl;
         //cout << str2 << endl;
         //cout << Px << endl;
         // transformation from char to numbers
         s1.reserve(s1.size()); s2.reserve(s2.size()); P.reserve(Px.size());
         map<char,int> charLetter; int letter_size = 0;
         int next_letter = 0;
         for(char c: str1)
         {    
             if(charLetter.find(c) != charLetter.end())
                s1.push_back(charLetter[ c ]);
             else{
                charLetter[c] = letter_size;
                s1.push_back(charLetter[ c ]);
                letter_size++;
             }
             next_letter++;
         }
         //second string:
         next_letter = 0;
         for(char c: str2)
         {
             if(charLetter.find(c) != charLetter.end())
                s2.push_back(charLetter[ c ]);
             else{
                charLetter.insert({c, letter_size});
                s2.push_back(charLetter[ c ]);
                letter_size++;
             }
             next_letter++;
         } 
         // P-constraint:
         next_letter = 0;
         for(char c: Px)
         {
             if(charLetter.find(c) != charLetter.end())
                P.push_back(charLetter[ c ]);
             else{
                charLetter.insert({c, letter_size});
                P.push_back(charLetter[ c ]);
                letter_size++;
             }
             next_letter++;
         } 
         for (int i = 0; i < P.size(); ++i)
             cout << P[i] << " "; 
         cout << endl;
         for (int i = 0; i < s1.size(); ++i)
             cout << s1[i] << " "; 
         // write the results in the output...
         std::stringstream filenameOut;
         filenameOut << outdir << inputFile[0].c_str() << ".out";
         std::string filenameO = filenameOut.str();
         ofstream myfileOut (filenameO);        
         // set up timer and run CPLEX
         cout << "Runing CPLEX..." << endl;
         run_cplex_rflcs(timer, myfileOut);
         double end_time = timer.elapsed_time(Timer::VIRTUAL);
         cout << "time: " << (end_time - cur_time ) <<"\n";
         myfileOut << "time: " << (end_time - cur_time ) <<"\n";
         myfileOut.close();
}



