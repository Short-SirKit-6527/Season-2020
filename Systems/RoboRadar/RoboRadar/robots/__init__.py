#!/usr/bin/python3

import importlib
import os
from abc import abstractmethod


try:
    from RoboRadar.lib import Config, DynamicShape
except ImportError:
    from lib import Config, DynamicShape
config = Config.getConfig()


class Robot(DynamicShape.DynamicShape):

    @classmethod
    @abstractmethod
    def getInfo(self):
        '''This method is called to get info associated with the robot
see the docs (to be made) for more info'''
        pass

    @abstractmethod
    def getShapes(self):
        '''This method is called once per frame.
it should return a list of shapes to be drawn on the screen.'''
        pass


def getRobots():
    robots = {}
    for f in os.listdir(__file__[:-11]):
        if f.endswith(".py") and f != "__init__.py":
            if __package__:
                module = "." + f[:-3]
            else:
                module = f[:-3]
            mod = importlib.import_module(
                module,
                package=__package__
                )
            robots.update(mod.types)
    return robots