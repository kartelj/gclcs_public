#!/bin/bash

#gdb -batch -ex "run" -ex "bt" --args /home1/djukanovic/lcps/src/srcAugust/lcps/src/LCPS -ifile $1 -alg $2 -ttime $3 -diving_num $4 -beta $5 $6 $7 $8
#algrind --leak-check=yes 
#gdb -batch -ex "run" -ex "bt" --args  
/home1/djukanovic/lcps/src/srcAugust/lcps/src/lcs/clcs/CLCS -ifile $1 -algorithm $2 -ttime $3 -bw $4 -guidance $5 $6 $7 $8 $9
