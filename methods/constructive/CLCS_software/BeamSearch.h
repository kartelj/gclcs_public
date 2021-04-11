#ifndef SRC_BEAMSEARCH_
#define SRC_BEAMSEARCH_H

#include "CLCS_solution.h"
#include "CLCS_evaluation.h"

namespace clcs {

/** mhlib parameter for beam width **/
extern mh::int_param bw;
/** mhlib parameter for k_best used in filtering **/
extern mh::int_param kbest;
/** mhlib parameter for controlling pre-restriction of the extension set V_ext **/
extern mh::int_param kext;
/** mhlib parameter for choosing a guiding function for the search **/
extern mh::int_param guidance;
/** mhlib parameter for enabling / disabling filter procedure **/
extern mh::int_param filtering; 

/** mhlib parameter for enabling / disabling filter procedure **/
extern mh::int_param turn_cutoff; 

/** structure to store nodes in the beam and enable easy ordering by their respective heuristic values */
struct NodeExtension {

	shared_ptr<Node> node;
	long double h; // stored value from evaluating node with heuristic
	long double h2 = 0.0; // second criterion for tie breaking
        long double cut_off = 0.0;


	bool operator < (const NodeExtension &other) const {
		if (h == other.h) {
		    return h2 < other.h2;
		}
		return h < other.h;
	}
	bool operator> (const NodeExtension &other) const {
		if (h == other.h) {
		    return h2 > other.h2;
		}
		return h > other.h;
	}
};

/** The Hash function for nodes (necessary for utilizing hash-map structure N of an A* search */
struct hash_node {

	std::size_t operator()(const std::pair<std::vector<int16_t>, std::vector<int16_t>> &pl_ql) const
	{   
                vector<int16_t> pL = pl_ql.first; 
                vector<int16_t> QL = pl_ql.second; 
		
                std::size_t seed = pL.size() + QL.size();
		for (auto &i : pL) {
		     seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
	        for (auto &i : QL) {
		     seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};

/** possible choices for applied heuristic **/
enum class Heuristic { UB, H_PROB, H_POW, EX, EX_PIECE, MIN_AVG };

class BeamSearch {
public:
	/** beam width */
	int k_bw = bw();
	/** parameter to control the amount of extensions (see max_extensions) */
	double k_ext = kext();
	/** parameter to control, to how many of the best nodes the domination relation should be checked */
	int k_best = kbest();
	/** maximum number of nodes in one extension step (k_bw * k_ext) */
	int max_extensions;
	/** upper bound to use */
	clcs_eval::upperBound ub = clcs_eval::ub;  // tight upper bound known from LCS problem
	/** heuristic to use */
	clcs_eval::heuristic h;
	/** tie-breaker heuristic to use (if any) */
	clcs_eval::heuristic h2 = clcs_eval::h_prob;// nullptr;
        
        std::unordered_map<std::pair<std::vector<int16_t>, std::vector<int16_t> >, bool , hash_node> N{}; // container of nodes at each level: does not allow adding duplicate elements...
	/** the constructor for BeamSearch */
	explicit BeamSearch(CLCS_solution *sol) : sol(sol), inst(sol->inst)
	{      
                Embed = std::vector<std::vector<int>>(sol->inst->embed_end.size(), std::vector<int>()); 
 
                for(int i = 0; i < (int) sol->inst->embed_end.size(); ++i) 
                {    
                    std::vector<int> s(sol->inst->embed_end[ i ].size(), 0); 
                    Embed[i] = s; int j = 0;
                    for(int val: sol->inst->embed_end[ i ]) 
                    {
                        Embed[ i ][ j ] = val; ++j;
                    }
                }
                max_extensions = std::floor(k_bw * k_ext);
                // choose guiding function 
		if (guidance() == (int) Heuristic::H_PROB)
			h = clcs_eval::h_prob;
		else if (guidance() == 2)
			h = clcs_eval::penality;
		else if (guidance() == 3) 
			h = clcs_eval::probability;
		/*else if (guidance() == (int) Heuristic::EX)
			h = clcs_eval::ex;
		else if (guidance() == (int) Heuristic::MIN_AVG) {
			h = clcs_eval::h_min_avg;
			h2 = clcs_eval::k_norm;
		}*/
                h2 = clcs_eval::k_norm;//probability;   //k_norm; // tie-breaking
 
	}
	/** perform beam search */
	void startSearch();
	/** perform greedy heuristic */
	void startGreedy();
	/** return the solution vector of the best found solution */
	void  getSolution();

public:
        long double evaluate_cutoff( const Node &v );
	/** node representing the best found solution so far */
	shared_ptr<Node> s_best;
	/** pointer to CLCS solution */
        CLCS_solution *sol;
	/** pointer to CLCS instance */
	const CLCS_inst *inst;
        std::vector<std::vector<int>> Embed; 

	/** create extension set sorted by a heuristic function */
	vector<NodeExtension> extendAndEvaluate(const vector<shared_ptr<Node>>& beam);
	/** function required for H_prob: determine p-value **/
	int determine_p(vector<NodeExtension> &extensions);
	/** removes nodes whose upper bound is equal or lower than the length of the best found solution so far */
	void prune(vector<NodeExtension> &extensions);
	/** removes nodes dominated by other nodes (limited by k_best) */
	void filter(vector<NodeExtension> &extensions);
	/** determine if node v is dominated by one of the first k_best nodes in extensions */
	bool isDominated(Node* v,  vector<NodeExtension>& extensions);
	/** dynamic removal of the dominated nodes from each V_ext set **/
	bool dominated_member(int index, vector<NodeExtension>& C, int k_fi);
        void update_embed_struct(const Node& v ); //std::map<int, std::vector<int16_t>> &  temp);
        void checkAndAdd(vector<shared_ptr<Node>>& beam, std::shared_ptr<clcs::Node> & v); // add or not v into beam?
        void print_embed();
        bool complete(const std::shared_ptr<clcs::Node>& v);
        bool  feasNode(shared_ptr<Node> child );
        bool  branchOrNot( const Node& v); 
};

}

#endif //SRC_BEAMSEARCH_H
