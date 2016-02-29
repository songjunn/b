# coding: utf-8
import struct
import binascii


class Protocol(object):
    def __init__(self):
        self._size = 16
        self._type = 0
        self._crc = 0
        self._trans = 0
        self._data = ""

    def package(self, type, data):
        self._type = type
        self._data = data
        self._size += len(data)
        self._crc = binascii.crc32(data)
        s = struct.Struct('hhiq' + str(len(data)) + 's')
        v = (self._size, self._type, self._crc, self._trans, self._data)
        return s.pack(*v)

    def unpackage(self, data):
        # get header
        if len(data) < 16:
            return False
        self._size, self._type, self._crc, self._trans, = struct.unpack(
            'hhiq', data[0:16])
        # get body
        if len(data) < self._size:
            return False
        self._data, = struct.unpack(
            str(self._size - 16) + 's', data[16:self._size])
        return True

    def type(self):
        return self._type

    def size(self):
        return self._size

    def data(self):
        return self._data
