import os
from doit.tools import create_folder
from build_sys.doit_helpers import file_utils
from build_sys.doit_helpers import gcc_utils

DOIT_CONFIG = {'default_tasks': ['print_size']}


# ---------------------------------------------------------------------
# Build settings

PROJECT_NAME = 'goggles'

BUILD_DIR = 'build'

C_COMPILER = r'D:\programs_win\arduino-1.0.5\hardware\tools\avr\bin\avr-gcc'

C_INCLUDE_FLAGS = [
    '-Ilibs/arduino_core/include',
    '-Ilibs/neopixel/include',
]

C_FLAGS = [
    '-c',
    '-g',
    '-Os',
    '-Wall',
    '-ffunction-sections',
    '-fdata-sections',
    '-mmcu=atmega328p',
    '-DF_CPU=16000000L',
    '-MMD',
    '-DUSB_VID=null',
    '-DUSB_PID=null',
    '-DARDUINO=105',
]

CPP_COMPILER = r'D:\programs_win\arduino-1.0.5\hardware\tools\avr\bin\avr-g++'

CPP_INCLUDE_FLAGS = [
    '-Ilibs/arduino_core/include',
    '-Ilibs/neopixel/include',
]

CPP_FLAGS = C_FLAGS + ['-fno-exceptions']

LINKER = r'D:\programs_win\arduino-1.0.5\hardware\tools\avr\bin\avr-gcc'

LINKER_FLAGS = [
    '-Os',
    '-Wl,--gc-sections',
    '-mmcu=atmega328p',
]

LINKER_LIB_FLAGS = [
    '-lm'
]

LINKER_TARGET = os.path.join(BUILD_DIR, PROJECT_NAME + '.elf')

OBJCOPY_PROGRAM = r'D:\programs_win\arduino-1.0.5\hardware\tools\avr\bin\avr-objcopy'

OBJCOPY_EEPROM_FLAGS = [
    '-O',
    'ihex',
    '-j',
    '.eeprom',
    '--set-section-flags=.eeprom=alloc,load',
    '--no-change-warnings',
    '--change-section-lma',
    '.eeprom=0',
]

EEPROM_TARGET = os.path.join(BUILD_DIR, PROJECT_NAME + '.eep')

OBJCOPY_FLASH_FLAGS = [
    '-O',
    'ihex',
    '-R',
    '.eeprom',
]

FLASH_TARGET = os.path.join(BUILD_DIR, PROJECT_NAME + '.hex')

AVRDUDE_LOCATION = r'D:\programs_win\arduino-1.0.5\hardware\tools\avr\bin\avrdude'

AVRDUDE_CONF_FILE = r'D:\programs_win\arduino-1.0.5\hardware\tools\avr\etc\avrdude.conf'

AVRDUDE_FLAGS = [
    '-C' + AVRDUDE_CONF_FILE,
    '-patmega328p',
    '-carduino',
    '-PCOM5',
    '-b115200',
    '-D'
]

SOURCE_DIRS = [
    'src',
    'libs/neopixel'
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
            sources = self.get_c_sources()
            sources += self.get_cpp_sources()
            self.objs = [get_obj_path(x) for x in sources]
            self.objs += ['libs/arduino_core/core.a']
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


def get_c_compile_command_str(source, dest):
    return ' '.join([C_COMPILER] + C_FLAGS + C_INCLUDE_FLAGS + [source, '-o', dest])


def get_cpp_compile_command_str(source, dest):
    return ' '.join([CPP_COMPILER] + CPP_FLAGS + CPP_INCLUDE_FLAGS + [source, '-o', dest])


def get_link_command_str(objs, dest):
    return ' '.join([LINKER] + LINKER_FLAGS + ['-o', dest] + objs + LINKER_LIB_FLAGS)


def get_objcopy_eeprom_command_str(input, output):
    return ' '.join([OBJCOPY_PROGRAM] + OBJCOPY_EEPROM_FLAGS + [input, output])


def get_objcopy_flash_command_str(input, output):
    return ' '.join([OBJCOPY_PROGRAM] + OBJCOPY_FLASH_FLAGS + [input, output])


def get_avrdude_command_str(hex_file):
    return ' '.join([AVRDUDE_LOCATION] + AVRDUDE_FLAGS +
                    ['-Uflash:w:' + hex_file + ':i'])


# ---------------------------------------------------------------------
# doit tasks

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


def task_link():
    return {
        'actions': [get_link_command_str(build_data.get_objs(), LINKER_TARGET)],
        'targets': [LINKER_TARGET],
        'file_dep': build_data.get_objs(),
        'clean': True
    }


def task_create_eeprom_binary():
    return {
        'actions': [get_objcopy_eeprom_command_str(LINKER_TARGET, EEPROM_TARGET)],
        'targets': [EEPROM_TARGET],
        'file_dep': [LINKER_TARGET],
        'clean': True
    }


def task_create_flash_binary():
    return {
        'actions': [get_objcopy_flash_command_str(LINKER_TARGET, FLASH_TARGET)],
        'targets': [FLASH_TARGET],
        'file_dep': [LINKER_TARGET],
        'clean': True
    }


def task_print_size():
    return {
        'actions': [r'D:\programs_win\arduino-1.0.5\hardware\tools\avr\bin\avr-size ' + LINKER_TARGET],
        'file_dep': [LINKER_TARGET],
        # dummy target so this always runs
        'targets': ['print size'],
        'verbosity': 2
    }


def task_upload():
    return {
        'actions': [get_avrdude_command_str(FLASH_TARGET)],
        'file_dep': [FLASH_TARGET],
        # dummy target so this always runs
        'targets': ['upload'],
        'verbosity': 2
    }


if __name__ == '__main__':
    print build_data
