#coding: utf-8

import random,sys,math
import logging
from config import *

__all__ = ['generate']


def exponential (mean):
	return (-mean * math.log(random.random()))

def getPacketSize (averagePacketSize):	
	packetSize = int(exponential(averagePacketSize))
	while (packetSize < MIN_PACKET_SIZE) or (packetSize > MAX_PACKET_SIZE):
		packetSize = int(exponential(averagePacketSize))

	return packetSize

def getArrivalTime(arrivalRate,averagePacketSize):
	return exponential(( averagePacketSize * 8)/arrivalRate)

def generate(filename,simulationTime, arrivalRate, averagePacketSize):
	try:
		with open(filename,'w') as file_in:
			time = 0.0
			while time <= simulationTime:
				time += getArrivalTime(arrivalRate,averagePacketSize)
				packetSize = getPacketSize(averagePacketSize)	
				file_in.write("%s \t %s\n" % (time,packetSize))

	except IOError as ioerr:
		logging.exception('File creation error: IOError: '+str(ioerr))
