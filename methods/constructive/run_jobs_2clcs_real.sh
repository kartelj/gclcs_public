#!/bin/bash

# parameter description:
# 1: alg
declare -i alg=$1;

#dir-name:
prefix="clcs-";
prefix=$prefix"alg-"$1;

for instance_file in /home1/e01129111/instances/real/* ; do
  filename=$(basename -- "$instance_file")
  #extension="${filename##*.}"
  out_file=`printf "clcs_%s" $filename`;
  out_path=`printf "/home1/e01129111/dec-10/runs-r/%s" $prefix`;
  if [[ ! -e $out_path ]]; then
    mkdir $out_path
  fi
  para=`printf " odir %s oname %s" $out_path $out_file`;
  name=`printf "%s" "$out_file"`;
  qsub -N $name -l bc4 -l mem_free=16.1G -l h_vmem=16.1G -l s_vmem=16G -r y -e "/home1/e01129111/dec-10/runs-r/err.txt" -o /dev/null /home1/e01129111/dec-10/script.sh $instance_file $1 900 1000 3 "$para"
done

# print the name of dir
echo $prefix;
