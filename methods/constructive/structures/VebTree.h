#ifndef SRC_VEBTREE_H
#define SRC_VEBTREE_H

#include <cmath>
#include <memory>
#include <vector>

namespace clcs {

// Van Emde Boas Tree
class VEBTree {
	/** number of keys (universe size) */
	int u;
	/** child size (= ceil(sqrt(u))) */
	int childSize;
	/** minimum value stored in tree */
	int min = NO_KEY;
	/** maximum value stored in tree */
	int max = NO_KEY;
	/** summary */
	std::unique_ptr<VEBTree> summary;
	/** clusters */
	std::vector<std::unique_ptr<VEBTree>> clusters; 
	/** calculates the key based on position and number of keys */
	int retrieveKey(int cluster, int position);
public:
	/** constants to signal that key was not found / no value is present */
	static const int NO_KEY = -1;
	/** copy constructor */
	VEBTree(const VEBTree &copy) : u(copy.u), childSize(copy.childSize), min(copy.min), max(copy.max) {
		if (copy.summary != nullptr) {
			summary = std::unique_ptr<VEBTree>(new VEBTree(*copy.summary));
		}
		clusters = std::vector<std::unique_ptr<VEBTree>>();
		for (auto &cluster : copy.clusters) {
			if (cluster == nullptr)
			    clusters.emplace_back(nullptr);
			else
			    clusters.emplace_back(std::unique_ptr<VEBTree>(new VEBTree(*cluster)));
		}
	}
	/** constructor to initialize VEBTree of size @size */
	explicit VEBTree(int size) : u(size), childSize(ceil(sqrt(size))) {
		if (size <= 2) {
			summary = nullptr;
			clusters = std::vector<std::unique_ptr<VEBTree>>(size);
		} else {
			summary = std::unique_ptr<VEBTree>(new VEBTree(childSize));
			clusters = std::vector<std::unique_ptr<VEBTree>>(childSize);
		}
	}
	/** get index of responsible cluster */
	int high(int key);
	/** get index of key in cluster (where key is stored) */
	int low(int key);
	/** insert key into tree */
	void insert(int key);
	/** check if key exists in tree */
	bool isMember(int key);
	/** delete a key from the tree (if present) */
	void remove(int key);
	/** find predecessor for a key */
	int findPred(int key);
	/** find successor for a key */
	int findSucc(int key);
	/** get the highest key */
	int findMax();
	/** get the lowest key */
	int findMin();
	/** get the next number after x that is a power of 2 */
	static int ceilToP2 (int x);
};

}

#endif //SRC_VEBTREstatic E_H
