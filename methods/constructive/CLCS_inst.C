#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <assert.h>
#include <map>
#include <climits>
#include <algorithm>
#include "mh_log.h"
#include "mh_util.h"
#include "CLCS_inst.h"

//#include <vector>
//#include <unordered_map>
//#include <stdio.h>
//#include <string.h>
//#include <fstream>      // std::ifstream

using namespace std;
using namespace mh;

namespace clcs {
/** mhlib external parameters */
mh::string_param ifilelook("ifilelook", "map structure from lookahead", "");
mh::int_param ub("ub", "upper bound used as a guidance: 0: LCS based UB: 1: CLCS UB (min of two, using the CLCS DP-based matrices)", 0, 0, 1); 
mh::int_param pruning("pruning", "turn on the Prune method", 1, 0, 1);

CLCS_inst::CLCS_inst(const std::string &fname) : filename(fname)
{
	cout << "Read instance " << fname << endl;
	ifstream is(fname);
	if (!is)
		mherror("Cannot open the problem instance file", fname);
        string s_m, s_sigma, s_pm, s_k; 

	is >> s_m >> s_sigma  >> s_pm >> s_k;
        cout << s_m << " " << s_sigma << " " << s_pm << " " << s_k << endl;
        m = stoi( ((string)s_m).substr( 4, s_m.size()));
        sigma = stoi( ((string)s_sigma).substr( 6, s_sigma.size()));
        pm = stoi( ((string)s_pm).substr( 4, s_pm.size()));
        k = stoi( ((string)s_k).substr(2, s_k.size()));
        cout << m << "-" << sigma << "-" << pm << "-" << k << endl;
	if (m < 1)
		mherror("Invalid number of sequences when reading problem instance", tostring(m));
	if (sigma < 2)
		mherror("Invalid alphabet size when reading problem instance", tostring(sigma));
       
	int count = 0;
	int len = 0;
	n = 0;

	// read constrained strings P = (P1,...,P_k)
	cout << "k: " << k << endl;
        // read S
	S = std::vector<std::vector<int>>(m, std::vector<int>());
        for (int i = 0; i < m; i++) {
		// read S_i as string, preceded by the length and transform it in vector<int>
		len = 0;
		string seq;
		is >> len >> seq;
		if (!is || len < 1)
		    mherror("Instance file does not contain enough data");
		if (len != int(seq.size()))
		    mherror("Stated sequence length does not correspond to string length in problem instance", tostring(len));
		vector<int> seqs(len, 0);
                int ind = 0; 
		for(int val: seqs) {
                   S[ i ].push_back( val ); // ind++;
                }

		for (int j = 0; j < len; ++j) { // read characters...
			int a;
			if (char2Int.count(seq[j]) == 0) {

				char2Int[seq[j]] = count;
				int2Char.push_back(seq[j]);
				a = count;
				count++;
			} else
				a = char2Int[seq[j]];
			S[i][j] = a; // coding the letter with number...
		}
		n = max(n, len);
//	cout << seq << endl;
	}
    	
	int sigma1 = char2Int.size();
        P = std::vector<std::vector<int>>(k, std::vector<int>());
       	for(int ix = 0; ix < k; ++ix)
	{   
	    string sq;
            is >> len >> sq; // cout << sq[0] << endl; 
            vector<int> seqs(len, 0);
	    for(int val: seqs)
	        P[ix].push_back(val);
         
            for (int j = 0; j < len; ++j) { // read its characters
		int a = char2Int[sq[j]];
		//cout << "a ===> " << a << endl;
		P[ ix ][ j ] = a;  
	    }
        }
	// up to the same length:
	for(int i = 0; i < S.size(); ++i) 
	{   
            vector<int>s_ii(n, sigma1); 
	    for(int j = 0; j < S[i].size(); ++j) 
	    {
                 s_ii[j] = S[i][j];
	    }
            S[i].clear(); 
	    for(int ix = 0; ix < s_ii.size(); ++ix)
	        S[i].push_back(s_ii[ ix ]); 
	}
	sigma = char2Int.size();
	p = P.size(); //cout << "p" << p << endl;
	structure_occurances(); // create occurrences structure...
	structure_embeddings(); // create embedding structures...
  	store_M();   // create M structure...
	P_matrix(); // populate heuristic (probabilistic) matrix...
	
        // calculate UB for the starting problem 
        UB = 0;
	vector<int> minc(sigma, INT_MAX); // minimal numbers of appearances of each letter in all sequences
	/*for (int i = 0; i < m; i++) {
		vector<int> c(sigma, 0);  // number of appearances of each letter in S_i
		for (int a : S[i])
		     c[a]++;
		for (int a = 0; a < sigma; a++)
		     minc[a] = min(c[a], minc[a]);
		c.clear();
	}
	for (int i = 0; i < sigma; i++) {
	     if (minc[i] >= 1)
	         UB += minc[i];
	}*/
        //ShortestSuperstring(n);
        if(ifilelook() != "" )
	{	
           string sxx=ifilelook();
           LookAheadStructure(sxx);
	 }
}

std::vector<int> CLCS_inst::occurances_string_letter(std::vector<int> &str, int a)
{
	//occurances of letter <a> in string str: 0 - means that it does not appear: vector[pl] denoted the number of occurrences of letter a starting from s[pl, end];
	vector<int> occurances_letter(n, 0);
	// predecessor of S_i for  letter <letter> :
	int number = 0;
	for (int i = n - 1; i >= 0; i--) // successors of X
	{
	     if (str[i] == a)
		 number++;
	     occurances_letter[i] = number;
	}
	return occurances_letter;
}

std::vector<int> CLCS_inst::successor_string_letter(std::vector<int> &str, int a)
{
	vector<int> successor_letter(n + 1, n + 1);
	int size = (int) str.size();
	int number = size + 1; // starting position meaning that no successor of letter <a>
	for (int i = size - 1; i >= 0; --i) {
		if (str[i] == a) {
			number = i + 1;
			successor_letter[i] = number;
		} else
			successor_letter[i] = number;
	}
	if (str[0] == a)
		successor_letter[0] = 1;

	return successor_letter;
}

std::vector<std::vector<int>> CLCS_inst::occurances_all_letters(vector<int> &str)
{
	vector<vector<int>> occurances_all(sigma, vector<int>(n, 0));
	vector<int> occurances_letter(n, 0);

	for (int a = 0; a < this->sigma; a++) // iterate through each letter from alphabet and find Occ vector for it for each input string <str>
		occurances_all[a] = occurances_string_letter(str, a);

	return occurances_all;
}

vector<vector<int>> CLCS_inst::succesors_all_letters(vector<int> &str)
{
	vector<vector<int>> successors_all(sigma, vector<int>(n + 1, n+1));
	for (int a = 0; a < this->sigma; a++) // iterate for all letters from alphabet
		successors_all[a] = successor_string_letter(str, a);
	return successors_all;

}

void CLCS_inst::structure_occurances()
{       // procedure to establish preprocessing structure Occs
	occurance_positions = vector<vector<vector<int> > >(m, vector<vector<int>>(sigma, vector<int>(n, 0)));
	successors = vector<vector<vector<int> > >(m, vector<vector<int>>(sigma, vector<int>(n + 1, 0)));
	for (int i = 0; i < this->m; i++) {
		vector<int> s_i = S[i];
		occurance_positions[i] = occurances_all_letters(S[i]);
		successors[i] = succesors_all_letters(S[i]);
	}
}

void CLCS_inst:: P_matrix()
{
        // P-values from Mousavi and Tabataba (2012): required to establish EX guidance, and here is preprocessed
	P_m = std::vector<std::vector<long double>>(n + 1, std::vector<long double>(n + 1 , 0.0)); // reserve memory
	for (int q = 0; q <= n; ++q)
        {
	     for (int k  = 0; k <= n; ++k)
	     {
		  if (k == 0)
	              P_m[k][q] = 1.0;
		  else
		  if(k > q)
		      P_m[k][q] = 0.0;
		  else
	              P_m[k][q] = ((long double) 1.0 / sigma ) * P_m[k-1][q - 1] + ( (long double) sigma - 1.0)/ (sigma) * P_m[k][q-1];
	     }
        }
}

vector<vector<int>> CLCS_inst::lcs_m_ij(int i, int j)   // creating LCS M_ij score matrix (of dimension |s_i| x |s_j|)
{
	vector<vector<int>> m_ij(S[i].size() + 1, vector<int>(S[j].size() + 1, 0));

	for (int x = (int) S[i].size(); x >= 0; x--) {
		for (int y = (int) S[j].size(); y >= 0; y--) {
			if (x == (int) S[i].size())
				m_ij[x][y] = 0;
			else if (y == (int) S[j].size())
				m_ij[x][y] = 0;
			else if (S[i][x] == S[j][y]) // x + 1 and y + 1
				m_ij[x][y] = m_ij[x + 1][y + 1] + 1;
			else
				m_ij[x][y] = std::max(m_ij[x][y + 1], m_ij[x + 1][y]);   // corresponds to m_ij(i,j) = LCS(s_i(x), s_j(y))
		}
	}
	return m_ij;
}


vector<vector<vector<int16_t>>> CLCS_inst::clcs_m_ij(int i, int j, int px)   // creating CLCS M_ij score matrix (of dimension |s_i| x |s_j| x |P|)
{
	vector<vector<vector<int16_t>>> m_ij(S[i].size() + 1,
			vector<vector<int16_t>>(S[j].size() + 1, vector<int16_t>(P[ px ].size() + 1, INT16_MIN)));
        int ppx = P[ px ].size();
	// initializing borders
	for (int x = 0; x <= (int) S[i].size(); x++) {
		m_ij[x][S[j].size()][ ppx ] = 0;
	}
	for (int y = 0; y <= (int) S[j].size(); y++) {
		m_ij[S[j].size()][y][ ppx ] = 0;
	}
	// calculate values of scoring matrix (from bottom right corner to upper left corner)
	for (int x = (int) S[i].size() - 1; x >= 0; x--) {
		for (int y = (int) S[j].size() - 1; y >= 0; y--){
			for (int z = ppx; z >= 0; z--) {
			     if (S[i][x] == S[j][y]) {
					if (z != ppx && S[i][x] == P[ px ][ z ]) {
					    // strong match
					    m_ij[x][y][z] = 1 + m_ij[x + 1][y + 1][z + 1];
					} else {
					    // weak match
					    m_ij[x][y][z] = 1 + m_ij[x + 1][y + 1][z];
					}
			     } else {
					// no match
					m_ij[x][y][z] = std::max(m_ij[x + 1][y][z], m_ij[x][y + 1][z]);
		          }
		       }
		}
	}
	return m_ij;
}

void CLCS_inst::store_M() //<< creating M structure...
{
	if (ub() != 0) {
		mh::out() << "Using CLCS scoring matrices..." << std::endl;
	}

	for (int i = 0; i < m - 1; i++) 
	     M_lcs.push_back(lcs_m_ij(i, i + 1));
        // CLCS scoring matrix (heuristically should be choosen i,j indices (the smallest factor of simmilarity), for now i=j=0)
        M.push_back(clcs_m_ij(0, 0, 0));	 
}


void CLCS_inst::LookAheadStructure(string & fname)
{
cout << "Ucitavamo..." << endl;
for(int i = 0; i < P.size(); ++i)
{
    vector<vector<unordered_map<int, long double, HashFunction> >> P_i;
    for(int j = 0; j < P[i].size(); ++j)
    {
        vector<unordered_map<int, long double, HashFunction>> ssi; 
        for(int k =0 ; k <  S.size(); ++k ) 
        {
            unordered_map<int, long double, HashFunction> X; 
            // index1:v1 index2:v2,...
            ssi.push_back(X); 
        }     
         P_i.push_back(ssi);
    }   
    LookheadMap.push_back(P_i); 
}

std::ifstream fs(fname, std::ifstream::in); 
if(!fs)
{  
   cout << "File not found " << endl;
   return; 
   //assert("File nnot found);
}
// read results:
string x1,y1,z1,w1, s;
fs >> x1 >> y1 >> z1 >> w1;
std::getline(fs, x1);
std::getline(fs, y1); 
while(fs)
{
   fs >> x1 >> y1 >> z1;

 // Returns first token 
    //char* token = strtok(row.c_str(), ' '); 
    vector<char*> vx;
    // Keep printing tokens while one of the 
    int x = atoi(x1.c_str());
    int y = atoi(y1.c_str());
    int z = atoi(z1.c_str()); cout << x << " " << y << " " << z << endl; 
    // koordinate...
    // parsiranje vrijednosti:
    std::getline(fs, x1); cout << x1 << endl;
    std::getline(fs, s); cout << "s: " << s << endl;
    if( s == "") 
	break;
    std::string delimiter=":";
    std::string token;
    size_t pos = 0; vector<string> vi1; vector<string> vv1;
    bool even = true;
    while ((pos = s.find(delimiter)) != std::string::npos) {
	        token = s.substr(0, pos);
		    std::cout << token << std::endl;
		    if(even) 
		      vi1.push_back(token); 
		    else
	              vv1.push_back(token); 
		    even = !even;
		    if(!even)
		       delimiter=" ";
		    else
		       delimiter=":";    
		    s.erase(0, pos + delimiter.length());
    }
    cout << "\nindex: ";
    vector<int> index; vector<long double> vals1;
    for(int i = 0; i < vi1.size(); ++i){ 
        index.push_back(stoi(vi1[i]));
	cout << index[i] << " "; 
    }
    cout << "\npos: ";
    for(int i = 0; i < vv1.size(); ++i) 
    {
	cout << "abc: " << vv1[i] << endl;
        vals1.push_back(stold(vv1[i]) );
        cout << vals1[i] << " "; 
    }
  for(int i = 0; i <vals1.size(); ++i) 
      LookheadMap[ x ][ y ][ z ].insert( { index[ i ], vals1[ i ] } );    
}
  //print_Matrix(LookheadMap);

}

void CLCS_inst::write(ostream &ostr, int detailed) const
{
	ostr << "m=" << m << " n=" << n << " |Sigma|=" << sigma
		 << " UB=" << UB << endl;
	if (detailed > 0) {
		// write all sequences
		for (int i = 0; i < m; i++) {
			ostr << "S" << i << "=";
			bool first = true;
			for (int a : S[i]) {
			     if (first)
				  first = false;
				else
				  ostr << ",";
			     ostr << a;
			}
			ostr << endl;
		}
		ostr << "Sigma mapping: ";
		for (int i = 0; i < sigma; i++)
		     ostr << i << "->" << int2Char[i] << " ";
		ostr << endl;
		ostr << "Constrained string P:" << P.size() << endl;
                for( int ix = 0; ix < k; ++ix) 
                {
		     for (int px : this->P[ ix ])
		          ostr << px << " ";
                     ostr << endl;
                }
		
	} 
}

void CLCS_inst::structure_embeddings()
{       
       // preporcess Embed data structure
       embed_end = std::vector<std::vector<int>>(m, std::vector<int>()); // Embed[i, pi] --> last index where pi is included in s_i from right to left...
       for ( int i = 0; i < m; i++) {
            
           vector<int> pos_p_i;
           for(int ix = 0; ix < k; ++ix) 
           {
               int index = 1;   int x = n; int match = 0; bool us = false;
               for(int j = S[ i ].size() - 1; j >= 0 and index <= P[ ix ].size() ; --j) 
               {
                   if( S[ i ][ j ] == P[ ix  ][ P[ ix ].size() - index ])
                   {    
                          x = j; 
                          match++;
                          index++;  
                   }
                   if(index == P[ ix ].size())
                      us = true; 
               }
               if( us ) 
                   pos_p_i.push_back( x + 1 );
               else 
                   pos_p_i.push_back( n + 1 );
           }
           for(int val: pos_p_i)
                embed_end[i].push_back( val );
       }
}

void CLCS_inst::update_embed_struct(map<int, vector<int16_t>> &  temp)
{
     //std::map<int, vector<int>>::iterator it = temp.begin();
     // Iterate over the map using c++11 range based for loop
     for (std::pair<int, vector<int16_t>> ix : temp) { 
        vector<int16_t> elem = ix.second; int index = 0;
        for(int val: elem)
        {
           this->embed_end[ix.first][ index ] = val; 
           ++index;
        }
     }
}

/** Shortest supersting problem: greedy algorithm **/

int CLCS_inst::findOverlappingPair(vector<int>& str1, 
                     vector<int>& str2, vector<int>& str)
{
    cout << "str1 " << str1[0] << " " << str1[1] << endl;
    // Max will store maximum 
    // overlap i.e maximum
    // length of the matching 
    // prefix and suffix
    int max = INT_MIN;
    int len1 = str1.size();
    int len2 = str2.size();
 
    // Check suffix of str1 matches
    // with prefix of str2
    for (int i = 1; i <= std::min(len1, len2); i++)
    {
         
        // Compare last i characters 
        // in str1 with first i
        // characters in str2
        bool prefix_i = true;
        for(int ix = 0; ix < i && prefix_i; ++ix) 
        {   //cout <<  str2[ ix ]  << " " << str1[ str1.size() - ix - 1 ] << "\n"; 
            if( str2[ ix ] != str1[ str1.size() - ix - 1 ])
               prefix_i = false; 
        }

        if (prefix_i)
        {
            if (max < i)
            {
                // Update max and str
                max = i;
                //str = str1 + str2.substr(i);
                for(int val: str1) 
                   str.push_back(val); 
                for(int k = 0; k < i; ++k) 
                    str.push_back(str2[ k ]);
            }
        }
    }
 
    // Check prefix of str1 matches 
    // with suffix of str2
    for (int i = 1; i <= min(len1, len2); i++)
    {
         
        // compare first i characters 
        // in str1 with last i
        // characters in str2
        bool prefix_i = true;
        for(int ix = 0; ix < i && prefix_i; ++ix) 
            if(str1[ ix ] != str2[ str2.size() - ix - 1 ])
               prefix_i = false; 

        if (prefix_i )
        {
            if (max < i)
            {
                 
                // Update max and str
                max = i;
                //str = str2 + str1.substr(i);
                //str = str1 + str2.substr(i);
                for(int val: str2) 
                   str.push_back(val); 
                for(int k = 0; k < i; ++k) 
                    str.push_back(str1[ k ]);
            }
        }
    }
 
    return max;
}

void CLCS_inst::ShortestSuperstring( int len)
{
     cout << "Greedy for ShortestSuperstring problem " << endl;
     std::vector<vector<int>> arr;
   
     for(int i = 0; i < P.size(); ++i) 
     {   string s=""; vector<int> s_i = vector<int>(P[i].size(), 0); 
         for(int j = 0; j < P[ i ].size(); ++j) 
         {   //cout << P[ i ][ j ] << endl;
             s_i[j] = P[ i ][ j ];
         }
 
         arr.push_back(s_i); 
     }
     len = S.size(); 
     cout << "Greedy for ShortestSuperstring problem intiallize " << endl;
     // Run len-1 times to 
     // consider every pair
     while(len != 1)
     {
         
        // To store  maximum overlap
        int max = INT_MIN;   
       
        // To store array index of strings
        int l, r;    
       
        // Involved in maximum overlap
        vector<int> resStr;    
       
        // Maximum overlap
        for (int i = 0; i < len; i++)
        {
            for (int j = i + 1; j < len; j++)
            {
                vector<int> str;
 
                // res will store maximum 
                // length of the matching
                // prefix and suffix str is 
                // passed by reference and
                // will store the resultant 
                // string after maximum
                // overlap of arr[i] and arr[j], 
                // if any.
                cout << i << " " << j << endl;
                int res = findOverlappingPair(arr[i], 
                                         arr[j], str);
 
                // check for maximum overlap
                if (max < res)
                {
                    max = res;
                    for(int val: str)
                       resStr.push_back(val); 
                    l = i, r = j;
                }
            }
        }
 
        // Ignore last element in next cycle
        len--;   
 
        // If no overlap, append arr[len] to arr[0]
        if (max == INT_MIN)
        {    
             for(int val: arr[len]) 
             arr[0].push_back(val); 
        }
        else
        {
            // Copy resultant string to index l
            arr[l].clear(); 
            for(int val: resStr) 
                arr[l].push_back(val); 
           
            // Copy string at last index to index r
            arr[r].clear(); 
            for(int val: arr[len]) 
                arr[r].push_back(val);  
        }
    }
    cout << "Superstring " << arr[0].size() << endl;
}

} // namespace lcps
