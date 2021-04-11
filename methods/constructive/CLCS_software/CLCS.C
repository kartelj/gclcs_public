#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <mh_util.h>
#include <mh_param.h>
#include <mh_random.h>
#include <mh_log.h>
#include <mh_pop.h>
#include <mh_advbase.h>
#include <new>  // std::bad_alloc
#include <assert.h> 
#include "CLCS_inst.h"
#include "CLCS_solution.h"

using namespace std;
using namespace mh;

namespace clcs {

/** \ingroup param
	File from which to read the problem instance. */
string_param ifile("ifile", "problem instance file name", "10_100_20.9");
/** \ingroup param
	Optional file from which to read an initial solution. */
string_param isfile("isfile", "name of file to load initial solution from", "");
/** \ingroup param
	Name of file to save final solution. If empty, the final solution will
	not be saved. 
*/
string_param sfile("sfile", "name of file to save final solution to", "");


} // clcs namespace

using namespace clcs;

int main(int argc, char *argv[]) {
	char *_emergencyMemory = new char[100000];
	//CLCS_solution *sol = nullptr; //init of empty solution (input in try-catch block)
	try {
		// Probably set some parameters to new default values
		maxi.setDefault(1);        // we maximize here
		popsize.setDefault(1);        // other values make no sense with the Scheduler
		titer.setDefault(-1);        // the maximum number of performed iterations

		// parse arguments and initialize random number generator
		param::parseArgs(argc, argv);
		random_seed();

		/* initialize out() stream for standard output and logstr object for logging
		   according to set parameters. */
		initOutAndLogstr();

		// write out all mhlib parameters and the mhlib version to make runs reproducable
		out() << "#--------------------------------------------------"
			  << endl;
		out() << "# ";
		for (int i = 0; i < argc; i++)
			out() << argv[i] << ' ';
		out() << endl;
		out() << "#----------------- mhlib parameters ---------------------------------"
			  << endl;
		out() << "# version: " << VERSION << endl;
		out() << "# " << mhversion() << endl;
		param::printAll(out());
		out() << endl;

		// create and load problem instance
	        const CLCS_inst *inst = new CLCS_inst(ifile()); // read problem instance object
		inst->write(out(), 1);    // write out some info on instance
	       
		//out() << endl;
		// create a solution object
	        CLCS_solution *sol = new CLCS_solution(inst);

		if (isfile() != "")
			sol->load(isfile());
		else
			sol->initialize(0);
		//apply algorithm according to parameter alg()
		switch (algorithm()) {

                       case 0: {// Execute our A* algorithm
				cout << "Run A* search algorithm: " << endl;
				sol->A_Star();
				break;
			}
			case 1: { // run Greedy procedure for m-CLCS problem (OPTIMA 2020)
                               
				 cout << "Run greedy procedure: " << endl;
                                 int greedy_val = 0;
				 std::vector<int16_t> greedy_sol;
				 sol->Greedy();
       
				break;
			}
			case 3: {
                                // run four BS configurations  
				cout << "Run Generalized Beam Search (GBS) framework:" << endl;
				int greedy_val = 0;
				std::vector<int16_t> greedy_sol;
				if (pruning() == 1) { // include Prune method in GBS framework
				    sol->Greedy();
				    greedy_val = (int) sol->s.size();
				    if(greedy_val > 0) 
				       greedy_sol.assign((sol->s).begin(), (sol->s).end());
				    cout << "Initial greeedy sol.: " << greedy_val << endl;
				}
                                cout << "Run BS " << endl; 
				sol->BS(); // run BS
				if (sol->s.size() == 0 && (int) greedy_sol.size() > 0) // init greedy sol. taken
				    sol->s.assign(greedy_sol.begin(), greedy_sol.end());//set up the solution
				break;
			}
			default:
				cout << "wrong number (no algorithm exists with that <id>) " << endl;
		}
		double timeEnd = wctime() ? mhwctime(): mhcputime();
                sol->checkFeasibility();
		//sol->write(out(), 1);
		
		//save final solution in file
	        sol->save(outStream::getFileName(".sol", "NULL"));
		delete sol;
		delete inst;
                cout << "Algorithm finished..." << endl;

	}//caatch solutions when runing out of memory
	catch (exception &e) {
		delete[] _emergencyMemory;
		out() << "mh_exception &e" << endl;
		writeErrorMessage(string("Standard exception occurred: ") + e.what());
	}
	catch (...) {
		delete[] _emergencyMemory;
		writeErrorMessage("Unknown exception occurred");
		std::exit(1);
	}
	return 0;
}
