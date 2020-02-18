import json
import os


_conf = {}


def findConfig():
    if os.path.exists(os.getcwd() + os.sep + "RoboRadarConfig.json"):
        return os.path.exists(os.getcwd() + os.sep + "RoboRadarConfig.json")
    return __file__[:-14] + os.sep + "RoboRadarConfig.json"


def loadConfig(filePath=None):
    global _conf
    if filePath is None:
        filePath = findConfig()
    with open(filePath) as f:
        _conf = json.load(f)
        if _conf["ROBOT"]["IP_ADDRESS"] is None:
            _conf["ROBOT"]["IP_ADDRESS_SET"] = False
            if _conf["TEAM"]["NUMBER"] != 0:
                setNtAddress(_conf["TEAM"]["NUMBER"])
        else:
            _conf["ROBOT"]["IP_ADDRESS_SET"] = True


def setNtAddress(tn):
    if not _conf["ROBOT"]["IP_ADDRESS_SET"]:
        tn = str(tn).zfill(4)
        tn = (tn[0:2], tn[2:4])
        iaf = _conf["ROBOT"]["IP_ADDRESS_FORMAT"]
        _conf["ROBOT"]["IP_ADDRESS"] = iaf.format(tn[0], tn[1])


def getConfig():
    return _conf
