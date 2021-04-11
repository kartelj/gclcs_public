#ifndef SRC_CLCS_EVALUATION_H
#define SRC_CLCS_EVALUATION_H

#include "CLCS_solution.h"
#include "CLCS_inst.h"

using namespace clcs;

namespace clcs_eval {

	/** type definition for upper bound functions */
	typedef int (*upperBound)(const Node &v, const CLCS_inst *inst);
	/** type definition for heuristic functions */
	typedef long double (*heuristic)(const Node &v, const CLCS_inst *inst, int p);

	/** calculate greedy criterion */
	double greedy(const Node &v, const CLCS_inst *inst);
	/** Upper bound for the LCSP **/
	int ub(const Node &v, const CLCS_inst *inst);
        long double h_prob(const Node &v, const CLCS_inst *inst, int p); 
  	long double k_norm(const Node &v, const CLCS_inst *inst, int p);
	long double penality(const Node & v, const CLCS_inst *inst, int p);
        long double  probability(const Node& v, const CLCS_inst *inst, int p); 
        long double h_prob_gen(const Node& v, const CLCS_inst *inst, int p); 

}

#endif //SRC_CLCS_EVALUATION_H
