import json
import os


_conf = {}


def findConfig():
    if os.path.exists(os.getcwd() + os.sep + "RoboRadarConfig.json"):
        return os.path.exists(os.getcwd() + os.sep + "RoboRadarConfig.json")
    print(__file__[:-14] + os.sep + "RoboRadarConfig.json")
    return __file__[:-14] + os.sep + "RoboRadarConfig.json"


def loadConfig(filePath=None):
    global _conf
    if filePath is None:
        filePath = findConfig()
    with open(filePath) as f:
        _conf = json.load(f)


def getConfig():
    return _conf
