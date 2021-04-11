The Constrained Longest Common Subsequence Problem (CLCS) project

############################################################################################################

This project covers exact and heuristic approaches to solve the CLCS problem.
Moreover, since the implementation of the specialized algorithms to solve the CLCS with two input strings 
have not been available in the literature, the project also provides (re-)implementations of the
state-of-the art approaches, apart of our A* search. 

The project is developed under Linux Ubuntu 18 and C++ with g++ 7.4. 

Participants on the project: 

Marko Djukanovic, MSc,  TU Wien,
Christoph Berger, a student of master studies in Computer Science at TU Wien (about to complete his master),
Günther R. Raidl, Ph.D., TU Wien
and Christian Blum, Ph.D., IIIA-CSIC, UAB, Barcelona

############################################################################################################

I Instalation of the project: 

In order to run the CLCS software, follow the next steps (on Linux):

1. Make sure that the MHLIB software is properly installed and compiled on your local machine. MHLIB is 
   an open source library and can be downloaded from https://bitbucket.org/ads-tuwien/mhlib/src/master/.
   
2. Then store the directory  with all files of the CLCS software into the mhlib project.

3. into the root dir of the CLCS project 
   on terminal line you type: 
 
   export MHLIB="<path_of_mhlib>" (for example, export MHLIB="/home/name/Desktop/projects/mhlib/")
   
   to properly link the MHLIB to the CLCS project.

   Another way to set up this is to look at makefile (line 5 and set ut MHLIB variable)

3. Compile the CLCS project, that is, run 
   
   make
  
   to compile the project. 

4. run the program, see III.

############################################################################################################

II. The external parameters of the project: 

ifile: instance file: path to the instance file 

algorithm: type of the algorithm that you want to run. The following values are possible: 

           0: Approx
           1: Greedy 
           2: Randomized Greedy
           3: GBS framework
           4: CHIN
           5: AE
           6: IR
           7: A*
           8: HUNG
           9: DEO

Algorithms 0-3 are the part of another project.
Algorithms 4-9 cover the paper submitted to the journal Bioinformatics.

time: time limit (in seconds) allowed for execution

beta: beam ( > 0) width if GBS executed

k_filter: any value >= 0,  parameter of GBS for Filter procedure

pruning: 
         0: Prune() method turned off
         1: Prune() method turned on
        
guidance: used to set up search guidance in the heuristic search of GBS. THe following values are possible:

          0: UB
          1: H
          2: H_pow; 
          3: EX; 
          4: Piece-by-piece  -- turned off  (current)
          5: Min-Avg

kext (>=0): used in GBS for limiting the extension set of each level even before other steps (filter and Prune)... 

############################################################################################################

III. Execution of the program

An example showing how to run an A* search on test_instance (see CLCS dir) within a limit of 900 seconds:  
./CLCS -ifile test_instance -ttime 900 -algorithm 7 


