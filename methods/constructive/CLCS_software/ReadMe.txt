The Generalized Constrained Longest Common Subsequence Problem (CLCS) with many constraints

############################################################################################################

This project covers heuristic approaches to solve the generalized CLCS problem with many patterns.
The project is developed under Linux Ubuntu 18 and C++ with g++ 7.4. 


############################################################################################################

I Instalation of the project: 

In order to run the CLCS software, follow the next steps (on Linux - Ubuntu):

1. Make sure that the MHLIB software is properly installed and compiled on your local machine. MHLIB is 
   an open source library and can be downloaded from https://bitbucket.org/ads-tuwien/mhlib/src/master/.
   
2. Then store the directory  with all files of the CLCS software into the mhlib project.

3. Into the root dir of the CLCS project 
   on terminal line you run: 
 
   export MHLIB="<path_of_mhlib>" (for example, export MHLIB="/home/name/Desktop/projects/mhlib/")
   
   to properly link the MHLIB to the CLCS project.

   Another way to set up this is to look at makefile (line 5 and set up the MHLIB variable)

3. Compile the CLCS project, that is, run the following commands: 
   
   make
  
   to compile the project. 

4. run the program, see III.

############################################################################################################

II. The external parameters of the project: 

ifile: instance file: path to the instance file 

algorithm: type of the algorithm that you want to run. The following values are possible: 

           0: A*
           1: Greedy 
           3: BS variants 

ttime: time limit (in seconds) allowed for execution; in our paper we set it up to 1200 (s) 

bw: beam ( > 0) width if algorithm = 3 is called

kbest: any value >= 0,  parameter of BS for Filtering procedure

guidance: used to set up search guidance in the heuristic search of GBS. THe following values are possible:

          0: UB
          1: H guidance from the paper

bacteria: 0: make sure to set up this value when you run algorithms on the instances which do not belong to benchmark set Real 
          1: ensure this value when run the program on the instances which belong to benchmark set Real 

feasibility: 0: if you want to run a basic-BS
             1: when the algorithm is run w.r.t. feasiblity check (use if you want to run a restricted-BS)
############################################################################################################

III. Execution of the program

An example showing how to run an A* search on test_instance (see CLCS dir) within a limit of 900 seconds:  
./CLCS -ifile test_instance -ttime 1200 -algorithm 0 


