#coding: utf-8

import sys
from collections import deque
from config import *
import logging


__all__ = ['Queue']


class Queue (deque):
	def __init__(self,serviceRate):
		super(Queue, self).__init__(self)
		self.meanDelay = 0.0
		self.meanQueueLen = 0
		self.counter = 0
		self.lastArrivalTime = 0.0
		self.amountOfBytesIn = 0
		self.serviceRate = serviceRate
		
	def push(self,e):

		if(len(self) <= 0):
			e.serviceStartTime = e.packetArrivalTime
		else:
			last_packet = self[-1]
			e.serviceStartTime = last_packet.serviceFinishTime

		e.serviceFinishTime = e.serviceStartTime + e.packetProcessingTime
		e.totalTimeSystem = e.serviceFinishTime - e.packetArrivalTime
		e.totalTimeQueue = e.serviceStartTime - e.packetArrivalTime

		self.append(e)

		self.meanDelay  += e.totalTimeSystem
		self.meanQueueLen += len(self)
		self.counter += 1
		self.lastArrivalTime = e.packetArrivalTime
		self.amountOfBytesIn += e.packetSize

		logging.debug(("+ %s %s %s") % (e.id_,len(self),e.packetArrivalTime))

	def pop(self,time):
		pulled = []
		while len(self) > 0  and self[0].serviceFinishTime < time:
			e = self.popleft()
			pulled.append(e)
			self.meanQueueLen += len(self)
			logging.debug(("- " + "%s "*8) % (e.id_,len(self), e.packetArrivalTime, e.serviceStartTime, e.packetProcessingTime, e.serviceFinishTime, e.totalTimeSystem, e.totalTimeQueue))
		return pulled

	def clean(self):
		return self.pop(sys.float_info.max)

	def getProcessingTime(self,packetSize):
		return float(int(packetSize)*8/self.serviceRate)

	def calculateMeanDelay(self):
		return self.meanDelay/self.counter

	def calculateMeanQueueLen(self):
		return self.meanQueueLen/float(self.counter)

	def calculateMeanArrivalRate(self):
		return (self.amountOfBytesIn*8)/self.lastArrivalTime

	def getUtilization(self):
		return (self.calculateMeanArrivalRate()/self.serviceRate)
