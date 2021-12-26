from TetrisBooster.base import TetrisBase

class Controller:

    def __init__(self, tetris):
        self.tetris = tetris

    def move_left(self):
        return TetrisBase.move_left(self.tetris.base)

    def move_right(self):
        return TetrisBase.move_righ(self.tetris.base)

    def soft_drop(self):
        return TetrisBase.soft_drop(self.tetris.base)

    def hard_drop(self):
        return TetrisBase.hard_drop(self.tetris.base)

    def rotate_left(self):
        return TetrisBase.rotate_left(self.tetris.base)

    def rotate_right(self):
        return TetrisBase.rotate_right(self.tetris.base)

    def hold(self):
        return TetrisBase.hold(self.tetris.base)
    
    def sequential_move(self, move_seq):
        return TetrisBase.sequential_move(self.tetris.base, move_seq)
    
    function_list = [move_left, move_right, soft_drop, hard_drop, rotate_left, rotate_right, hold]
    
    def index_call(self, index):
        return self.function_list[index](self)
