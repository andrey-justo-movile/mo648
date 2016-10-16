#!/bin/bash

OUTPUT_PATH=$1
SERVICE_RATES=("${@:2}")


DATA=('Utilization' 'Mean-delay' 'Mean-queue-length')

if [ -d $OUTPUT_PATH/gnuplot ]; then
	rm -rf $OUTPUT_PATH/gnuplot
fi
mkdir $OUTPUT_PATH/gnuplot


for rate in ${SERVICE_RATES[@]}; do
	array=()
	i=0
	for data in ${DATA[@]}; do
		array[i]=`cat $OUTPUT_PATH/*serviceRate-$rate-bps* | grep "$data" | awk -F " " {'print $5'} | awk '{sum+=$1; sumsq+=$1*$1;} END {avg=sum/NR; std=sqrt(sumsq/NR - avg^2); ci99=(2.575 * std)/sqrt(NR); printf "%.12f %.12f %.12f %.12f %.12f\n", avg, std, ci99, avg-ci99, avg+ci99}' | awk '{print $1, $3}'` 
		((i++))
	done
	echo "`echo ${array[0]} | awk '{print $1}'` `echo ${array[1]}`" >> $OUTPUT_PATH/gnuplot/`echo ${DATA[1]} | tr "[:upper:]" "[:lower:]"`.plot 
	echo "`echo ${array[0]} | awk '{print $1}'` `echo ${array[2]}`" >> $OUTPUT_PATH/gnuplot/`echo ${DATA[2]} | tr "[:upper:]" "[:lower:]"`.plot 
done



