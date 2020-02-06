import importlib, os
from abc import ABC, abstractmethod


try:
    from RoboRadar import CONFIG
    from RoboRadar.lib.DynamicShapeController import DynamicShapeController
except:
    from __main__ import CONFIG
    from __main__ import DynamicShapeController

class Robot(DynamicShapeController):

    @classmethod
    @abstractmethod
    def getInfo(self):
        '''This method is called to get info associated with the robot
see the docs (to be made) for more info'''
        pass
    
    @abstractmethod
    def getShapes(self):
        '''This method is called once per frame and must return a list of shapes to be drawn on the screen'''
        pass

    def draw(self):
        self.getShapes()

robots = {}
for file in os.listdir(__file__[:-11]):
    if file.endswith(".py") and file != "__init__.py":
        if __package__:
            module = "." + file[:-3]
        else:
            module = file[:-3]
        robotIndex = len(robots)
        robots[file[:-3]] = importlib.import_module(module, package=__package__)
