#!/bin/bash
# Script to sum up and aggregate results from real data benchmarks
# USE DIRECTORY AS PARAMETER
base_dir=${1%/}
sum_dir=$base_dir/sum
if [[ ! -e $sum_dir ]]; then
	mkdir $sum_dir
fi
for d in $base_dir/clcs-*/ ; do
    echo "processing directory $d ..."
    sfile_name=`basename $d`
    python summary.py $d > ${sum_dir}/sum-${sfile_name}.txt
    ./launch_r2.bat ${sum_dir}/sum-${sfile_name}.txt ${sum_dir}/agg-${sfile_name}.csv
    sed 's/file/index file/g' ${sum_dir}/agg-${sfile_name}.csv > ${sum_dir}/${sfile_name}.csv
    rm ${sum_dir}/agg-${sfile_name}.csv
done