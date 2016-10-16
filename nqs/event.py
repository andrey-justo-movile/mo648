#coding: utf-8

__all__ = ['Event']

class Event:

	""" Event class that contains packet informations"""
	id_ = 0

	def __init__(self, packetSize, packetArrivalTime, packetProcessingTime):
		self.id_ = Event.id_
		Event.id_ += 1
		self.packetSize = packetSize
		self.packetArrivalTime = packetArrivalTime
		self.packetProcessingTime = packetProcessingTime
		self.serviceStartTime = 0
		self.serviceFinishTime = 0
		self.totalTimeSystem = 0
		self.totalTimeQueue = 0

	def __str__(self):
		return "<Event id:%s\tsize:%s\tarrival_time:%s\tduration_time:%s>" % (self.id_, self.packetSize, self.packetArrivalTime, self.packetProcessingTime)
