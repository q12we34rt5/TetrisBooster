from setuptools import setup, Extension, find_packages

tetris_module = Extension(
      name='base.TetrisBase',
      sources=(['src/tetris.cpp', 'src/PyTetris.cpp']),
      include_dirs=['includes', 'src'],
      extra_compile_args=['-Wall', '-O3'],
      language='c++'
)

setup(
      name='TetrisBooster',
      description='Tetris API base on C++.',
      version='11.0.0',
      author='q12we34rt5',
      author_email='luke123132112@gmail.com',
      url='https://github.com/q12we34rt5',
      packages=find_packages(),
      ext_package='TetrisBooster',
      ext_modules=[tetris_module]
)