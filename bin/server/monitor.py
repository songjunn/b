#!/usr/bin/env python

import socket
import os
import time,curses,threading
import random
import signal
from struct import *
import sys

run_flag = True
message_quit = "press any key,quit"
sock_path = "./unix_sock/unix_gate.sock"

class UnixSock:
    def __init__(self, svr = sock_path):
        self.fd = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.fd.connect(svr)
        self.fd.settimeout(5)
    def recv(self, size):
        return self.fd.recv(size)
    def close(self):
        self.fd.close()

def monitor(stdscreen):
    s = UnixSock(sock_path)
    r = random.Random()
    stdscreen.clear()
    stdscreen.refresh()

    curses.init_pair(1, curses.COLOR_CYAN, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_BLUE, curses.COLOR_BLACK)

    threading.Thread(target = thread_monitor, args = (stdscreen, ),
            name = 'thread').start()
    global run_flag
    while run_flag:
        try:
            (len, n) = unpack("ii", s.recv(8))
            if len == None or len == 0: continue
            data = s.recv(int(len)-4)
        except :
            stdscreen.addstr(1, 1, "exception")
            stdscreen.refresh()
            time.sleep(2)
            s.close()
            return 
        stdscreen.addstr(n, 0, data, curses.color_pair(1))
        #stdscreen.move(0, 10)
        stdscreen.move(stdscreen.getmaxyx()[0] - 5, 20)
        stdscreen.refresh()

def thread_monitor(screen):
    screen.addstr(screen.getmaxyx()[0] - 6, 1, sock_path, curses.color_pair(2))
    screen.addstr(screen.getmaxyx()[0] - 5, 1, message_quit, curses.color_pair(2))
    global run_flag
    ch = screen.getch()
    while ch != 10 and ch != 113:
        ch = screen.getch()
    run_flag = False
    
def sig_func():
    pass

def init():
    signal.signal(signal.SIGINT, signal.SIG_IGN)
        
if __name__ == "__main__":
    if len(sys.argv) >= 2:
        sock_path = sys.argv[1]
    init()
    curses.wrapper(monitor)
    #test()

