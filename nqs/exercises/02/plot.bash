#!/bin/bash


OUTPUT_PATH=$1


DATA=('Mean-queue-length')

if [ -d $OUTPUT_PATH/eps ]; then
	rm -rf $OUTPUT_PATH/eps
fi
mkdir $OUTPUT_PATH/eps


for data in ${DATA[@]}; do

	if [ "$data" == "Mean-delay" ]; then
		title="Utilização x Retardo médio" 
		ylab="Retardo Médio" 
		using="1:2:3" 
		with="with yerrorlines pointtype 12"
	elif [ "$data" == "Mean-queue-length" ];then
		title="Utilização x Tamanho da Fila"
		ylab="Tamanho Médio da Fila"
		using="1:2"
		with="pointtype 12" 
	fi 

	gnuplot << EOF
		reset
		set encoding utf8
		set   autoscale           # scale axes automatically
		unset log                 # remove any log-scaling
		unset label               # remove any previous labels

		set mytics
		set style data linespoints
		set xlabel "Utilização"
		set ylabel "$ylab"
		set title "$title"

		set key left top
		set term postscript enhanced eps dashed lw 1 "Helvetica" 14
		set output "$OUTPUT_PATH/eps/`echo $data | tr "[:upper:]" "[:lower:]"`.eps"

		plot "$OUTPUT_PATH/gnuplot/`echo $data | tr "[:upper:]" "[:lower:]"`.plot"  using `echo $using` title "$ylab" `echo $with`
EOF

done
