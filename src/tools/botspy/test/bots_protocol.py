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
        self._size, self._type, self._crc, self._trans, self._data = struct.unpack(
            'hhiqs', data)

    def type(self):
        return self._type

    def size(self):
        return self._size
