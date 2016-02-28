# -*- coding: utf-8 -*-
import logging
import socket
import threading
import tornado.ioloop
import tornado.iostream
#import gevent
#import gevent.monkey
#gevent.monkey.patch_all()

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

    def sendData(self, data):
        logging.debug("Send: %s", data)
        self._stream.write(data)
        
        message = protocol.Protocol()
        buffer = message.package(type, data)
        self._stream.write(buffer)
        logging.debug("Send message %d size %d: %s", type, len(buffer), data)

    def on_connect(self):
        logging.info("Connect success...")

    def on_close(self):
        logging.info("Close stream...")

    def on_receive(self, data):
        logging.debug("Received: %s", data)
        #self._stream.close()

__client_status__ = ['None', 'Connected', 'Online', 'Offline']

class Bots(TCPClient):
    def __init__(self, name):
        self._name = name
        self._status = __client_status__[0]

    def looper(self, time, func):
        self._looper.call_later(time, func)

    def on_connect(self):
        TCPClient.on_connect(self)
        self._stream.read_bytes(16, self.on_receive)
        self._status = __client_status__[1]
        self.test_sendData()

    def on_close(self):
        TCPClient.on_close(self)
        self._status = __client_status__[3]

    def on_receive(self, data):
        logging.debug("Received: %s", data)

    def test_sendData(self):
        data = "hello python! I'm bots %d" % self._name
        self.sendData(9, data)
        self.looper(2, self.test_sendData)

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
        bots = Bots(i)
        bots.connect("221.228.207.92", 20900, net.getLooper())
    net.start()

if __name__ == "__main__":
    try:
        main()
    except Exception, ex:
        print "Ocurred Exception: %s" % str(ex)
        quit()