import subprocess
import aooplatform

class FreeBSD(aooplatform.Platform):
    def __init__(self):
        self.GXX_INCLUDE_PATH = None
        self.layerRPaths = {}
        self.layerRPaths['URELIB'] = '$ORIGIN'
        self.layerRPaths['UREBIN'] = '$ORIGIN/../lib:$ORIGIN'
        self.layerRPaths['OOO'] = '$ORIGIN:$ORIGIN/../ure-link/lib'
        self.layerRPaths['BRAND'] = '$ORIGIN:$ORIGIN/../basis-link/program:$ORIGIN/../basis-link/ure-link/lib'
        self.layerRPaths['SDKBIN'] = '$ORIGIN/../../ure-link/lib'
        self.layerRPaths['NONEBIN'] = '$ORIGIN/../lib:$ORIGIN'

    def getGUI(self):
        return 'UNX'

    def getCOMID(self):
        return 'gcc3'

    def getCompilerDefs(self, soenv):
        if self.GXX_INCLUDE_PATH is None:
            gccVersion = subprocess.Popen("gcc -dumpversion", shell=True, stdout=subprocess.PIPE).stdout.readline().rstrip()
            self.GXX_INCLUDE_PATH = soenv['COMPATH'] + "/include/c++/" + gccVersion
        return [
            soenv['COM'],
            'HAVE_GCC_VISIBILITY_FEATURE',
            'CPPU_ENV=gcc3',
            'GXX_INCLUDE_PATH=' + self.GXX_INCLUDE_PATH
        ]

    def getCPUDefs(self, soenv):
        return [soenv['CPUNAME']]

    def getOSDefs(self, soenv):
        defs = [
            soenv['OS'],
            '_PTHREADS',
            'UNIX',
            'UNX'
        ]
        if 'PTHREAD_CFLAGS' in soenv:
            defs += [soenv['PTHREAD_CFLAGS']]
        return defs

    def getCFlags(self, soenv):
        flags = [
            '-Wall',
            '-Wendif-labels',
            '-Wextra',
            '-Wshadow',
            '-fPIC',
            '-fmessage-length=0',
            '-fno-common',
            '-fno-strict-aliasing',
            '-fvisibility=hidden',
            '-pipe'
        ]
        if soenv['EXTERNAL_WARNINGS_NOT_ERRORS'] != 'TRUE':
            flags += ['-Werror']
        if 'SYSBASE' in soenv:
            flags += ['--sysroot=' + soenv['SYSBASE']]
        return flags

    def getCXXFlags(self, soenv):
        flags = [
            '-Wall',
            '-Wendif-labels',
            '-Wextra',
            '-Wno-ctor-dtor-privacy',
            '-Wno-non-virtual-dtor',
            '-Wshadow',
            '-fPIC',
            '-fmessage-length=0',
            '-fno-common',
            '-fno-strict-aliasing',
            '-fno-use-cxa-atexit',
            '-fvisibility-inlines-hidden',
            '-fvisibility=hidden',
            '-pipe'
        ]
        if soenv['EXTERNAL_WARNINGS_NOT_ERRORS'] != 'TRUE':
            flags += ['-Werror']
        if 'SYSBASE' in soenv:
            flags += ['--sysroot=' + soenv['SYSBASE']]
        if soenv['COM'] == 'CLANG':
            flags += ['-DHAVE_STL_INCLUDE_PATH']
        else:
            flags += ['-DBOOST_TR1_DISABLE_INCLUDE_NEXT', '-DBOOST_TR1_GCC_INCLUDE_PATH=c++']
        return flags

    def getInclude(self, soenv):
        includes = []
        for i in soenv['SOLARINC'].split(' '):
            if i.endswith('/stl'):
                continue;
            if i.startswith('-I'):
                includes.append(i[2:])
        return includes

    def getIncludeStl(self, soenv):
        includes = []
        for i in soenv['SOLARINC'].split(' '):
            if i.startswith('-I') and i.endswith('/stl'):
                includes.append(i[2:])
        return includes

    def getExceptionFlags(self, soenv, enabled):
        flags = []
        if enabled:
            flags += ['-DEXCEPTIONS_ON', '-fexceptions']
            if soenv['COM'] == 'GCC':
                flags += ['-fno-enforce-eh-specs'];
        else:
            flags += ['-DEXCEPTIONS_OFF', '-fno-exceptions']
        return flags

    def getCompilerOptFlags(self, enabled, debugLevel):
        if enabled:
            if debugLevel == 2:
                return ['-O0']
            else:
                return ['-Os']
        else:
            return ['-O0']

    def getDebugCFlags(self, compiler, enableSymbols):
        if compiler == 'CLANG':
            if enableSymbols == 'SMALL':
                return ['-ggdb1', '-fno-inline']
            else:
                return ['-ggdb3', '-fno-inline']
        else:
            if enableSymbols == 'SMALL':
                return ['-ggdb1', '-finline-limit=0', '-fno-inline', '-fno-default-inline']
            else:
                return ['-ggdb3', '-finline-limit=0', '-fno-inline', '-fno-default-inline']

    def getRPATH(self, layer):
        return self.layerRPaths[layer]

    def getLibraryDefs(self, soenv):
        return []

    def getLDFlags(self, soenv, debugging, debugLevel):
        sysbase = soenv.get('SYSBASE')
        if sysbase is None:
            sysbase = ''
        flags = [
            '-Wl,-rpath-link,' + sysbase + '/lib:' + sysbase + '/usr/lib',
            '-Wl,-z,combreloc',
            '-Wl,-z,defs'
        ]
        if soenv.get('FBSD_GCC_RPATH') is not None:
            flags += [soenv['FBSD_GCC_RPATH']]
        if sysbase != '':
            flags += ['--sysroot=' + sysbase]
        if soenv['HAVE_LD_HASH_STYLE'] == 'TRUE':
            flags += ['-Wl,--hash-style=both']
        if soenv['HAVE_LD_BSYMBOLIC_FUNCTIONS'] == 'TRUE':
            flags += [
                '-Wl,--dynamic-list-cpp-new',
                '-Wl,--dynamic-list-cpp-typeinfo',
                '-Wl,-Bsymbolic-functions'
            ]
        if debugLevel == 0:
            flags += ['-Wl,-O1']
        return flags;

    def getLDPATH(self, soenv):
        path = soenv['SOLARLIB']
        path = path.replace('-L../lib ', '')
        path = path.replace('-L', '')
        return path.split()

    def getLibraryLDFlags(self, soenv, layer, outDirLocation, debugging, debugLevel):
        flags = self.getLDFlags(soenv, debugging, debugLevel)
        flags += [ '-Wl,-z,noexecstack' ]
        flags += [
            '-Wl,-z,origin',
            '-Wl,-rpath,' + self.getRPATH(layer),
            '-Wl,-rpath-link,' + outDirLocation
        ]
        return flags

    def getStandardLibs(self):
        return []
