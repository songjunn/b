# coding: utf-8
import logging
import threading
import protocolpack
from twisted.internet import reactor, protocol, task, defer

class NetClient(protocol.Protocol):
    def connectionMade(self):
    	print "NetClient Connection made."
    
    def connectionLost(self, reason):
        print "NetClient Connection lost:", reason

    def dataReceived(self, data):
        print "NetClient Received..."

    def sendData(self, type, data):
        message = protocolpack.Protocol()
        buffer = message.package(type, data)
        #self.transport.write(buffer)
        logging.debug("Send message %d size %d: %s", type, len(buffer), data)

    def shutdown(self):
        self.transport.loseConnection()

class Bots(NetClient):
    def __init__(self):
        self._id = 0

    def setID(self, id):
        self._id = id
        print "I'm bots %d" % self._id

    def startNetwork(self, ip, port):
        #reactor.connectTCP(ip, port, NetFactory())
        c = protocol.ClientCreator(reactor, Bots)
        net = c.connectTCP(ip, port)
        #net = defer.Deferred()
        #net.addCallback(self.schedule, 1, self.test_send)
        self.schedule(None, 1, self.test_send)

    def connectionMade(self):
        print "Bots Connection made."
        #self.schedule(1, self.test_send)
        #net = defer.Deferred()
        #net.addCallback(self.schedule, 1, self.test_send)
        

    def schedule(self, fileObj, timer, func):
        call = task.LoopingCall(func)
        call.start(timer)

    def test_send(self):
        data = "hello python! I'm bots %d" % self._id
        print data
        self.sendData(9, data)

class NetFactory(protocol.ClientFactory):
    protocol = Bots

    def startedConnecting(self, connector):
        print "Connection created."

    def clientConnectionFailed(self, connector, reason):
        print "Connection failed:", reason
        connector.connect()
    
    def clientConnectionLost(self, connector, reason):
        print "Connection lost:", reason
        connector.connect()

def init_logging():
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)
    sh = logging.StreamHandler()
    formatter = logging.Formatter(
        '%(asctime)s %(module)s-L%(lineno)d-%(levelname)s: %(message)s')
    sh.setFormatter(formatter)
    logger.addHandler(sh)
    logging.info(
        "Set logging level: %s", logging.getLevelName(logger.getEffectiveLevel()))

# this connects the protocol to a server running on port
def main():
    init_logging()
    for i in xrange(1,3):
        bots = Bots()
        bots.setID(i)
        bots.startNetwork("221.228.207.92", 20900)
    reactor.run()

# this only runs if the module was *not* imported
if __name__ == '__main__':
    main()
