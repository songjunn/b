# -*- coding: utf-8 -*-
import logging
import network
# import gevent
# import gevent.monkey
# gevent.monkey.patch_all()

__client_status__ = ['None', 'Connected', 'Online', 'Offline']


class Bots(network.TCPClient):
    def __init__(self, name):
        self._name = name
        self._status = __client_status__[0]

    def looper(self, time, func):
        self._looper.call_later(time, func)

    def on_connect(self):
        network.TCPClient.on_connect(self)
        self._status = __client_status__[1]
        self.start_test()

    def on_close(self):
        network.TCPClient.on_close(self)
        self._status = __client_status__[3]

    def start_test(self):
        self.test_sendData()

    def test_sendData(self):
        self.sendData(9, "hello python")
        #self.looper(0.5, self.test_sendData)


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


def main():
    init_logging()
    net = network.Network()
    # threads = [gevent.spawn(createBots, i) for i in xrange(2)]
    # gevent.joinall(threads)
    for i in xrange(1):
        bots = Bots(i)
        bots.connect("221.228.207.92", 20900, net.getLooper())
    net.start()

if __name__ == "__main__":
    try:
        main()
    except Exception, ex:
        print "Ocurred Exception: %s" % str(ex)
        quit()
