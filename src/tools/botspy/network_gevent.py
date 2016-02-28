# -*- coding: utf-8 -*-
import logging
import protocol
import gevent
from gevent.queue import Queue
from gevent.socket import socket
# import gevent.monkey
# gevent.monkey.patch_all()


class NetClient(object):
	def __init__(self):
		self._sockfd = None
		self._recvbuff = None
		self._sendqueue = Queue()

	def connect(self, host, port):
        self._sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
        self._sockfd.connect((host, port))
        logging.info("Connect success...")

    def shutdown(self):
        self._sockfd.close()
        logging.info("Shutdown socket...")

    def sendData(self, type, data):
        message = protocol.Protocol()
        buffer = message.package(type, data)
        self._stream.write(buffer)
        logging.debug("Send message %d size %d: %s", type, len(buffer), data)
        self._stream.read_bytes(1024, self.on_receive, streaming_callback=self.on_receive_stream, partial=True)

	def send(self, data):

