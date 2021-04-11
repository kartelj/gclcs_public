
#include "BoundedHeap.h"

namespace clcs {

/** insert IR_item at specific @pos and @data into store (atribute) structure */
void BoundedHeap::insert(int pos, int val, std::tuple<int, int, int> data)
{
	store[pos] = IR_Item(val, std::get<0>(data), std::get<1>(data), std::get<2>(data));
	vEBTree.insert(pos);
}

/** increase @val at specific @pos (if higher than previous value), store data and remove all succeeding outdated values */
void BoundedHeap::increase(int pos, int val, std::tuple<int, int, int> data)
{
	if (val > store[pos].val) {
		insert(pos, val, data);
		int successor = pos;
		while ((successor = vEBTree.findSucc(successor)) >= 0) {
			if (store.at(successor).val < val) {
			    vEBTree.remove(successor); // remove outdated values
			}
		}
	}
}

/** get item of maximum value at a position smaller than @pos */
IR_Item BoundedHeap::getMax(int pos)
{
	int maxPos = vEBTree.findPred(pos);
	if (maxPos >= 0)
	    return store.at(maxPos);
	else
	    return IR_Item();
}

}
