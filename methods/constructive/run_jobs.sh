#!/bin/bash

get_abs_filename() {
  # $1 : relative filename
  echo "$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
}
# parameter description:
#1: alg
#2: bw
#3: guidances
#4: k_best for filtering
# k_ext: fixed (high) 

#testing
declare -i alg=$1;
declare -i beta=$2;
declare -i k_filter=$4;
k_ext=10000;

#instance params
sigmas=(4 12 20)
ms=(5 10 50 100)
sizes=(100 500 1000)
ps=(1 2 5 10)

#dir-name:

prefix="clcs-";
prefix=$prefix"alg-"$1;
prefix=$prefix"bs-$2";
prefix=$prefix"guidance-$3";

# if filtering is incorporated:
if [  "$k_filter" != "0" ]; then
   prefix=$prefix"-k_filter-"$k_filter
fi

for n in "${!sizes[@]}";
  do
   for i in "${!sigmas[@]}";
    do
      for m in "${!ms[@]}";
      do 
         for p in "${!ps[@]}";
	 do
            for index in {0..9}
              do
                  instance_file=`printf "/home1/djukanovic/lcps/src/srcAugust/lcps/src/lcs/clcs/instances/instances/%d_%d_%d_%d.%d" ${ms[m]} ${sizes[n]} ${sigmas[i]} ${ps[p]} $index`;
                  out_file=`printf "clcs_%d_%d_%d_%d.%d"  ${ms[m]} ${sizes[n]} ${sigmas[i]} ${ps[p]} $index`;
                  out_path=`printf "/home1/djukanovic/lcps/src/srcAugust/lcps/src/lcs/clcs/runs/%s" $prefix`;
                  if [[ ! -e $out_path ]]; then
                      mkdir $out_path
                  fi
                  para=`printf " kbest %d kext %d odir %s oname %s" $k_filter $k_ext $out_path $out_file`;
                  name=`printf "%s" "$out_file"`;
		  #echo $name;
                  qsub -N $name -l bc4 -l mem_free=16.1G -l h_vmem=16.1G -l s_vmem=16G -r y -e "/home1/djukanovic/lcps/src/srcAugust/lcps/src/lcs/clcs/runs/err.txt" -o /dev/null  /home1/djukanovic/lcps/src/srcAugust/lcps/src/lcs/clcs/script.sh $instance_file $1 900 $2 $3 "$para"
             done
         done
      done
   done
done
# print the name of dir
echo $prefix;
