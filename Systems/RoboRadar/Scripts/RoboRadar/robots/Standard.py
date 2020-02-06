from networktables import NetworkTables
from . import Robot, CONFIG

class NetworkTableBot(Robot):

    def __init__(self, *args, **kwargs):
        NetworkTables.initialize(server=kwargs.get("server", CONFIG["SERVER_ADDRESS"]))
        self.nt = NetworkTables.getTable("RoboRadar")

    @property
    def x(self):
        return self.nt.getNumber("posX")

    @property
    def y(self):
        return self.nt.getNumber("posY")

    @property
    def r(self):
        return self.nt.getNumber("posR")
        

class BoxBot(NetworkTableBot):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    @classmethod
    def getInfo(cls):
        data = {
            "name": "BoxBot",
            "units": "inches",
            }
        return data

    def getShapes(self):
        shapes = [
            {
                "name": "test",
                "type": "polygon",
                "style": ("filled", "aa"),
                "color": (0,255,0),
                "layer": 0,
                "points": [
                    (-24,24),
                    (24,24),
                    (24,-24),
                    (-24,-24)
                    ]
                }
            ]

print(BoxBot())

types = {
    "BoxBot": BoxBot
    }
