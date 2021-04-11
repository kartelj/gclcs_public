#include <string>
#include <map>
#include <vector>
#include <assert.h> 
#include <bits/stdc++.h>
#include <iostream>
#include <assert.h>
#include <unordered_map>
#include <stdio.h>
#include <fstream>      // std::ifstream
#ifndef CLCS_INST_H
#define CLCS_INST_H

namespace clcs {

/** A class representing a Constrained Longest Common Subsequence (CLCS) problem instance. */
extern mh::int_param alg;
/** which upper bound to use **/
extern mh::int_param ub; 
/** prune method **/
extern mh::int_param pruning;
/** prune method **/
extern mh::string_param ifilelook;

extern mh::int_param bacteria; 
struct HashFunction
{
       size_t operator()( const int& k ) const
        {
            // Compute individual hash values for first, second and third
            // http://stackoverflow.com/a/1646913/126995
            size_t res = 17;
            res = res * 31 + std::hash<int>()( k );
            return res;
        }	
	  
};

class CLCS_inst {
public:
	/** Constructor creates an instance by reading it from the specified file. */
	CLCS_inst(const std::string &fname);

public:
	const std::string filename;    ///< Name of instance file.
	int m;                   ///< Number of input strings.
        int k;                   ///< Number of patern strings
	int n;                   ///< Length of the longest input string.
        int pm;                  ///< Lenght of patern strings (supposed they are of equal lenght
	int p;                   ///< Length of the constrained strings
	int sigma;               ///< Size of the alphabet.
	int UB;                  ///< An (trivial) upper bound for the solution length.
        int c_x;  // similarity measure coefficient 
	int c_y; 
	int c_p; 
	std::vector<std::vector<int>> S; ///< Vector of input sequences encoded by integers from [0,sigma).
	std::vector<std::vector<std::vector<int>>> occurance_positions;
	///< structure for reading the number of occurrences of letter <a> of some string i<=m, starting from pl to the end of string (s_i[pos, end]): call asoccurences[i][a][pos]
	std::vector<std::vector<std::vector<int>>> successors;  ///< Succ structure (preprocessing)
	std::vector<std::vector<std::vector<int>>> M_lcs; ///< M stores score matrices (relaxed version only M_{i, i+1}) (LCS case)
	std::vector<std::vector<std::vector<std::vector<int16_t>>>> M; ///< M stores score matrices (relaxed version only M_{i, i+1}) (CLCS case)
	std::vector<char> int2Char; ///< Translation table for internal letters corresponding to integers in [0,sigma) to real alphabet.
	std::map<char, int> char2Int;///< Translation table for real alphabet into integers in [0,sigma).
	std::vector<std::vector<int>> P; ///< Constraint string.
	std::vector<std::vector<int>> embed_end;  ///< Embed structure (preprocessing for Greedy method ==> incremental evaluation only on demand)
	std::vector<std::vector<std::vector<int>>> Embed;  ///< 3D-Embed structure (preprocessing)
	std::vector<std::vector<long double>> P_m; // Matrix of Probabilities (MT, 2012) 
	/** Method for creating a constant problem instance object by reading the instance data from
	 the given file. */
        /** LookUp Structure **/
	std::vector<std::vector<std::vector< std::unordered_map<int, long double, HashFunction> >>> LookheadMap; 
	static CLCS_inst *createFromFile(const std::string &fname) {
		return new CLCS_inst(fname);
	}

	/** Write instance data in a human readable format. */
	void write(std::ostream &os, int detailed = 0) const;

	/** Destructor. */
	~CLCS_inst() {
	} 

private:
	/** preprocessing Occurance-based structure 
            that is required for an efficient UB_1 calculation  */
	std::vector<int> occurances_string_letter(std::vector<int> &str, int letter);
	std::vector<std::vector<int>> occurances_all_letters(std::vector<int> &str);

	/** Succ data structure  */
	std::vector<std::vector<int>> succesors_all_letters(std::vector<int> &str);
	std::vector<int> successor_string_letter(std::vector<int> &str, int a);

	/** UB_2: preprocessing structures for efficient calcualtion of upper bound UB_2  */
	std::vector<std::vector<int>> lcs_m_ij(int i, int j); // creating lcs version of m_ij (score) matrix ...
        std::vector<std::vector<std::vector<int16_t>>> clcs_m_ij(int i, int j, int k);   // creating LCS M_ij score matrix (of dimension |s_i| x |s_j|)
	void store_M();
	/** populate Occ structure */
	void structure_occurances();
	/** populate Embed structure */
	void structure_embeddings();
	/** populate P-matrix for deriving H-heuristic (m-CLCS problem; for BS search used as search guidance) */
	void P_matrix();
         /** UB(s_i1, s_j1) **/
        int ub_2(int i1, int j1);
	/** sim. coef. **/
	void similarity(); 
        /* embed structure: pre-processing... */
        void embeding();


public:
        void update_embed_struct( std::map<int, std::vector<int16_t>> &  temp);
        std::vector<std::vector<int>>  embed( int p_index );
        void ShortestSuperstring( int len);
        void LookAheadStructure(std::string & fname);
        int  findOverlappingPair(std::vector<int>& str1, 
                     std::vector<int>& str2, std::vector<int>& str);
};

}

#endif /* CLCS_INST_H */
