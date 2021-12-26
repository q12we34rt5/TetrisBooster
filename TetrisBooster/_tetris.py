from TetrisBooster.base import TetrisBase
from ._controller import Controller

class Tetris:

    def __init__(self, width = 10, height = 20, tetris = None):
        if type(tetris) == type(None):
            self.base = TetrisBase.create(width, height)
            self.width = width
            self.height = height
        else:
            self.base = TetrisBase.copy(tetris.base)
            self.width = tetris.width
            self.height = tetris.height
        self.control = Controller(self)
    
    def copy(self):
        return Tetris(tetris = self)

    def start(self):
        return TetrisBase.start(self.base)

    def update(self):
        return TetrisBase.update(self.base)
    
    def get_state(self):
        return TetrisBase.get_state(self.base)

    def get_info(self):
        return TetrisBase.get_info(self.base)
    
    def get_board(self):
        return TetrisBase.get_board(self.base)

    def get_shadow(self):
        return TetrisBase.get_shadow(self.base)

    def get_next(self):
        return TetrisBase.get_next(self.base)

    def get_hold(self):
        return TetrisBase.get_hold(self.base)
    
    def get_current(self):
        return TetrisBase.get_current(self.base)
