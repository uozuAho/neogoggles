import file_utils


def get_compile_cmd_str(src, obj, compiler='gcc', defs=[], includes=[], flags=[]):
    cmd_args = [compiler]
    cmd_args += ['-D'+d for d in defs]
    cmd_args += ['-I'+i for i in includes]
    cmd_args += flags + ['-c']
    cmd_args += ['-o', obj]
    cmd_args += [src]
    return _arg_list_to_command_string(cmd_args)


def get_link_cmd_str(target, objs, linker='gcc', libdirs=[], libs=[], flags=[],
                     linker_script=None):
    cmd_args = [linker]
    cmd_args += flags
    cmd_args += ['-L'+d for d in libdirs]
    if linker_script is not None:
        cmd_args += ['-T'+linker_script]
    cmd_args += ['-o', target]
    cmd_args += objs
    cmd_args += ['-l'+lib for lib in libs]
    return _arg_list_to_command_string(cmd_args)


def get_dependency_dict(path, depfile_pattern='*.d'):
    """ Search path and all subdirectories for dependency files,
        return a dictionary of target : [dependencies] pairs
    """
    depfiles = file_utils.find(path, depfile_pattern)
    deps = {}
    for dep in depfiles:
        deps.update(read_dependency_file(dep))
    return deps


def read_dependency_file(path):
    """ Scan a gcc-generated dependency file for targets
        and their dependencies. Returns a dictionary of
        target : [dependencies] pairs
    """

    def get_target_from_line(line):
        """ Return target, remainder if a target found in the line.
            Otherwise return None, None
        """
        line_split = line.split(': ')
        if len(line_split) == 2:
            return line_split[0], line_split[1]
        return None, None

    def get_deps_from_string(string):
        return [s for s in string.split() if s != '\\']

    target_dict = {}
    with open(path) as infile:
        current_target = None
        current_deps = []
        for line in infile:
            tgt, remainder = get_target_from_line(line)
            if tgt is None:
                current_deps += get_deps_from_string(line)
            else:
                if current_target is not None:
                    target_dict[current_target] = current_deps
                current_target = tgt
                current_deps = get_deps_from_string(remainder)
    target_dict[current_target] = current_deps
    return target_dict


def _arg_list_to_command_string(arg_list):
    return ' '.join([str(arg) for arg in arg_list])
