import os
from doit.tools import create_folder
from build_sys.doit_helpers import arduino_utils
from build_sys.doit_helpers import file_utils
from build_sys.doit_helpers import gcc_utils

# ---------------------------------------------------------------------
# Build settings

PROJECT_NAME = 'goggles'

BUILD_DIR = 'build'

OBJ_DIR = os.path.join(BUILD_DIR, 'obj')

ARDUINO_ROOT = 'D:\\programs_win\\arduino-1.0.5'

ARDUINO_ENV = arduino_utils.ArduinoEnv(ARDUINO_ROOT, BUILD_DIR)

SOURCE_DIRS = [
    'src',
    os.path.join(ARDUINO_ENV.libraries_path, 'Adafruit_NeoPixel')
]

INCLUDE_DIRS = [
    'src',
    os.path.join(ARDUINO_ENV.libraries_path, 'Adafruit_NeoPixel')
]
INCLUDE_DIRS += ARDUINO_ENV.cincludes


# ---------------------------------------------------------------------
# data

class BuildData:
    def __init__(self):
        self.c_sources = []
        self.cpp_sources = []
        for path in SOURCE_DIRS:
            self.c_sources += file_utils.find(path, '*.c')
            self.cpp_sources += file_utils.find(path, '*.cpp')

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
                                         defs=ARDUINO_ENV.cdefs,
                                         includes=INCLUDE_DIRS,
                                         flags=ARDUINO_ENV.cflags)


def get_cpp_compile_command_str(source, obj):
    return gcc_utils.get_compile_cmd_str(source, obj,
                                         compiler=ARDUINO_ENV.cpp_compiler,
                                         defs=ARDUINO_ENV.cppdefs,
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


if __name__ == '__main__':
    ARDUINO_ENV.get_build_core_tasks()
    print ARDUINO_ENV
