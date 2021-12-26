from TetrisBooster.base import TetrisBase
from ._tetris import Tetris
import numpy as np

class ActionSpace:

    def __init__(self, n):
        self.n = n

    def sample(self):
        return np.random.choice(self.n)
    
    def __repr__(self):
        return f'ActionSpace({self.n})'


class GymTetrisEnvironment:

    __movs = ['0', '1']
    __rots = ['', '4', '5', '44']
    __sels = __movs + __rots

    def __init__(self, tetris = None, env = None, max_step_pre_block = 10):
        if type(env) == type(None):
            if type(tetris) == type(None):
                tetris = Tetris(10, 20)
            if tetris.width != 10 or tetris.height != 20:
                raise Exception(f'Can only use standard size of tetris board. (e.g. (10, 20), but ({tetris.width} ,{tetris.height}) was given.)')
            self.tetris = tetris
            self.action_space = ActionSpace(len(GymTetrisEnvironment.__sels))
            self.__reward_formula = lambda feats: (7 - feats[1]) / 7 - feats[3] / 2 + 50 * feats[4] - feats[5] - feats[7] / 15 - 50 * (1 - feats[8]) + 1

            self.__current_rewards = 0
            self.__previous_rewards = 0
            self.__return_current_rewards = False
            self.__total_blocks = 0

            self.__last_mov_is_shift = False
            self.__last_mov_is_none = True
            self.__last_mov_is_rot = False
            self.__last_mov_is_valid = True

            self.max_step_pre_block = max_step_pre_block
            self.__remains_step_before_drop = self.max_step_pre_block

            self.__curr_feats = 0

            self.reset()
        else:
            self.tetris = env.tetris.copy()
            self.action_space = env.action_space
            self.__reward_formula = env.__reward_formula

            self.__current_rewards = env.__current_rewards
            self.__previous_rewards = env.__previous_rewards
            self.__return_current_rewards = env.__return_current_rewards
            self.__total_blocks = env.__total_blocks

            self.__last_mov_is_shift = env.__last_mov_is_shift
            self.__last_mov_is_none = env.__last_mov_is_none
            self.__last_mov_is_rot = env.__last_mov_is_rot
            self.__last_mov_is_valid = env.__last_mov_is_valid

            self.max_step_pre_block = env.max_step_pre_block
            self.__remains_step_before_drop = env.__remains_step_before_drop

            self.__curr_feats = env.__curr_feats

    def copy(self):
        return GymTetrisEnvironment(env = self)

    def set_reward_formula(self, new_formula):
        r'''
        set_reward_formula(new_formula)

        Set a customize game reward formula.

        Parameters
        ----------
        new_formula : function
            new_formula = lambda feats: ...
            "feats" is the return value of "get_reward_feature_list" function.

            feats[0]:  line_height               float32_array ; int
            feats[1]:  line_balance_score        float32       ; int
            feats[2]:  max_height                float32       ; int
            feats[3]:  inc_height                float32       ; int
            feats[4]:  inc_clear_lines           float32       ; int
            feats[5]:  last_block_holes_count    float32       ; int
            feats[6]:  last_block_place_height   float32       ; int
            feats[7]:  total_holes_below_top     float32       ; int
            feats[8]:  game_state                float32       ; binary
            feats[9]:  last_block_compactness    float32       ; int
            feats[10]: last_mov_is_shift         float32       ; binary
            feats[11]: last_mov_is_none          float32       ; binary
            feats[12]: last_mov_is_rot           float32       ; binary
            feats[13]: last_mov_is_valid         float32       ; binary

            The default formula was given as
                lambda feats: (7 - feats[1]) / 7 - feats[3] / 2 + 50 * feats[4] - feats[5] - feats[7] / 15 - 50 * (1 - feats[8]) + 1
        '''
        self.__reward_formula = new_formula

    def get_observations(self):
        return np.hstack([
            np.array(TetrisBase.get_learning_features(self.tetris.base), dtype=np.float32),
            self.__curr_feats[4],
            self.__curr_feats[5],
            self.__curr_feats[6],
            self.__curr_feats[9],
            self.__remains_step_before_drop
        ])
    
    def __get_reward(self):
        return self.__reward_formula(self.get_reward_feature_list())

    def get_reward(self):
        r'''
        Returns
        -------
        out : float32
            A game reward calculate by reward formula from the current state of game.

            The default reward formula was given as
                lambda feats: (7 - feats[1]) / 7 - feats[3] / 2 + 50 * feats[4] - feats[5] - feats[7] / 15 - 50 * (1 - feats[8]) + 1
            
            You can set a customize formula by using member function "set_reward_formula".
        
            You can use member function "get_reward_feature_list()" to get "feats".
        '''
        return self.__current_rewards if self.__return_current_rewards else self.__previous_rewards
    
    @staticmethod
    def __get_next_state_reward_feature_list(curr_rw, next_rw):
        line_height = next_rw[:10]
        line_balance_score = np.abs(next_rw[10:19]).sum()
        max_height = next_rw[19]
        inc_height = max_height - curr_rw[19]
        inc_clear_lines = next_rw[20] - curr_rw[20]
        last_block_holes_count = next_rw[21]
        last_block_place_height = next_rw[22]
        total_holes_below_top = next_rw[23]
        game_state = next_rw[24]
        last_block_compactness = next_rw[25]
        return [
            line_height,
            line_balance_score,
            max_height,
            inc_height,
            inc_clear_lines,
            last_block_holes_count,
            last_block_place_height,
            total_holes_below_top,
            game_state,
            last_block_compactness
        ]
    
    def get_reward_feature_list(self):
        r'''
        returns:
            [
                line_height,
                line_balance_score,
                max_height,
                inc_height,
                inc_clear_lines,
                last_block_holes_count,
                last_block_place_height,
                total_holes_below_top,
                game_state,
                last_block_compactness
                last_mov_is_shift,
                last_mov_is_none,
                last_mov_is_rot,
                last_mov_is_valid,
            ]
        '''
        curr_rw = np.array(TetrisBase.get_reward_features(self.tetris.base), dtype=np.float32)
        next_rw = np.array(TetrisBase.get_hard_drop_reward_features(self.tetris.base), dtype=np.float32)
        self.__curr_feats = GymTetrisEnvironment.__get_next_state_reward_feature_list(curr_rw, next_rw)
        return np.hstack([self.__curr_feats, self.__last_mov_is_shift, self.__last_mov_is_none, self.__last_mov_is_rot, self.__last_mov_is_valid])

    def reset(self):
        self.tetris.start()
        self.get_reward()
        self.__current_rewards = self.__get_reward()
        self.__previous_rewards = 0
        self.__return_current_rewards = False
        self.__total_blocks = 0
        self.__last_mov_is_shift = False
        self.__last_mov_is_none = True
        self.__last_mov_is_rot = False
        self.__last_mov_is_valid = True
        self.__remains_step_before_drop = self.max_step_pre_block
        return self.get_observations()
        
    def step(self, action):
        prev_total_blocks = self.__total_blocks
        mov_valid_count = self.tetris.control.sequential_move(self.__sels[action])
        self.__remains_step_before_drop -= 1
        if action < 2:
            self.__last_mov_is_shift = True
            self.__last_mov_is_none = False
            self.__last_mov_is_rot = False
        elif action > 2:
            self.__last_mov_is_shift = False
            self.__last_mov_is_none = False
            self.__last_mov_is_rot = True
        else:
            self.__last_mov_is_shift = False
            self.__last_mov_is_none = True
            self.__last_mov_is_rot = False
        if mov_valid_count > 0 or self.__last_mov_is_none:
            self.__last_mov_is_valid = True
        else:
            self.__last_mov_is_valid = False
        self.__previous_rewards = self.__get_reward()
        if self.__remains_step_before_drop > 0:
            self.tetris.update()
        else:
            self.tetris.control.hard_drop()
        self.__current_rewards = self.__get_reward()
        self.__total_blocks = TetrisBase.get_info(self.tetris.base)[3]
        if self.__total_blocks - prev_total_blocks > 0:
            self.__return_current_rewards = False
            self.__remains_step_before_drop = self.max_step_pre_block
        else:
            self.__return_current_rewards = True
        return self.get_observations(), self.get_reward(), not self.tetris.get_state(), {}
    
    #__block_style = [b" "[0],b" "[0],b" "[0],b"Z"[0],b"L"[0],b"O"[0],b"S"[0],b"I"[0],b"J"[0],b"T"[0]]
    #__top_wall = b"-"[0]
    #__left_wall = b"|"[0]
    #__corn_wall = b"+"[0]
    #__new_line = b"\n"[0]
    def render(self):
        return TetrisBase.dump(self.tetris.base)
        #buf_size = (2 * self.tetris.width + 3) * (self.tetris.height + 2)
        #buf = bytearray(buf_size)
        #index = 0
        #board = self.tetris.get_board()
        ## fill top
        #buf[index] = self.__corn_wall
        #index += 1
        #for i in range(self.tetris.width):
        #    buf[index] = self.__top_wall
        #    index += 1
        #    buf[index] = self.__top_wall
        #    index += 1
        #buf[index] = self.__corn_wall
        #index += 1
        #buf[index] = self.__new_line
        #index += 1
        ## fill board
        #for i in range(self.tetris.height):
        #    buf[index] = self.__left_wall
        #    index += 1
        #    for j in range(self.tetris.width):
        #        try:
        #            buf[index] = self.__block_style[board[i * self.tetris.width + j] >> 4]
        #            index += 1
        #            buf[index] = self.__block_style[board[i * self.tetris.width + j] >> 4]
        #            index += 1
        #        except IndexError as e:
        #            raise IndexError(i, j, board[i * self.tetris.width + j] >> 4)
        #    buf[index] = self.__left_wall
        #    index += 1
        #    buf[index] = self.__new_line
        #    index += 1
        ## fill bottom
        #buf[index] = self.__corn_wall
        #index += 1
        #for i in range(self.tetris.width):
        #    buf[index] = self.__top_wall
        #    index += 1
        #    buf[index] = self.__top_wall
        #    index += 1
        #buf[index] = self.__corn_wall
        #index += 1
        #buf[index] = self.__new_line
        #index += 1
        #return buf.decode()

    def close(self):
        pass

