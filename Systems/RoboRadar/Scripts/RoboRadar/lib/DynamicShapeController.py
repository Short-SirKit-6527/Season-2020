from abc import ABC, abstractmethod

class DynamicShapeController(ABC):
    @abstractmethod
    def draw(self):
        pass
