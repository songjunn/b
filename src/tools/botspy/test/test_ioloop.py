# -*- coding: utf-8 -*-
import logging
import socket
from tornado.ioloop import IOLoop

ioloop = IOLoop.instance()

class Bots(object):
	def __init__(self):
		self._connfd = 0

	def connect(self, addr, port):
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		sock.connect((addr, port))
		self._connfd = sock.fileno()

	def shutdown(self):
		pass

	def send(self, data):
		pass

	def recv(self):
		pass

	def close(self):
		pass

def main():
	bots = Bots()
	bots.connect("221.228.207.92", 20900)

if __name__ == "__main__":
    try:
        main()
    except Exception, ex:
        print "Ocurred Exception: %s" % str(ex)
        quit()