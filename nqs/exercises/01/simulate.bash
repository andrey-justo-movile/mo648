#!/bin/bash

UNIT=000000
SIMULATION_NUMBER=01
SERVICE_RATES=(100$UNIT 95$UNIT 90$UNIT 85$UNIT 80$UNIT 75$UNIT 70$UNIT 65$UNIT 60$UNIT 55$UNIT 50$UNIT 45$UNIT 40$UNIT 35$UNIT 30$UNIT 25$UNIT 20$UNIT 15$UNIT 14$UNIT 13$UNIT 12$UNIT 11$UNIT 10$UNIT)
ARRIVAL_RATE="10"
AMOUNT_OF_SIMULATION=2
OUTPUT_PATH="./$SIMULATION_NUMBER/results"
SIMULATOR_PATH="../../nqs.py"

# If simulation_data directory exist, remove it 
if [ -d $OUTPUT_PATH ]; then
	echo "rm -rf $OUTPUT_PATH"
	rm -rf $OUTPUT_PATH

fi

# creating simulation_data directory
echo "mkdir $OUTPUT_PATH"
mkdir $OUTPUT_PATH

echo "Starting simulation..."

# Running nqs simulator with diferent traffic values
for i in $(seq 1 $AMOUNT_OF_SIMULATION); do
	# For each service rate in SERVICE_RATES
    	for rate in ${SERVICE_RATES[@]}; do
		echo "Simulation #$i with service rate $rate Mbit/s";
			python2.7 $SIMULATOR_PATH -o $OUTPUT_PATH/$i-serviceRate-$rate-bps-$ARRIVAL_RATE$UNIT-bps.out -s $rate -a $ARRIVAL_RATE$UNIT
	done
done

echo "Simulaton was finished.."

echo "Creating .plot files for  gnuplot...";
./input_gnuplot.bash "$OUTPUT_PATH" "${SERVICE_RATES[@]}"
echo ".plot files was created..."


echo "Creating .eps files..."
./plot.bash $OUTPUT_PATH
echo ".eps files was created..."
