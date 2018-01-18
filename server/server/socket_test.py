# coding: utf-8

import os
import sys
import time
import socket
import threading

"""
time python3 socket_test.py
"""

host = 'localhost'
port = 3000
buff_size = 1024


def log(*args, **kwargs):
    print(*args, **kwargs)


def request():
    for i in range(100):
        cmd = 'curl localhost:3000'
        os.system(cmd)
        # time.sleep(0.5)
        # client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # client.connect((host, port))
        # # s = 'hello im client {}\n'.format(i)
        # # client.send(s.encode('utf-8'))
        # client.close()


def main():
    start = time.time()
    ts = []
    for i in range(50):
        t = threading.Thread(target=request, name='LoopThread')
        ts.append(t)
        t.start()

    for t in ts:
        t.join()

    end = time.time()
    log('time', end - start)

if __name__ == '__main__':
    main()