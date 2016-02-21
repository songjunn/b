# coding: utf-8
import struct


class Protocol()

    def __init__():
        self._size = 16
        self._type = 0
        self._crc = 0
        self._trans = 0
        self._data = ""

    def package(type, data):
        self._type = type
        self._data = data
        self._size = len(data)
        self._crc = calculateCrc(data)
        return struct.pack(
            'iiiis', self._size, self._type, self._crc, self._trans, self._data)

    def calculateCrc(data):
        return 97
