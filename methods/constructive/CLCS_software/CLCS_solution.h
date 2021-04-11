#ifndef CLCS_SOLUTION_H
#define CLCS_SOLUTION_H

#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <climits>
#include <memory>
#include <mh_util.h>
#include <mh_solution.h>
#include <mh_param.h>
#include "CLCS_inst.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;
namespace clcs {

extern mh::int_param algorithm;
extern mh::double_param time;
extern mh::int_param feasibility;
/** A Node structure definition */
struct Node {
	shared_ptr<Node> parent;
	vector<int16_t> pL;
        vector<int16_t> QL;
        vector<int16_t> additional_letters; 
	int16_t l_v = 0;

	Node(vector<int16_t> pL1, vector<int16_t> QL1) { //cout << "construct" << endl;
            pL = vector<int16_t>(pL1.size(), 0); QL = vector<int16_t>(QL1.size(), 0);
            int index = 0; 
            for(int i: pL1)
            {  
                pL[ index ] = i; 
                ++index;
            }
         index = 0;  
         for(int i: QL1)
         {
             QL[ index ] = i; 
             index++;
         }
        // cout << "end of constr" << endl;
	}
         Node() {
	}
        void print()
        {
            cout << "Node printing... l_v: " << l_v << " " << pL.size() << endl;   
            cout << "pL: ";
            for (int i = 0; i < (int) pL.size(); ++i) 
                 cout << pL[ i ] << " ";
   cout << "QL: ";
            for (int i = 0; i < (int) QL.size(); ++i) 
                 cout << QL[ i ] << " ";
           cout << endl;
        }
	~Node() {
		pL.clear();
	}
};


/** Solution class for CLCS problem  */

class CLCS_solution : public mh::mh_solution {

public:
        const CLCS_inst *inst; ///< A pointer to the respective problem instance.
	std::vector<int16_t> s;///< CLCS solution vector s (store solution)
        std::map<int, std::map< int, std::vector<int16_t>>> temp;
	int UB;  // proven upper bound
	/** statistics **/
	int iter = 0; // follow the number of BS or ACS iterations
	double timeStart = 0.0;
	double tbest = 0;  ///< time when best solution was found
	int iter_best = 0; ///< number of iteration when best solution found (not used here, planed for the future anytime search variants)
	double gap = 100.0;///< needed for the exact search 

	/** Constructor creating an empty solution. */
	CLCS_solution(const CLCS_inst *inst) : mh::mh_solution(1, ""), inst(inst), UB(inst->UB) {
		s.reserve(inst->UB);
	}
	/** Constructor for creating a partial solution from the given array and middle letter information. */
	CLCS_solution( CLCS_inst *inst, std::vector<int16_t> &s, int UB)
			: mh::mh_solution(1, ""), inst(inst), s(s), UB(UB) {
		/** Dynamically cast a const mh::mh_solution reference to a reference of this class. */
	}
	/** Copy constructor. */
	CLCS_solution(const CLCS_solution &sol) : mh::mh_solution(1, ""), inst(sol.inst),s(sol.s), UB(sol.UB)  {
	}
	/** Destructor. */
	virtual ~CLCS_solution() {
	}
	/** Creates an uninitialized/empty solution. */

	mh::mh_solution *createUninitialized() const override {
		return new CLCS_solution(inst);
	}

	/** Clone this solution. */
	//mh::mh_solution *clone()  override { return new CLCS_solution(*this); }

	/** Dynamically cast an mh::mh_solution reference to a reference of this class. */
	static CLCS_solution &cast(mh::mh_solution &ref) {
		return (dynamic_cast<CLCS_solution &>(ref));
	}

	static const CLCS_solution &cast(const mh::mh_solution &ref) {
		return (dynamic_cast<const CLCS_solution &>(ref));
	}

	/** Copy a solution. */
	void copy(const mh_solution &sol) override;

	/** Clears the solution. */
	void clear() {
		UB = inst->UB;
		s.clear();
		invalidate(); // invalidate --> empty solution
	}

	/** Returns the complete solution as a string. */
	std::string getSolution();

	/** Initializes the solution with the given string LCS. */
	void decodeSolution(const std::string &LCS);

	/** The objective function corresponds to the length of the solution. */
	double objective() override {
		return s.size();
	}

        void setS( vector<int16_t>& sx)
        {
              s.reserve(sx.size());
              int ind = 0;
              for(int16_t val: sx )
                 s[ind++] = val;
        }

	CLCS_solution &operator=(const CLCS_solution &solution);

	/** Writes out the solution in human readable form. */
	virtual void write(std::ostream &os, int detailed = 0);

	/** Saves a solution to a file. Does nothing if fname==NULL. */
	void save(const std::string &fname) override;

	/** Loads a solution from a file. */
	void load(const std::string &fname) override;


	/** BS procedure for the CLCS */ 
	void BS();
	/** Greedy procedure for the CLCS */
	void Greedy();
        void A_Star();
	/** check if the algorithm has to b terminated due to exceeding time limitation */
	bool terminate();
	/** Checking the feasibility of solution */
	void checkFeasibility();
        /** temp structure **/
        void update_embed_column_pix( int letter, int pix, int p_u) ;
        /** check if embed is satisifed **/
        bool feas_embed_check(int position_next, int i, vector<int16_t>& lambda );
	/** Get all feasible letters for a certain node; update embed, columns which needs to be updated, are stored in temp*/
	map<int, pair<vector<int16_t>, vector<int16_t>>> findFeasibleSigma(const Node &v, std::vector<std::vector<int>> Embed );
	/** Get all feasible, non-dominated letters for a certain node */
	map<int, pair<vector<int16_t>, vector<int16_t>>> findSigmaNd(const Node &v, std::vector<std::vector<int>> Embed);
	/** Check if position vector pL_1 is dominated by position vector pL_2 */
	bool check_domination( vector<int16_t> &pL_1, vector<int16_t> & QL_1,  vector<int16_t> &pL_2, vector<int16_t> &QL_2) const;
        bool feasNode(vector<int16_t> pL, vector<int16_t> QL);
	/** return a new child by extending parent node by one letter */
	shared_ptr<Node> expandNode(const shared_ptr<Node> &parent, int letter, vector<int16_t> &pL, vector<int16_t> &pQ) const;
        bool feas(int letter, const Node &v); 
	/** derive Solution from leaf node */
        void deriveSolution(Node *v);
        /** BS **/
        map<int, pair< vector<int16_t>,  vector<int16_t>>> findFeasibleSigmaBS(const Node &v);
        map<int, pair<vector<int16_t>, vector<int16_t>>> findSigmaNdBS(const Node &v); 
};

} // clcs namespace

#endif /* CLCS_SOLUTION_H */
