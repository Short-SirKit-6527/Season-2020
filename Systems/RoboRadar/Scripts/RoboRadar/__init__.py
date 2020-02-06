# Force program to run as a package/module. Recommended for compatibility.
#This is designed to run as a module, but this can cause issues when developing in IDLE.
#Non-module mode is of low priority, and may not work AT ALL!!!
# Default: True
FORCE_RUN_AS_MODULE = True

if (FORCE_RUN_AS_MODULE) and __package__ == None:
    import os
    print("Not running as module, restarting. Please run using 'py -m RoboRadar.__init__'")
    existingcwd = os.getcwd()
    os.chdir(os.path.dirname(os.path.realpath(__file__))[:-10])
    os.system("py -m RoboRadar.__init__")
    os.chdir(existingcwd)
    exit()

    
import os, sys, re
from enum import Enum, auto

'''
============================

- DEFINITIONS -

============================

These are standard definitions for the program to use.
Do not mess with these unless you know what you are doing!
'''

class VideoEngines(Enum):
    native = auto()
    numpy = auto()
    pygame = auto()

VERSION = "1.0.0"

'''
============================

- CONFIG -

============================

Default configuration stored alongside the program.
These are the settings that will be loaded into an .ini file the
first time RoboRadar is run.
'''

# Number of FPS to run the screen at. Recommended 30, 60, or the refresh rate of the monitor
# Default: 60
FPS = 60

# Team number of FRC Team
# Default: 0
TEAM_NUMBER = 0

# NetworkTables server address, leave this unchanged to use the team number. Each {}{} will be filled with part of the team number 
# Default: "10.{}.{}.2"
SERVER_ADDRESS = "10.{}.{}.2"

# Set the starting screen width and height
# Default: 480, 640
INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT = 480, 640

# Enable/disable antialiased polygons and lines
# Default: True
ANTIALIASING = True

# Enable/disable filled polygons (gives everything a wireframe look. NOT recommended)
# Default: True
FILLED_POLYGONS = True

# Set video engine for graphics
# Default: pygame
VIDEO_ENGINE = VideoEngines.pygame

# Set starting field
# Default: "FRC_2020"
DEFAULT_FIELD = "FRC_2020"

#Dont mess with this. It WILL cause syncing issues
CONFIG = {
    "FPS": FPS,
    "TEAM_NUMBER": TEAM_NUMBER,
    "SERVER_ADDRESS": SERVER_ADDRESS,
    "INITIAL_SCREEN_WIDTH": INITIAL_SCREEN_WIDTH,
    "INITIAL_SCREEN_HEIGHT": INITIAL_SCREEN_HEIGHT,
    "ANTIALIASING": ANTIALIASING,
    "FILLED_POLYGONS": FILLED_POLYGONS,
    "VIDEO_ENGINE": VIDEO_ENGINE,
    "DEFAULT_FIELD": DEFAULT_FIELD
    }

from RoboRadar.fields import fields, fieldFiles, fieldNames, fieldThemes
from RoboRadar.robots import robots

if VIDEO_ENGINE is VideoEngines.pygame:
    os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = "hide"
    import pygame
    import pygame.gfxdraw
    from pygame.locals import *
else:
    raise ValueError

def startIndependent():
    pygame.init()

    screen = pygame.display.set_mode((INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT), pygame.RESIZABLE)
    pygame.display.set_caption("RoboRadar v" + VERSION + " - Team " + str(TEAM_NUMBER))
    clock = pygame.time.Clock()

    r = Radar((INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT))
    r.loadField(DEFAULT_FIELD)
    
    while True:
        screen.fill((249,249,249))

        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                sys.exit()
            if event.type == VIDEORESIZE:
                r.resize(event.size)
                surface = pygame.display.set_mode((event.w, event.h), pygame.RESIZABLE)
      
        # Update.

        screen.blit(r.pygame_render(), (0, 0))
      
        # Draw.
        pygame.display.flip()
        clock.tick(FPS)

class Radar:
    def __init__(self, dimensions, interface=VIDEO_ENGINE, *args, **kwargs):
        self.dimensions = dimensions
        self.fieldIndex = None
        self.field = None
        if interface == VideoEngines.pygame:
            self._init_pygame(*args, **kwargs)

    def loadField(self, search, *args, **kwargs):
        if isinstance(search, int) and int(search) < len(fields) and int(search) >= 0:
            self.fieldIndex = search
        else:
            self.fieldIndex = fieldFiles.get(search, None)
        if self.fieldIndex is None:
            self.fieldIndex = fieldNames.get(search, None)
        if self.fieldIndex is None:
            self.fieldIndex = fieldThemes.get(search, None)
        if self.fieldIndex is None:
            raise ValueError
        self.field = fields[self.fieldIndex]
        self._loadField_engineSpecific()

    def _init_pygame(self, *args, **kwargs):
        self._loadField_engineSpecific = self._loadField_pygame
        self._visibleSurface = pygame.Surface(self.dimensions)
        self._resize_engineSpecific = self._resize_pygame

    def _loadField_pygame(self):
        self._resize_pygame()

    def resize(self, dimensions):
        self.dimensions = dimensions
        self._resize_engineSpecific()
        
    def _resize_pygame(self):
        self._visibleSurface = pygame.Surface(self.dimensions)
        self._visibleSurface.fill((0,255,0))
        if self.field.Data["width"] / self.field.Data["height"] <= self.dimensions[0] / self.dimensions[1]:
            height = self.dimensions[1]
            width  = self.field.Data["width"] / self.field.Data["height"] * height
        else:
            width = self.dimensions[0]
            height = self.field.Data["height"] / self.field.Data["width"] * width
        self._offset = (int((self.dimensions[0] - width) / 2), int((self.dimensions[1] - height) / 2))
        self._staticHeight, self._staticWidth = int(height), int(width)
        self._staticSurface = pygame.Surface(( self._staticWidth,  self._staticHeight))
        for shape in self.field.Data["static-shapes"]:
            if shape["type"] == "polygon":
                p = self._pygame_convertCoordinateSpace(shape["points"])
                if "filled" in shape["style"]:
                    pygame.gfxdraw.filled_polygon(self._staticSurface, p, shape["color"])
                if "outline" in shape["style"]:
                    pygame.gfxdraw.polygon(self._staticSurface, p, shape["color"])
                if "aa" in shape["style"]:
                    pygame.gfxdraw.aapolygon(self._staticSurface, p, shape["color"])
                    
            #print(shape)

    def _pygame_convertCoordinateSpace(self, points):
        p = []
        for point in points:
            p.append((((point[0]+self.field.Data["center"][0])/self.field.Data["width"]*self._staticWidth), ((point[1]+self.field.Data["center"][1])/self.field.Data["height"]*self._staticHeight)))
        return p

    def pygame_render(self):
        self._visibleSurface.fill((0, 255, 0))
        self._visibleSurface.blit(self._staticSurface, self._offset)
        return self._visibleSurface
    
if __name__ == "__main__":
    if TEAM_NUMBER == 0:
        num = input("TEAM_NUMBER option is set to 0, please enter a team number to use temporarily, or change it in __init__.py\nTeam Number: ")
        try:
            TEAM_NUMBER = float(num.strip())
        except ValueError:
            print("No TEAM_NUMBER entered, or non-numeric input given. Exiting with status code 1")
            exit(1)
    if TEAM_NUMBER <= 0:
        print("Invalid TEAM_NUMBER. TEAM_NUMBER must be greater than 0. Exiting with status code 1")
        exit(1)
    if int(TEAM_NUMBER) != TEAM_NUMBER:
        print("Invalid TEAM_NUMBER. TEAM_NUMBER must be an integer. Exiting with status code 1")
        exit(1)
    if not isinstance(TEAM_NUMBER, int):
        TEAM_NUMBER = int(TEAM_NUMBER)
    if len(str(TEAM_NUMBER)) > 4:
        print("Invalid TEAM_NUMBER. TEAM_NUMBER must be 4 digits or less. Exiting with status code 1")
        exit(1)
    CONFIG["TEAM_NUMBER"] = TEAM_NUMBER
    CONFIG["SERVER_ADDRESS"] = SERVER_ADDRESS.format(*re.findall('..', format(TEAM_NUMBER, "04")))
    startIndependent()
else:
    CONFIG["SERVER_ADDRESS"] = SERVER_ADDRESS.format(*re.findall('..', format(TEAM_NUMBER, "04")))
