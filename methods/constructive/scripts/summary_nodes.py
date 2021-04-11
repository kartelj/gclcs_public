#!/usr/bin/python3
# vim: tabstop=2 shiftwidth=2 expandtab
#
# Adopted from mhlib and specialized for the LCPS problem
#
# Summarizes the essential information from several heuristic algorithm runs

# into a CSV-table written to stdout.

# The result files to be processed are given as arguments (out-files or 
# directories containing out-files)

# The CSV-table contains:
# - File: name of processed out-filename (File)
# - obj: objective value of final solution (SOLUTOIN:)
# - tbest: CPU-time when final solution was found
# - UB: general upper bound for instance
#: - itot - total number of iteration
#
# Options:
# -t print column for ties information 
# -u  print column with information about UB_1 < UB_2
# if no parameter - no printing of ties and (UB_1 < UB_2) chekc column
# -s the column for drawing SQT plots:  


from __future__ import print_function
import sys, getopt, os, glob, re

paramf = False  # print also result of initial solution and first iteration; set by option -f
paramub = False
lbPath = "/.astar/"
paramt = False  # print also profiles, thus a list of triples {time, MS, LB,...}
paramamount = False;
paramub2_time_pct = False # print paramub2_time_pct
param_sqt=False
# process list of files/directories
def processlist(args):
  for file in args:
    if os.path.isdir(file):
      processlist(glob.glob(file+"/*"))
    else:
      # process out-file
      resfound = False  # becomes True when all data found
      profile = []
      times = []
      # the values for columns initialized 
      reexpanded = 0
      obj= ttot = ittot = ''
      UB=""
      ub1_large = ties = tbest = num_gen_nodes=0
     # print(file)
      if file.split(".")[-1] == "out" :
       with open(file) as f:
        for line in f:
          # apply regular expressions...

          m = re.match(r'UB:(\d+\.?\d*)',line)
          if m: UB = m.group(1)
          #m = re.match(r'\s*value\s*:\s*(\d+\.?\d*)', line)
          #if m: obj = m.group(1)
          m = re.match(r'\s*(value|obj)\s*:\s*(\d+\.?\d*)\s*\n*\s*(ttime)\s*:\s*(\d+\.?\d*)',line)
          if m : ttot=m.group(4)

          m = re.match(r'\s*Node\s*Stats\s*\(created,\s*expanded,\s*ignored,\s*merged\):\s*(\d+).*',line)
          if m : obj=m.group(1);

          #if m: obj = m.group(2); ttot=m.group(4) #tbest was here
          resfound=False
          #m = re.match(r'Solution:\s*(\d+)\s*time:\s*(\d+\.?\d*)[\da-zA-Z:=_\s\t]*gap:\s*(-?\d+\.?\d*)', line)

         # m = re.match(r'Solution:\s*(\d+)\s*time:\s*(\d+\.?\d*)\s*[=]*\s*[new]*\s*best\s*[\d+]*\s*Iteration\s*:\s*(\d+)\s*gap\s*[:]*\s*(\d+\.?\d*)\s*expand nodes\s*[:]*\s*(\d+\.?\d*)', line)
         # m = re.match(r'Solution:\s*(\d+)\s*time:\s*(\d+\.?\d*)\s*[=]*\s*[new]*\s*best\s*[\d+]*\s*Iteration\s*:\s*(\d+)\s*gap\s*[:]*\s*(\d+\.?\d*)', line)

         # if m: a1 = float(m.group(1)); a2 = float(m.group(2)); a3=float(m.group(3));a4=float(m.group(4)); profile.append(a1); profile.append(a2); profile.append(a3);profile.append(a4);   #  times. print(profile)

          
          if file.split(".")[-1] == 'out' :#log
           resfound=True

          if obj=="":
             obj=0

          if UB=='':
           UB=0
          
          if ittot=="":
           ittot=0

          if ttot=="" :
            ttot=0

      if resfound:
        #print(file,obj,ttot,ittot,UB, ties, sep="\t",end=''); #ub1_large, ties, 
        if  paramub:
            print(file,obj,ttot,ittot,UB,ub1_large, sep="\t",end=''); 
        elif paramt:
            print(file,obj,ttot,ittot,UB,ties, sep="\t",end='');
        elif paramamount:
            print(file,obj,ttot,ittot,UB, ub1_larger,ub1_smaller,ub1_equal, ub1_ub2_diff, sep="\t",end=''); 
        elif paramub2_time_pct:
            print(file,obj,ttot,ittot,UB, profile, sep="\t",end=''); # ub2_time_pct
        elif param_sqt:
            print(file, obj, ttot, ittot, UB, profile, sep="\t",end='');
        else:
            print(file, obj, ttot, ittot, tbest,  sep="\t",end='');
        print()
        
        

try:
  opts, args = getopt.getopt(sys.argv[1:],'hfutaps:')
except getopt.GetoptError:
  print('summary.py -f <out-files>')
  sys.exit(2)

for opt, arg in opts:
  if opt == '-h':
    print('summary.py -f <out-files>')
    sys.exit()
  elif opt == "-u":
      paramub = True
  elif opt == "-a" :
      paramamount = True 
  elif opt == "-p": # percentafe of time used for calculating UB_2
      paramub2_time_pct = True
  elif opt == "-t":
      paramt = True
  elif opt == "-s":
      param_sqt=True
      lbPath = arg
#print("file\tobj\tttot\tittot\tub\tties",end='') #\tub1_large\tties
if paramub:
   print("file\tobj\tttot\tittot\tub\tub1_large",end='')
elif paramt:
   print("file\tobj\tttot\tittot\tub\tties",end='')
elif paramamount:
   print("file\tobj\tttot\tittot\tub\tub1_larger_pct\tub1_weaker_pct\tub1_equal_pct\tub1_ub2_diff",end='')
elif paramub2_time_pct:
   print("file\tobj\tttot\tittot\tub\tprofile",end='') #ub_2_time_pct
elif param_sqt:
   print("file\tobj\tttot\tittot\tub\tprofile",end='')
else:
  print("file\tobj\tttot\tittot\ttbest",end='') 
print()
processlist(args)
