# coding: utf-8
import threading
import message
from twisted.internet import reactor, protocol

class NetClient(protocol.Protocol):   
    def connectionMade(self):
    	print "connection made."
        #self.transport.write("hello, world!")
    
    def dataReceived(self, data):
        "As soon as any data is received, write it back."
        print "Received:", data
        self.transport.loseConnection()
    
    def connectionLost(self, reason):
        print "connection lost:", reason

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

# this connects the protocol to a server running on port
def main():
    bots = Bots()
    bots.startNetwork("192.168.6.47", 20900)

# this only runs if the module was *not* imported
if __name__ == '__main__':
    main()
