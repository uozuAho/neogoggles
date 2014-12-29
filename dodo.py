import os
import sys

from doit.tools import create_folder

sys.path.append('dependencies/doit_helpers')
from doit_helpers.arduino import env as arduino_env
from doit_helpers import file_utils
from doit_helpers import gcc_utils

DOIT_CONFIG = {'default_tasks': ['build_exe']}

# ---------------------------------------------------------------------
# Build settings

PROJECT_NAME = 'goggles'

BUILD_DIR = 'build'

OBJ_DIR = os.path.join(BUILD_DIR, 'obj')

ARDUINO_ROOT = '/home/uozu/Downloads/arduino-1.0.6'

# ARDUINO_HARDWARE = 'uno'
ARDUINO_HARDWARE = 'pro_mini_8mhz'

ARDUINO_ENV = arduino_env.ArduinoEnv(PROJECT_NAME, ARDUINO_ROOT, BUILD_DIR, ARDUINO_HARDWARE)

SERIAL_PORT = '/dev/ttyUSB0'

SOURCE_DIRS = [
    'src',
    'src/themes',
    os.path.join('dependencies', 'neopixel')
]

INCLUDE_DIRS = [
    'src',
    os.path.join('dependencies', 'neopixel')
]
INCLUDE_DIRS += ARDUINO_ENV.cincludes

if ARDUINO_HARDWARE == 'uno':
    C_DEFS = ARDUINO_ENV.cdefs + ['UNO_HARDWARE']
    CPP_DEFS = ARDUINO_ENV.cdefs + ['UNO_HARDWARE']
elif ARDUINO_HARDWARE == 'pro_mini_8mhz':
    C_DEFS = ARDUINO_ENV.cdefs + ['PRO_MINI_8MHZ_HARDWARE']
    CPP_DEFS = ARDUINO_ENV.cdefs + ['PRO_MINI_8MHZ_HARDWARE']


# ---------------------------------------------------------------------
# data

class BuildData:
    def __init__(self):
        self.c_sources = []
        self.cpp_sources = []
        for path in SOURCE_DIRS:
            self.c_sources += file_utils.find(path, '*.c')
            self.cpp_sources += file_utils.find(path, ['*.cpp', '*.ino'])

        self.objs = [self.get_obj_path(x) for x in self.c_sources + self.cpp_sources]
        self.objs += [ARDUINO_ENV.core_lib_output_path]

        self.deps = gcc_utils.get_dependency_dict(BUILD_DIR)

    def get_obj_path(self, source_path):
        return os.path.join(OBJ_DIR, os.path.basename(source_path) + '.o')

    def get_dep_path(self, source_path):
        return os.path.join(OBJ_DIR, os.path.basename(source_path) + '.d')

build_data = BuildData()


# ---------------------------------------------------------------------
# utilities

def get_source_dependencies(source):
    obj = build_data.get_obj_path(source)
    if obj in build_data.deps:
        return build_data.deps[obj]
    else:
        return [source]


def get_c_compile_command_str(source, obj):
    return gcc_utils.get_compile_cmd_str(source, obj,
                                         compiler=ARDUINO_ENV.c_compiler,
                                         defs=C_DEFS,
                                         includes=INCLUDE_DIRS,
                                         flags=ARDUINO_ENV.cflags)


def get_cpp_compile_command_str(source, obj):
    return gcc_utils.get_compile_cmd_str(source, obj,
                                         compiler=ARDUINO_ENV.cpp_compiler,
                                         defs=CPP_DEFS,
                                         includes=INCLUDE_DIRS,
                                         flags=ARDUINO_ENV.cppflags)


# ---------------------------------------------------------------------
# tasks

def task_build_arduino_core():
    for task in ARDUINO_ENV.get_build_core_tasks():
        yield task


def task_compile_c():
    for source in build_data.c_sources:
        obj = build_data.get_obj_path(source)
        dep = build_data.get_dep_path(source)
        yield {
            'name': obj,
            'actions': [(create_folder, [os.path.dirname(obj)]),
                        get_c_compile_command_str(source, obj)],
            'targets': [obj, dep],
            'file_dep': get_source_dependencies(source),
            'clean': True
        }


def task_compile_cpp():
    for source in build_data.cpp_sources:
        obj = build_data.get_obj_path(source)
        dep = build_data.get_dep_path(source)
        yield {
            'name': obj,
            'actions': [(create_folder, [os.path.dirname(obj)]),
                        get_cpp_compile_command_str(source, obj)],
            'targets': [obj, dep],
            'file_dep': get_source_dependencies(source),
            'clean': True
        }


def task_build_exe():
    for task in ARDUINO_ENV.get_build_exe_tasks(PROJECT_NAME, build_data.objs):
        yield task


def task_upload():
    return ARDUINO_ENV.get_upload_task(SERIAL_PORT)


if __name__ == '__main__':
    ARDUINO_ENV.get_build_core_tasks()
    print ARDUINO_ENV
