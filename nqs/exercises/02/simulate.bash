#!/bin/bash

UNIT=000000
SERVICE_RATES=(100$UNIT 95$UNIT 90$UNIT 85$UNIT 80$UNIT 75$UNIT 70$UNIT 65$UNIT 60$UNIT 55$UNIT 50$UNIT 45$UNIT 40$UNIT 35$UNIT 30$UNIT 25$UNIT 20$UNIT 15$UNIT 14$UNIT 13$UNIT 12$UNIT 11$UNIT)

OUTPUT_PATH="./results"
SIMULATOR_PATH="../../nqs.py"
INPUT_TRACE="traffic_exercice2_sorted.in"


# If simulation_data directory exist, remove it 
if [ -d $OUTPUT_PATH ]; then
	echo "rm -rf $OUTPUT_PATH"
	rm -rf $OUTPUT_PATH
fi

# creating simulation_data directory
echo "mkdir $OUTPUT_PATH"
mkdir $OUTPUT_PATH

echo "Starting Execution..."

# For each service rate in SERVICE_RATES
for rate in ${SERVICE_RATES[@]}; do
	echo "Simulation #$i with service rate $rate Gbits";
	python2.7 $SIMULATOR_PATH -i $INPUT_TRACE -o $OUTPUT_PATH/serviceRate-$rate-bps.out -s $rate
done

echo "Simulations over!"

echo "Creating .plot files for  gnuplot...";
./input_gnuplot.bash "$OUTPUT_PATH" "${SERVICE_RATES[@]}"
echo ".plot files was created..."

echo "Creating .eps files..."
./plot.bash $OUTPUT_PATH
echo ".eps files was created..."
