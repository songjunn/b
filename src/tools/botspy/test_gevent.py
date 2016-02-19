# coding: utf-8
import gevent
import gevent.monkey
from gevent.socket import create_connection, timeout
gevent.monkey.patch_all()

class Bots(object):
    def __init__(self, id):
        self._id = id
        self._ip = ""
        self._port = 0
        self._sock = None

    def connectServer(self, ip, port):
        self._ip = ip
        self._port = port
        self._sock = create_connection((ip, port))

    def sendData(self, data):
    	  self._sock.send(data)

def createBots(i):
    bots = Bots(i)
    bots.connectServer('192.168.6.47', 20900)
    while (True):
        bots.sendData("abcdefg")

def main():
    threads = [gevent.spawn(createBots, i) for i in xrange(10)]
    gevent.joinall(threads)

if __name__ == '__main__':
    main()