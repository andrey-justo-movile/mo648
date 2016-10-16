#!/bin/bash

OUTPUT_PATH=$1
SERVICE_RATES=("${@:2}")


DATA=('Utilization' 'Mean-queue-length')

if [ -d $OUTPUT_PATH/gnuplot ]; then
	rm -rf $OUTPUT_PATH/gnuplot
fi
mkdir $OUTPUT_PATH/gnuplot


for rate in ${SERVICE_RATES[@]}; do
	array=()
	i=0
	for data in ${DATA[@]}; do
		array[i]=`cat $OUTPUT_PATH/*serviceRate-$rate-bps* | grep "$data" | awk -F " " {'print $5'} | awk '{sum+=$1;} END {avg=sum/NR; printf "%.12f\n", avg}' | awk '{print $1}'` 
		((i++))
	done
	echo "`echo ${array[0]} | awk '{print $1}'` `echo ${array[1]}`" >> $OUTPUT_PATH/gnuplot/`echo ${DATA[1]} | tr "[:upper:]" "[:lower:]"`.plot 
done



