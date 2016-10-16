#coding: utf-8

import sys

__all__ = ['Logger']

class Logger(object):
	@deprecated
	def __init__(self, fileOut, verbose):
		self.verbose = verbose
		if(self.verbose):
			self.stdout = sys.stdout
		self.fileOut = fileOut
		if(self.fileOut != None):
			self.log = open(logfile, 'w')
 
	def write(self, text):
		if(self.verbose):
			self.stdout.write(text)
		if(self.fileOut != None):
			self.log.write(text)
			self.log.flush()
 
	def close(self):
		if(self.verbose):
			self.stdout.close()
		if(self.fileOut != None):
			self.log.close()