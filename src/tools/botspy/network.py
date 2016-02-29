# -*- coding: utf-8 -*-
import logging
import protocol
from gevent.queue import Queue
from gevent.socket import socket
import gevent.monkey
gevent.monkey.patch_all()


class NetClient(object):
    def __init__(self):
        self._sockfd = None
        self._recvbuff = ""
        self._sendqueue = Queue()
        self._work = False

    def connect(self, host, port):
        self._work = True
        self._sockfd = gevent.socket.socket()
        self._sockfd.connect((host, port))
        self.connect_cb()
        gevent.spawn(self._writer)
        gevent.spawn(self._reader)

    def shutdown(self):
        self._work = False
        self._sockfd.close()
        self.shutdown_cb()

    def sendData(self, type, data):
        message = protocol.Protocol()
        buffer = message.package(type, data)
        self._sendqueue.put(buffer)
        logging.debug("Send message %d size %d: %s", type, len(buffer), data)

    def recvData(self, data):
        self._recvbuff += data
        proto = protocol.Protocol()
        if proto.unpackage(self._recvbuff) == True:
            self._recvbuff = self._recvbuff[proto.size():len(self._recvbuff)]
        logging.debug(
            "Recv message %d size %d: %s", proto.type(), proto.size(), proto.data())
        self.receive_cb(proto)

    def connect_cb(self):
        pass

    def shutdown_cb(self):
        pass

    def receive_cb(self, message):
        pass

    def _reader(self):
        while self._work:
            data = self._sockfd.recv(1024)
            if not data:
                self.shutdown()
            else:
                logging.debug("Recv size %d", len(data))
                self.recvData(data)
            gevent.sleep(1)

    def _writer(self):
        while self._work:
            if not self._sendqueue.empty():
                data = self._sendqueue.get()
                self._sockfd.sendall(data)
                logging.debug("Send size %d", len(data))
            gevent.sleep(1)
