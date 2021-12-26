#define PY_SSIZE_T_CLEAN 
#include <Python.h>
#include <vector>
#include <iostream>
#include <string>
#include "tetris.h"

static void DeleteTetris(PyObject* capsule) {
    Tetris* tetris = (Tetris*)PyCapsule_GetPointer(capsule, "tetris");
    delete tetris;
}

static const char* CreateTetris_doc = "";
static PyObject* CreateTetris(PyObject* self, PyObject* args) {
    int width, height;
    if (!PyArg_ParseTuple(args, "ii", &width, &height))
        return NULL;
    Tetris* tetris = new Tetris(width, height);
    return Py_BuildValue("O", PyCapsule_New(tetris, "tetris", DeleteTetris));
}

static const char* CopyTetris_doc = "";
static PyObject* CopyTetris(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    return Py_BuildValue("O", PyCapsule_New(new Tetris(*tetris), "tetris", DeleteTetris));
}

static const char* TetrisStart_doc = "";
static PyObject* TetrisStart(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    tetris->start();
    Py_INCREF(Py_None);
    return Py_None;
}

static const char* TetrisUpdate_doc = "";
static PyObject* TetrisUpdate(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    int success = (int)tetris->update();
    if (success)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static const char* TetrisGetState_doc = "";
static PyObject* TetrisGetState(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    if (tetris->game_state)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static const char* TetrisGetInfo_doc = "[line_heights, max_height, total_clear_lines, total_blocks]";
static PyObject* TetrisGetInfo(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    int line_height_size = tetris->board.width;
    char* line_height = new char[line_height_size];
    for (int i = 0; i < line_height_size; i++)
        line_height[i] = (char)tetris->info.line_height[i];
    PyObject* obj = Py_BuildValue("[y#,i,i,i]", line_height, line_height_size, tetris->info.max_height, tetris->info.clear_count, tetris->info.total_blocks);
    delete[] line_height;
    return obj;
}

static const char* TetrisGetBoard_doc = "";
static PyObject* TetrisGetBoard(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    // paste tetris board
    const int board_size = tetris->board.width * tetris->board.height;
    char *board = new char[board_size];
    for (int i = 0; i < tetris->board.height; i++)
        for (int j = 0; j < tetris->board.width; j++)
            board[i * tetris->board.width + j] = tetris->board.data[i][j];
    PyObject* obj = Py_BuildValue("y#", board, board_size);
    delete[] board;
    return obj;
}

static const char* TetrisGetShadow_doc = "";
static PyObject* TetrisGetShadow(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    // paste tetris shadow
    const int shadow_size = tetris->board.width * tetris->board.height;
    char *shadow = new char[shadow_size];
    for (int i = 0; i < tetris->board.height; i++)
        for (int j = 0; j < tetris->board.width; j++)
            shadow[i * tetris->board.width + j] = tetris->board.shadow[i][j];
    PyObject* obj = Py_BuildValue("y#", shadow, shadow_size);
    delete[] shadow;
    return obj;
}

static const char* TetrisGetNext_doc = "";
static PyObject* TetrisGetNext(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    // paste tetris next
    char next[5];
    for (int i = 0; i < 5; i++)
        next[i] = (char)tetris->next.blocks[i];
    return Py_BuildValue("y#", next, 5);
}

static const char* TetrisGetHold_doc = "";
static PyObject* TetrisGetHold(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    return Py_BuildValue("c", (char)tetris->hold.block);
}

static const char* TetrisGetCurrent_doc = "byte(4): [x, y, type, rot]";
static PyObject* TetrisGetCurrent(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    auto& cur = tetris->controller.current;
    const char current[4] = { (char)cur.x, (char)cur.y, (char)cur.type, (char)cur.rot };
    return Py_BuildValue("y#", current, 4);
}

static const char* TetrisMoveLeft_doc = "";
static PyObject* TetrisMoveLeft(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    int success = (int)tetris->controller.moveLeft();
    if (success)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static const char* TetrisMoveRigh_doc = "";
static PyObject* TetrisMoveRigh(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    int success = (int)tetris->controller.moveRigh();
    if (success)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static const char* TetrisSoftDrop_doc = "";
static PyObject* TetrisSoftDrop(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    int success = (int)tetris->controller.softDrop();
    if (success)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static const char* TetrisHardDrop_doc = "";
static PyObject* TetrisHardDrop(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    int success = (int)tetris->controller.hardDrop();
    if (success)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static const char* TetrisRotateLeft_doc = "";
static PyObject* TetrisRotateLeft(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    int success = (int)tetris->controller.rotateLeft();
    if (success)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static const char* TetrisRotateRight_doc = "";
static PyObject* TetrisRotateRight(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    int success = (int)tetris->controller.rotateRight();
    if (success)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static const char* TetrisHold_doc = "";
static PyObject* TetrisHold(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    int success = (int)tetris->controller.hold();
    if (success)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static const char* TetrisSequentialMove_doc = "";
static PyObject* TetrisSequentialMove(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    const char* move_seq;
    if (!PyArg_ParseTuple(args, "Os", &_tetris, &move_seq))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    int success_count = tetris->controller.seqMove(move_seq);
    return Py_BuildValue("i", success_count);
}

static const char* TetrisGetLearningFeatures_doc = "[board.width]: each line height\n\
[board.width]: diff between each line height\n\
[7]: current block\n\
[7]: next block";
static PyObject* TetrisGetLearningFeatures(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    auto feats = tetris->features.get_learning_features();
    const int N = feats.size();
    PyObject* feats_list = PyList_New(N);
    for (int i = 0; i < N; i++) {
        PyObject* python_int = Py_BuildValue("i", feats[i]);
        PyList_SetItem(feats_list, i, python_int);
    }
    return Py_BuildValue("O", feats_list);
}

static const char* TetrisGetRewardFeatures_doc = "[board.width]: each line height\n\
[1]: max line height\n\
[1]: total clear lines count\n\
[1]: last block holes count\n\
[1]: last block place height\n\
[1]: total number of holes below each line top\n\
[1]: game state";
static PyObject* TetrisGetRewardFeatures(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    auto feats = tetris->features.get_reward_features();
    const int N = feats.size();
    PyObject* feats_list = PyList_New(N);
    for (int i = 0; i < N; i++) {
        PyObject* python_int = Py_BuildValue("i", feats[i]);
        PyList_SetItem(feats_list, i, python_int);
    }
    return Py_BuildValue("O", feats_list);
}

static const char* TetrisGetHardDropRewardFeatures_doc = "";
static PyObject* TetrisGetHardDropRewardFeatures(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    auto feats = tetris->features.get_hard_drop_reward_features();
    const int N = feats.size();
    PyObject* feats_list = PyList_New(N);
    for (int i = 0; i < N; i++) {
        PyObject* python_int = Py_BuildValue("i", feats[i]);
        PyList_SetItem(feats_list, i, python_int);
    }
    return Py_BuildValue("O", feats_list);
}

static const char* TetrisDump_doc = "";
static PyObject* TetrisDump(PyObject* self, PyObject* args) {
    PyObject* _tetris;
    if (!PyArg_ParseTuple(args, "O", &_tetris))
        return NULL;
    Tetris* tetris;
    if (!(tetris = (Tetris*)PyCapsule_GetPointer(_tetris, "tetris")))
        return NULL;
    std::string str = tetris->dump();
    return Py_BuildValue("s", str.c_str());
}

// Module method table
static PyMethodDef tetris_methods[] = {
    { "create"                        ,(PyCFunction)CreateTetris,                    METH_VARARGS, CreateTetris_doc                    },
    { "copy"                          ,(PyCFunction)CopyTetris,                      METH_VARARGS, CopyTetris_doc                      },
    { "start"                         ,(PyCFunction)TetrisStart,                     METH_VARARGS, TetrisStart_doc                     },
    { "update"                        ,(PyCFunction)TetrisUpdate,                    METH_VARARGS, TetrisUpdate_doc                    },
    { "get_state"                     ,(PyCFunction)TetrisGetState,                  METH_VARARGS, TetrisGetState_doc                  },
    { "get_info"                      ,(PyCFunction)TetrisGetInfo,                   METH_VARARGS, TetrisGetInfo_doc                   },
    { "get_board"                     ,(PyCFunction)TetrisGetBoard,                  METH_VARARGS, TetrisGetBoard_doc                  },
    { "get_shadow"                    ,(PyCFunction)TetrisGetShadow,                 METH_VARARGS, TetrisGetShadow_doc                 },
    { "get_next"                      ,(PyCFunction)TetrisGetNext,                   METH_VARARGS, TetrisGetNext_doc                   },
    { "get_hold"                      ,(PyCFunction)TetrisGetHold,                   METH_VARARGS, TetrisGetHold_doc                   },
    { "get_current"                   ,(PyCFunction)TetrisGetCurrent,                METH_VARARGS, TetrisGetCurrent_doc                },
    { "move_left"                     ,(PyCFunction)TetrisMoveLeft,                  METH_VARARGS, TetrisMoveLeft_doc                  },
    { "move_righ"                     ,(PyCFunction)TetrisMoveRigh,                  METH_VARARGS, TetrisMoveRigh_doc                  },
    { "soft_drop"                     ,(PyCFunction)TetrisSoftDrop,                  METH_VARARGS, TetrisSoftDrop_doc                  },
    { "hard_drop"                     ,(PyCFunction)TetrisHardDrop,                  METH_VARARGS, TetrisHardDrop_doc                  },
    { "rotate_left"                   ,(PyCFunction)TetrisRotateLeft,                METH_VARARGS, TetrisRotateLeft_doc                },
    { "rotate_right"                  ,(PyCFunction)TetrisRotateRight,               METH_VARARGS, TetrisRotateRight_doc               },
    { "hold"                          ,(PyCFunction)TetrisHold,                      METH_VARARGS, TetrisHold_doc                      },
    { "sequential_move"               ,(PyCFunction)TetrisSequentialMove,            METH_VARARGS, TetrisSequentialMove_doc            },
    { "get_learning_features"         ,(PyCFunction)TetrisGetLearningFeatures,       METH_VARARGS, TetrisGetLearningFeatures_doc       },
    { "get_reward_features"           ,(PyCFunction)TetrisGetRewardFeatures,         METH_VARARGS, TetrisGetRewardFeatures_doc         },
    { "get_hard_drop_reward_features" ,(PyCFunction)TetrisGetHardDropRewardFeatures, METH_VARARGS, TetrisGetHardDropRewardFeatures_doc },
    { "dump"                          ,(PyCFunction)TetrisDump,                      METH_VARARGS, TetrisDump_doc                      },
    { NULL,NULL,0,NULL }
};

// Define module structure
static struct PyModuleDef tetris_module = {
    PyModuleDef_HEAD_INIT,
    "TetrisBase",                             // module name
    "A C++ tetris API extension for Python.", // doc
    -1,                                       // memory option
    tetris_methods                            // module method table
};

// Module initialization method
PyMODINIT_FUNC PyInit_TetrisBase() {
    return PyModule_Create(&tetris_module);
}
