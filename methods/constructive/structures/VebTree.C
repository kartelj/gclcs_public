
#include "VebTree.h"

namespace clcs {

/** calculates the key based on its position and number of keys */
int VEBTree::retrieveKey(int cluster, int position)
{
	return cluster * childSize + position;
}

/** get index of responsible cluster to store @key */
int VEBTree::high(int key)
{
	return key / childSize;
}

/** get index of @key in cluster (where key is stored) */
int VEBTree::low(int key)
{
	return key % childSize;
}

/** insert @key into tree */
void VEBTree::insert(int key)
{
	if (key == min) {
		return; // key is already there
	}
	if (min == NO_KEY) {
		// it's the first key in the tree
		min = key;
		max = key;
	} else {
		if (key < min) {
		    std::swap(min, key);
		}
		if (u > 2) {
			// find corresponding cluster to insert tree
		    int h_index = high(key);
		    int l_key = low(key);
		    if (clusters[h_index] == nullptr) {
			clusters[h_index] = std::unique_ptr<VEBTree>(new VEBTree(childSize));
		    }
		    if (clusters[h_index]->findMin() == NO_KEY) {
			summary->insert(h_index);
			clusters[h_index]->min = l_key;
			clusters[h_index]->max = l_key;
		    } else {
			clusters[h_index]->insert(l_key);
		    }
		}
		if (key > max) {
			max = key;
		}
	}
}

/** check if @key exists in tree */
bool VEBTree::isMember(int key)
{
	if (u < key) {
		return false;
	}
	if (min == key || max == key) {
		return true;
	} else {
		if (u == 2) {
		   return false;
		} else {
		   return clusters[high(key)] != nullptr && clusters[high(key)]->isMember(low(key));
		}
	}
}

/** delete @key from the tree (if present) */
void VEBTree::remove(int key)
{
	if (max == min) {
		// it's the only key in the tree - just reset min and max
		min = NO_KEY;
		max = NO_KEY;
	} else if (u == 2) {
		// base case
		if (key == 0) {
			min = 1;
		} else {
			min = 0;
		}
		max = min;
	} else {
		// delegate remove operation to corresponding cluster and update structures accordingly
		if (key == min) {
			int min_cluster = summary->findMin();
			key = retrieveKey(min_cluster, clusters[min_cluster]->findMin());
			min = key;
		}
		int h_index = high(key);
		clusters[h_index]->remove(low(key));
		if (clusters[h_index]->findMin() == NO_KEY) {
			summary->remove(h_index);
			if (key == max) {
				int max_in_sum = summary->findMax();
				if (max_in_sum == NO_KEY) {
					max = min;
				} else {
					max = retrieveKey(max_in_sum, clusters[max_in_sum]->findMax());
				}
			}
		} else if (key == max) {
			max = retrieveKey(h_index, clusters[h_index]->findMax());
		}
	}
}

/** return maximum of tree */
int VEBTree::findMax()
{
	return max;
}

/** return minimum of tree */
int VEBTree::findMin()
{
	return min;
}

/** find predecessor for a given @key */
int VEBTree::findPred(int key)
{
	if (u == 2) {
		// base case
		if (key == 1 && min == 0) {
			return 0;
		} else {
			return NO_KEY;
		}
	} else if (max != NO_KEY && key > max) {
		return max;
	} else {
		int h_index = high(key);
		int min_in_cluster = NO_KEY;
		if (clusters[h_index] != nullptr) {
			min_in_cluster = clusters[h_index]->findMin();
		}
		if (min_in_cluster != NO_KEY && low(key) > min_in_cluster) {
			// predecessor is in same cluster as key
			int offset = clusters[h_index]->findPred(low(key));
			return retrieveKey(h_index, offset);
		} else {
			// predecessor is not in same cluster as key
			int pred_cluster = summary->findPred(h_index);
			if (pred_cluster == NO_KEY) {
				if (min != NO_KEY && key > min) {
					return min;
				} else {
					return NO_KEY;
				}
			} else {
				int offset = clusters[pred_cluster]->findMax();
				return retrieveKey(pred_cluster, offset);
			}
		}
	}
}

/** find successor for a given @key */
int VEBTree::findSucc(int key)
{
	if (u == 2) {
		// base case
		if (key == 0 && max == 1) {
			return 1;
		} else {
			return NO_KEY;
		}
	} else if (min != NO_KEY && key < min) {
		return min;
	} else {
		int h_index = high(key);
		int max_in_cluster = NO_KEY;
		if (clusters[h_index] != nullptr) {
			max_in_cluster = clusters[h_index]->findMax();
		}
		if (max_in_cluster != NO_KEY && low(key) < max_in_cluster) {
			// successor is in same cluster as key
			int offset = clusters[h_index]->findSucc(low(key));
			return retrieveKey(h_index, offset);
		} else {
			// successor is not in same cluster as key
			int succ_cluster = summary->findSucc(h_index);
			if (succ_cluster == NO_KEY) {
				return NO_KEY;
			} else {
				int offset = clusters[succ_cluster]->findMin();
				return retrieveKey(succ_cluster, offset);
			}
		}
	}
}

/** get the next number after x that is a power of 2 */
int VEBTree::ceilToP2(int x)
{
	int p4 = 4;
	int k = 2;
	while (x > p4) {
		p4 = 1 << (k++);
	}
	return p4;
}

}
