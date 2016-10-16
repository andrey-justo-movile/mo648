#! /usr/bin/python
# coding: utf-8


import sys
	
#check python version
if not hasattr(sys, "hexversion") or sys.hexversion < 0x20701f0:
	sys.stderr.write("Sorry, your Python is too old.\n")
  	sys.stderr.write("Please, use python 2.7 or greater.\n")
  	sys.exit(1)

#abort solver execution with a problem occur
def abort():
	logging.error("Program will exit. Bye")
	sys.exit(-1)

# simulate the network queue
def simulate(filename,queue):
	try:
		with open(filename,'r') as traceFile:
			logging.warning("Starting simulation...")
			for line in traceFile.readlines():
				(packetArrivalTime,packetSize) = line.split()
				packetProcessingTime = queue.getProcessingTime(packetSize)
				queue.pop(float(packetArrivalTime))
				queue.push(Event(int(packetSize),float(packetArrivalTime),packetProcessingTime))		
			queue.clean()
			logging.warning("Simulaton was finished...")
			
	except IOError as ioerr:
		logging.exception('File creation error: IOError: %s' % (ioerr))
		abort()

# prints queue results in accordance with logging level
def printResults(queue):
	logging.info("=Mean-arrival-rate: %s" % queue.calculateMeanArrivalRate())
	logging.info("=Mean-queue-length: %s" % queue.calculateMeanQueueLen())
	logging.info("=Mean-delay: %s" % queue.calculateMeanDelay())
	logging.info("=Utilization: %s" % queue.getUtilization())

if __name__ == "__main__":

	from config import *
	from trafficGenerator import *
	from queue import Queue
	from event import Event
	from time import time
	import logging
	import argparse
	

	# Set command line parameters
	parser = argparse.ArgumentParser(description="Network queue simulator (NQS)", add_help=True, prog="nqs.py", usage='python %(prog)s [options]', epilog="Mail me (thiagogenez@ic.unicamp.br) for more details")

	parser.add_argument("--version", action="version", version='%(prog)s 2.0')
	parser.add_argument("-i", "--input-trace-file", action="store", type=str, dest="traceFile", default=None, help="Trace input file that NQS will simulate", metavar="FILE")
	parser.add_argument("-o", "--output-file", action="store", nargs='?',  type=argparse.FileType('wb', 0), dest="outputFile", default=sys.stdout, help="Output file where simulation results  will be stored", metavar="FILE")
	parser.add_argument("-t", "--simulation-time", action="store", type=float, dest="simulationTime", default=100,  help="Time simulation of simulator clock", metavar="TIME")
	#parser.add_argument("-v", "--verbose", action="store_true", dest="verbose", help = "Verbose mode ", default=False)
	parser.add_argument("-a", "--arrival-rate", action="store", type=float, dest='arrivalRate', default=ARRIVAL_RATE, help="Arrival rate (in bits) per second", metavar="RATE")
	parser.add_argument("-s", "--service-rate", action="store", type=float, dest='serviceRate', default=SERVICE_RATE, help="Service rate (in bits) per second", metavar="RATE")
	parser.add_argument("-b", "--average-packet-size", action="store", type=int, dest="averagePacketSize", default=AVERAGE_PACKET_SIZE, help="Avarege packet size in bytes")
	parser.add_argument("-l", "--logging", dest="logging", type=str, default=LOG_LEVEL, help="Logging level", metavar="[DEBUG,INFO,WARNING,ERROR,CRITICAL]")
	
	# parsing arguments
	try:
		args = parser.parse_args()
	except IOError as ioerr:
		logging.exception('File creation error: IOError: %s' % (ioerr))
		abort()

	# Setting logging level
	try:
		logging.basicConfig(stream=args.outputFile, filemode='w',format='%(asctime)s %(levelname)s %(message)s', level=args.logging.upper())
	except ValueError as verror:
		logging.exception("%s" % verror)
		logging.error("use -h option to help")
		abort()

	#set logging level
	logging.debug(args)
	logging.warning("Setting (%s) was output" % args.outputFile)
	
	# if trace file is not pass as parameter, then the simulator will create one
	if args.traceFile == None:
		if args.outputFile is not sys.stdout:

			#args.traceFile = args.outputFile.name.replace('.out', '.trace')
			#keep each trace file will waste space in disc, so i decided overwritting the traffic.trace file
			args.traceFile = args.outputFile.name.replace(args.outputFile.name[args.outputFile.name.rfind('/')+1:],'traffic.trace')
		else:
			args.traceFile = "traffic.trace"

		logging.warning("Creating trace file ...")
		generate(args.traceFile,args.simulationTime,args.arrivalRate, args.averagePacketSize)
		logging.warning("Trace file (%s) was created..." % args.traceFile)
	else:
		logging.warning("Using %s as trace file " % args.traceFile)
	


	logging.warning("Creating queue...")
	#creating queue to simulation
	q = Queue(args.serviceRate)
	logging.warning("Queue was created...")

	#simulate network queue	
	simulate(args.traceFile,q)
	
	#printing simulator results
	printResults(q)
	






