# -*- coding: utf-8 -*-
import logging
import socket
import tornado.ioloop
import tornado.iostream
import protocol


class Network(object):
    def __init__(self):
        self._ioloop = tornado.ioloop.IOLoop.instance()

    def start(self):
        logging.info("Start network...")
        self._ioloop.start()

    def stop(self):
        logging.info("Stop network...")
        self._ioloop.stop()

    def getLooper(self):
        return self._ioloop


class TCPClient(object):
    def __init__(self):
        self._stream = None
        self._looper = None

    def connect(self, host, port, looper):
        self._looper = looper
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
        self._stream = tornado.iostream.IOStream(sock)
        self._stream.set_close_callback(self.on_close)
        self._stream.connect((host, port), self.on_connect)

    def shutdown(self):
        logging.info("Shutdown stream...")
        self._stream.close()

    def sendData(self, type, data):
        message = protocol.Protocol()
        buffer = message.package(type, data)
        self._stream.write(buffer)
        logging.debug("Send message %d size %d: %s", type, len(buffer), data)
        self._stream.read_bytes(1024, self.on_receive, streaming_callback=self.on_receive_stream, partial=True)

    def on_connect(self):
        logging.info("Connect success...")

    def on_close(self):
        logging.info("Close stream...")

    def on_receive(self, data):
        logging.debug("Received: %s", data)

    def on_receive_stream(self, data):
        logging.debug("Received stream: %s", data)
