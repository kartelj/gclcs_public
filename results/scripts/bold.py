#!/usr/bin/env python3
#Using bold script: 
# copy your results in the shape
#m & n & v11 & v12 &...& v16 \\
#m & n & v21 & v22 &...& v26 \\
#...

def main(file_name):

    #TABLE SIZE columns: 
    TABLE_COLUMNS = 8
    M=1 # how many of different m-values
    M_DISPLAY = 0 # specific way of display m-column: element in the middle printed
    objectives=[1, 3, 5] # columns' numbers of the table where the objectives are given (counted from 0)
    bold=""
    with open(file_name) as f:
       rowi = 1
       for row in f:
          col = row.split('&')
          print(col)
          if(len(col) == TABLE_COLUMNS):
             best=0
             for i in range(0, TABLE_COLUMNS):   # compare 2,4,6th elements; save the largest (script specific)
                  #if(i >=2 and i <= TABLE_COLUMNS and i % 2 == 0 and float(col[i].strip()) > best):
                   if (col[i].strip() not in ( "-", "", " ") 
                       and (i in objectives) and ( float(col[i].strip()) > best) ):
                      #if col[i].strip() != "-" :
                      best=float(col[i].strip());
             for i  in range(0, TABLE_COLUMNS): 
               if(i > 1): #last element just being copied
                 #if ( i % 2 == 0 and  i < 7 and best == float(col[i].strip()) ): 
                  if i in objectives and col[i].strip() not in ( "-", " ", "")  and col[i].strip() != " - " and best == float(col[i].strip()) :
                     bold=bold+"\\textbf{" + col[i].strip() + "}" + ' & '
                  else:# if not objectives:
                     bold = bold + col[i]
                     if i < TABLE_COLUMNS - 1:  # do not add & for all except the last column...
                        bold = bold + ' & '
               else:  #  [m,n]  columns
                   if i == 0 : 
                       bold = bold + col[i] + ' & '
                   else: #i==0:
                       if  rowi % M == M_DISPLAY :
                           bold = bold  + col[i] + ' & '
                       else:
                           bold = bold + ' & ' 
          else:
            bold=bold+row 
          if rowi % M == 0:
             bold = bold + '\hline \n'
          rowi = rowi + 1    

     # output file generated and content included...

    with open(file_name + "_bold" + ".txt", "w") as f:
        f.write(bold)

if __name__ == "__main__":
    import sys 
    main(sys.argv[1]) # pick first argument in command line as file for reading...
    #print(main(sys.argv[1]))
