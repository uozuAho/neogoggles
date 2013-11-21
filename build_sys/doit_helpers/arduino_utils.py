import os
import gcc_utils
import file_utils
from doit.tools import create_folder


C_FLAGS = [
    '-c',
    '-g',
    '-Os',
    '-Wall',
    '-ffunction-sections',
    '-fdata-sections',
    '-mmcu=atmega328p',
    '-MMD',
]

C_DEFS = [
    'F_CPU=16000000L',
    'USB_VID=null',
    'USB_PID=null',
    'ARDUINO=105',
]

CPP_FLAGS = C_FLAGS + ['-fno-exceptions']

CPP_DEFS = C_DEFS

LINKER_FLAGS = [
    '-Os',
    '-Wl,--gc-sections',
    '-mmcu=atmega328p',
]

LINKER_LIBS = [
    'm'
]

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

OBJCOPY_FLASH_FLAGS = [
    '-O',
    'ihex',
    '-R',
    '.eeprom',
]

AVRDUDE_FLAGS = [
    '-patmega328p',
    '-carduino',
    '-b115200',
    '-D'
]


class ArduinoEnv:

    def __init__(self, proj_name, arduino_path, build_dir):
        self.proj_name = proj_name
        self.build_dir = build_dir
        self.root_path = arduino_path
        self.bin_path = os.path.join(
            self.root_path, 'hardware', 'tools', 'avr', 'bin')
        self.core_path = os.path.join(
            self.root_path, 'hardware', 'arduino', 'cores', 'arduino')
        self.libraries_path = os.path.join(self.root_path, 'libraries')

        self.c_compiler = os.path.join(self.bin_path, 'avr-gcc')
        self.cpp_compiler = os.path.join(self.bin_path, 'avr-g++')
        self.linker = os.path.join(self.bin_path, 'avr-gcc')
        self.archiver = os.path.join(self.bin_path, 'avr-ar')
        self.objcopy = os.path.join(self.bin_path, 'avr-objcopy')
        self.print_size = os.path.join(self.bin_path, 'avr-size')
        self.avrdude = os.path.join(self.bin_path, 'avrdude')

        self.avrdude_conf = os.path.join(
            self.root_path, 'hardware', 'tools', 'avr', 'etc', 'avrdude.conf')

        self.cflags = C_FLAGS
        self.cdefs = C_DEFS
        self.cincludes = [
            self.core_path,
            os.path.join(
                self.root_path, 'hardware', 'arduino', 'variants', 'standard')
        ]
        self.cppflags = CPP_FLAGS
        self.cppdefs = CPP_DEFS
        self.cppincludes = self.cincludes
        self.ldflags = LINKER_FLAGS
        self.ldlibs = LINKER_LIBS
        self.ldincludes = []

        self.core_csources = file_utils.find(self.core_path, '*.c')
        self.core_cppsources = file_utils.find(self.core_path, '*.cpp')

        self.core_lib_output_dir = os.path.join(
            self.build_dir, '_arduino_core_')
        self.core_obj_output_dir = os.path.join(
            self.core_lib_output_dir, 'obj')
        self.core_lib_output_path = os.path.join(
            self.core_lib_output_dir, 'core.a')

        self.core_objs = [self._source_to_obj_path(x)
                          for x in self.core_csources + self.core_cppsources]

        self._set_core_deps()

        self.elf_target = os.path.join(self.build_dir, self.proj_name + '.elf')
        self.eep_target = os.path.join(self.build_dir, self.proj_name + '.eep')
        self.hex_target = os.path.join(self.build_dir, self.proj_name + '.hex')

    # -------------------------------------------------------------------
    # public

    def get_build_core_tasks(self):
        tasks = self._get_compile_core_c_tasks()
        tasks += self._get_compile_core_cpp_tasks()
        tasks += self._get_archive_core_objs_tasks()
        return tasks

    def get_build_exe_tasks(self, name, objs):
        tasks = self._get_build_elf_tasks(objs, self.elf_target)
        tasks += self._get_build_eeprom_binary_tasks(self.elf_target, self.eep_target)
        tasks += self._get_build_flash_binary_tasks(self.elf_target, self.hex_target)
        tasks += self._get_print_size_task(self.elf_target)
        return tasks

    def get_upload_task(self, serial_port):
        avrdude_flags = AVRDUDE_FLAGS
        avrdude_flags += ['-C' + self.avrdude_conf] + ['-P' + serial_port]
        avrdude_flags += ['-Uflash:w:' + self.hex_target + ':i']
        avrdude_cmd = ' '.join([self.avrdude] + avrdude_flags)
        return {
            'actions': [avrdude_cmd],
            'file_dep': [self.hex_target],
            # dummy target so this always runs
            'targets': ['upload'],
            'verbosity': 2
        }

    # -------------------------------------------------------------------
    # private

    def _source_to_obj_path(self, source):
        src_filename = os.path.basename(source)
        return os.path.join(self.core_obj_output_dir, src_filename) + '.o'

    def _source_to_dep_path(self, source):
        src_filename = os.path.basename(source)
        return os.path.join(self.core_obj_output_dir, src_filename) + '.d'

    def _set_core_deps(self):
        if os.path.exists(self.core_obj_output_dir):
            self.deps = gcc_utils.get_dependency_dict(self.core_obj_output_dir)
        else:
            self.deps = {}

    def _get_core_obj_deps(self, obj_path):
        if obj_path in self.deps:
            return self.deps[obj_path]
        else:
            return []

    def _get_compile_core_c_tasks(self):
        tasks = []
        for source in self.core_csources:
            obj = self._source_to_obj_path(source)
            dep = self._source_to_dep_path(source)
            tasks.append({
                'name': obj,
                'actions': [(create_folder, [self.core_obj_output_dir]),
                            gcc_utils.get_compile_cmd_str(source, obj,
                                                          compiler=self.c_compiler,
                                                          defs=self.cdefs,
                                                          includes=self.cincludes,
                                                          flags=self.cflags)],
                'targets': [obj, dep],
                'file_dep': self._get_core_obj_deps(obj),
                'clean': True
            })
        return tasks

    def _get_compile_core_cpp_tasks(self):
        tasks = []
        for source in self.core_cppsources:
            obj = self._source_to_obj_path(source)
            dep = self._source_to_dep_path(source)
            tasks.append({
                'name': obj,
                'actions': [(create_folder, [self.core_obj_output_dir]),
                            gcc_utils.get_compile_cmd_str(source, obj,
                                                          compiler=self.cpp_compiler,
                                                          defs=self.cppdefs,
                                                          includes=self.cppincludes,
                                                          flags=self.cppflags)],
                'targets': [obj, dep],
                'file_dep': self._get_core_obj_deps(obj),
                'clean': True
            })
        return tasks

    def _get_archive_core_objs_tasks(self):
        archive_command = self.archiver + \
            ' rcs ' + self.core_lib_output_path + ' '
        archive_command += ' '.join(self.core_objs)
        return [{
            'name': 'archiving core',
            'actions': [archive_command],
            'targets': [self.core_lib_output_path],
            'file_dep': self.core_objs,
            'clean': True
        }]

    def _get_build_elf_tasks(self, objs, dest):
        return [{
            'name': dest,
            'actions': [gcc_utils.get_link_cmd_str(dest, objs,
                                                   linker=self.linker,
                                                   libdirs=self.ldincludes,
                                                   libs=self.ldlibs,
                                                   flags=self.ldflags)],
            'file_dep': objs,
            'targets': [dest],
            'clean': True
        }]

    def _get_build_eeprom_binary_tasks(self, source, dest):
        objcopy_cmd = ' '.join([self.objcopy] + OBJCOPY_EEPROM_FLAGS + [source, dest])
        return [{
            'name': dest,
            'actions': [objcopy_cmd],
            'file_dep': [source],
            'targets': [dest],
            'clean': True
        }]

    def _get_build_flash_binary_tasks(self, source, dest):
        objcopy_cmd = ' '.join([self.objcopy] + OBJCOPY_FLASH_FLAGS + [source, dest])
        return [{
            'name': dest,
            'actions': [objcopy_cmd],
            'file_dep': [source],
            'targets': [dest],
            'clean': True
        }]

    def _get_print_size_task(self, binary):
        return [{
            'name': 'size',
            'actions': [self.print_size + ' ' + binary],
            'file_dep': [binary],
            # dummy target makes this always run
            'targets': ['print size'],
            'verbosity': 2,
        }]

    def __str__(self):
        out_str = ''
        for key in dir(self):
            out_str += str(key) + ':\n'
            attr = getattr(self, key)
            if not hasattr(attr, '__iter__') or type(attr) is str:
                out_str += '    ' + str(attr) + '\n'
            else:
                for item in attr:
                    out_str += '    ' + str(item) + '\n'
        return out_str
