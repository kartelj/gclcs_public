#ifndef SRC_ASTAR_H
#define SRC_ASTAR_H


#include <cstdint>
#include <utility>
#include <vector>
#include <list>
#include "CLCS_solution.h"
#include "CLCS_evaluation.h"

namespace clcs {

/**
 * Structure @QNode used for storing not yet expanded nodes in priority queue.
 * The ranking of the nodes is determined through the implementation of the comparison operator "<".
 */
struct QNode {

	shared_ptr<Node> node;
	int16_t ub;
	bool operator < (const QNode &other) const
	{
		if (ub == other.ub) {
			if (node->l_v == other.node->l_v) // breaking ties I
			    return node->QL[ 0 ] < other.node->QL[ 0 ] ;
			else // breaking ties II
			    return node->l_v < other.node->l_v;
		} else
			return ub < other.ub;
	};

	bool operator > (const QNode &other) const {
		if (ub == other.ub) {
			if (node->l_v == other.node->l_v)
			    return std::accumulate(node->QL.begin(), node->QL.end(), 0) > std::accumulate(other.node->QL.begin(), other.node->QL.end(), 0);
			else
			    return node->l_v > other.node->l_v;
		} else
			return ub > other.ub;
	};

	explicit QNode(shared_ptr<Node> node, int16_t ub) : node(std::move(node)), ub(ub) {
               //ub += node->l_v;

	}
};

/** The Hash function for nodes (necessary for utilizing hash-map structure N of an A* search */
struct hash_node1 {

	std::size_t operator()(const std::pair<vector<int16_t>, vector<int16_t>> &pL_QL) const
	{
		std::size_t seed = pL_QL.first.size() + pL_QL.second.size();

		for (auto &i : pL_QL.first) {
		     seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		for (auto &i : pL_QL.second) {
		     seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};

class AStar {
public:
	/** perform A* search and return the solution vector of an optimal solution (if found) */
	void startSearch();
	/** print statistics of the search */
	void printStatistics();
	/** constructor for AStar */
	explicit AStar( CLCS_solution *sol) : sol(sol), inst(sol->inst) {
	}
	/** upper bound to use */
	clcs_eval::upperBound ub = clcs_eval::ub;

private:
	/** pointer to CLCS solution */
        CLCS_solution *sol;
	/** pointer to CLCS instance */
	const CLCS_inst *inst;
        /** keep opt node at s_best **/
	shared_ptr<Node> s_best;
	/** hash-map storing all encountered nodes */
	std::unordered_map< std::pair< std::vector<int16_t>, std::vector<int16_t>>, int , hash_node1 > N{}; // pair => longest path...
	/** priority queue storing not yet expanded nodes */
	std::priority_queue<QNode> Q;
	/** some variables for statistics */
	long stat_nodes_created = 0;
	long stat_nodes_expanded = 0;
	long stat_nodes_ignored = 0;
	long stat_nodes_merged = 0;
	unsigned int stat_max_nested = 0;

	/** helper function to add a node to N and Q */
	void addNode(const shared_ptr<Node> &v );
	/** print node information */
	void printNode(const shared_ptr<Node> &v, bool withParent = true);

        bool complete(const std::shared_ptr<clcs::Node>& v);
        void getSolution();
};

}


#endif //SRC_ASTAR_H
