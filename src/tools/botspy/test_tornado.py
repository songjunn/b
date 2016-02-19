# -*- coding: utf-8 -*-
import logging
import socket
import threading
import tornado.ioloop
import tornado.iostream
import gevent
import gevent.monkey
gevent.monkey.patch_all()

class Network(object):
    def __init__(self):
        self._ioloop = tornado.ioloop.IOLoop.instance()

    def start(self):
        logging.info("start network...")
        self._ioloop.start()

    def stop(self):
        logging.info("stop network...")
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

    def sendData(self, args):
        data = args[0]
        logging.debug("Send: %s", data)
        self._stream.write(data)

    def on_connect(self):
        logging.info("Connect success...")
        #self.looper(1, self.sendData, "hello")
        #self._looper.call_later(1, self.sendData, "hello")

    def on_close(self):
        logging.info("Close stream...")

    def on_receive(self, data):
        logging.debug("Received: %s", data)
        self._stream.close()

class Bots(TCPClient):
    def __init__(self, name):
        self._name = name

    def looper(self, time, func, *args, **kwargs):
        self._looper.call_later(time, func, args)

    def test_sendData(self):
        self.looper(1, self.sendData, "hello")

def init_logging():
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)
    sh = logging.StreamHandler()
    formatter = logging.Formatter('%(asctime)s %(module)s-L%(lineno)d-%(levelname)s: %(message)s')
    sh.setFormatter(formatter)
    logger.addHandler(sh)
    logging.info("set logging level: %s", logging.getLevelName(logger.getEffectiveLevel()))

def main():
    init_logging()
    net = Network()
    #threads = [gevent.spawn(createBots, i) for i in xrange(2)]
    #gevent.joinall(threads)
    for i in xrange(2):
        bots = TCPClient()
        bots.connect("192.168.6.47", 20900, net.getLooper())
    net.start()

if __name__ == "__main__":
    try:
        main()
    except Exception, ex:
        print "Ocurred Exception: %s" % str(ex)
        quit()