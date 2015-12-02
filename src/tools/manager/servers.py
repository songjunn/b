#coding: utf-8
import json

def loadData(filepath):
    f = file(filepath)
    data = json.load(f)
    f.close()
    return data

def getServer(id, data):
    for ele in data:
        obj = data[ele]
        if obj["id"] == id:
            return obj["ip"], obj["port"], obj["usr"], obj["psd"]

