# coding: utf-8
import logging
import threading
import protocolpack
from twisted.internet import reactor, protocol

class NetClient(protocol.Protocol):   
    def connectionMade(self):
    	print "connection made."
        self.sendData(9, "hello python")
    
    def dataReceived(self, data):
        "As soon as any data is received, write it back."
        print "Received:", data
        # self.transport.loseConnection()
    
    def connectionLost(self, reason):
        print "connection lost:", reason

    def sendData(self, type, data):
        message = protocolpack.Protocol()
        buffer = message.package(type, data)
        self.transport.write(buffer)
        logging.debug("Send message %d size %d: %s", type, len(buffer), data)

class NetFactory(protocol.ClientFactory):
    protocol = NetClient

    def startedConnecting(self, connector):
    	print "Connection created."

    def clientConnectionFailed(self, connector, reason):
        print "Connection failed:", reason
        reactor.stop()
    
    def clientConnectionLost(self, connector, reason):
        print "Connection lost:", reason
        reactor.stop()

class Bots():
    def startNetwork(self, ip, port):
        reactor.connectTCP(ip, port, NetFactory())
        reactor.run()

    def startTestPressure(self):
   		while (True):
   			count = 1

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
    bots = Bots()
    bots.startNetwork("221.228.207.92", 20900)

# this only runs if the module was *not* imported
if __name__ == '__main__':
    main()
