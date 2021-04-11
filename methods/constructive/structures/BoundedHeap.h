#include <utility>
#include <vector>
#include <tuple>
#include "../CLCS_solution.h"
#include "VebTree.h"

#ifndef SRC_BOUNDEDHEAP_H
#define SRC_BOUNDEDHEAP_H

namespace clcs {

class BoundedHeap {
	/** max item count for heap */
	int size;
	/** structure for the additional data */
	std::vector<IR_Item> store;
	/** search structure for keys */
	VEBTree vEBTree;
	/** insert val at pos and store data */
	void insert(int pos, int val, std::tuple<int, int, int> data);
public:
	/** increase val at pos (if higher) and store data */
	void increase(int pos, int val, std::tuple<int, int, int> data);
	/** get the item with max value at a position smaller than pos */
	IR_Item getMax(int pos);
	/** copy constructor */
	BoundedHeap(const BoundedHeap &copy) : size(copy.size), store(copy.store), vEBTree(VEBTree(copy.vEBTree)) {};
	/** construct bounded heap for specific size */
	explicit BoundedHeap(int size) : size(size), vEBTree(VEBTree(VEBTree::ceilToP2(size))) {
		 store = std::vector<IR_Item>(size, IR_Item());
	}
};

}

#endif //SRC_BOUNDEDHEAP_H
