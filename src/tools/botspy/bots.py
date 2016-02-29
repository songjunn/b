# -*- coding: utf-8 -*-
import logging
import network
import gevent.monkey
from threading import Timer
import time
gevent.monkey.patch_all()

__client_status__ = ['None', 'Connected', 'Online', 'Offline']


class Bots(network.NetClient):
    def __init__(self, id):
        super(Bots, self).__init__()
        self._id = id
        self._status = __client_status__[0]

    def connect_cb(self):
        logging.info("Bots %d connect success...", self._id)
        self._status = __client_status__[1]
        self.schedule(1, self.test_sendData)

    def shutdown_cb(self):
        logging.info("Bots %d shutdown...", self._id)
        self._status = __client_status__[3]

    def receive_cb(self, message):
        pass

    def schedule(self, interval, func):
        def looper():
            while True:
                func()
                gevent.sleep(interval)
        gevent.spawn(looper)

    def test_sendData(self):
        data = "hello python! I'm bots %d" % self._id
        self.sendData(9, data)


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

    def createBots(i, addr, port):
        bots = Bots(i)
        bots.connect(addr, port)
    threads = [
        gevent.spawn(createBots, i, "221.228.207.92", 20900) for i in xrange(2)]
    gevent.joinall(threads)

    while True:
        gevent.sleep(60)

if __name__ == "__main__":
    try:
        main()
    except Exception, ex:
        logging.debug("Ocurred Exception: %s" % str(ex))
        quit()
