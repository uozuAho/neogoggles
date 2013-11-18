import os
from doit.tools import create_folder
from build_sys.doit_helpers import arduino_utils
from build_sys.doit_helpers import file_utils
from build_sys.doit_helpers import gcc_utils

# ---------------------------------------------------------------------
# Build settings

PROJECT_NAME = 'goggles'

BUILD_DIR = 'build'

ARDUINO_ROOT = 'D:\\programs_win\\arduino-1.0.5'

ARDUINO_ENV = arduino_utils.ArduinoEnv(ARDUINO_ROOT, BUILD_DIR)

SOURCE_DIRS = [
    'src',
    os.path.join(ARDUINO_ENV.libs_path, 'Adafruit_NeoPixel')
]


# ---------------------------------------------------------------------
# data

class BuildData:

    """ This data is generated as doit tasks are generated """

    c_sources = None
    cpp_sources = None
    objs = None
    deps = None

    def get_c_sources(self):
        if self.c_sources is None:
            self.c_sources = []
            for path in SOURCE_DIRS:
                self.c_sources += file_utils.find(path, '*.c')
        return self.c_sources

    def get_cpp_sources(self):
        if self.cpp_sources is None:
            self.cpp_sources = []
            for path in SOURCE_DIRS:
                self.cpp_sources += file_utils.find(path, ['*.cpp', '*.ino'])
        return self.cpp_sources

    def get_objs(self):
        if self.objs is None:
            sources = self.get_c_sources() + self.get_cpp_sources()
            self.objs = [get_obj_path(x) for x in sources]
            self.objs += [ARDUINO_ENV.core_lib_output_path]
        return self.objs

    def get_deps(self):
        if self.deps is None:
            if os.path.exists(BUILD_DIR):
                self.deps = gcc_utils.get_dependency_dict(BUILD_DIR)
            else:
                self.deps = []
        return self.deps

    def __str__(self):
        output = 'c source files:\n'
        output += ''.join('    ' + x + '\n' for x in self.get_c_sources())
        output += '\nc++ source files:\n'
        output += ''.join('    ' + x + '\n' for x in self.get_cpp_sources())
        output += '\nfound dependency files for the following:\n'
        output += ''.join('    ' + x + '\n' for x in self.get_deps())
        return output


build_data = BuildData()


# ---------------------------------------------------------------------
# utilities

def get_obj_path(source_path):
    return os.path.join(BUILD_DIR, source_path + '.o')


def get_dep_path(source_path):
    return os.path.join(BUILD_DIR, source_path + '.d')


def get_source_dependencies(source):
    deps = build_data.get_deps()
    obj = get_obj_path(source)
    if obj in deps:
        dependencies = deps[obj]
    else:
        dependencies = [source]
    return dependencies


def get_c_compile_command_str(source, obj):
    return gcc_utils.get_compile_cmd_str(source, obj,
                                         compiler=ARDUINO_ENV.c_compiler,
                                         defs=ARDUINO_ENV.cdefs,
                                         includes=ARDUINO_ENV.cincludes,
                                         flags=ARDUINO_ENV.cflags)


# ---------------------------------------------------------------------
# tasks

def task_build_arduino_core():
    for task in ARDUINO_ENV.get_build_core_tasks():
        yield task


def task_compile_c():
    for source in build_data.get_c_sources():
        obj = get_obj_path(source)
        dep = get_dep_path(source)
        yield {
            'name': source + '.o',
            'actions': [(create_folder, [os.path.dirname(obj)]),
                        get_c_compile_command_str(source, obj)],
            'targets': [obj, dep],
            'file_dep': get_source_dependencies(source),
            'clean': True
        }


def task_compile_cpp():
    for source in build_data.get_cpp_sources():
        obj = get_obj_path(source)
        dep = get_dep_path(source)
        yield {
            'name': source + '.o',
            'actions': [(create_folder, [os.path.dirname(obj)]),
                        get_cpp_compile_command_str(source, obj)],
            'targets': [obj, dep],
            'file_dep': get_source_dependencies(source),
            'clean': True
        }


if __name__ == '__main__':
    ARDUINO_ENV.get_build_core_tasks()
    print ARDUINO_ENV
