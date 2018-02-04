import subprocess
import aooplatform

class Windows(aooplatform.Platform):
    def __init__(self):
        pass

    def getGUI(self):
        return 'WNT'

    def getCOMID(self):
        return 'MSC'

    def getCompilerDefs(self, soenv):
        return [
            'MSC',
            '_CRT_NON_CONFORMING_SWPRINTFS',
            '_CRT_NONSTDC_NO_DEPRECATE',
            '_CRT_SECURE_NO_DEPRECATE',
            '_MT',
            '_DLL',
            'BOOST_MEM_FN_ENABLE_CDECL',
            'CPPU_ENV=msci',
            'FULL_DESK',
            'M1500'
        ]

    def getCPUDefs(self, soenv):
        return [
            'INTEL',
            '_X86_=1'
        ]

    def getOSDefs(self, soenv):
        return [
            'WINVER=0x0500',
            '_WIN32_IE=0x0500',
            'NT351',
            'WIN32',
            'WNT'
        ]

    def getCFlags(self, soenv):
        flags = [
            '-Gd',
            '-GR',
            '-Gs',
            '-GS',
            '-nologo',
            '-Wall',
            '-wd4005',
            '-wd4061',
            '-wd4127',
            '-wd4180',
            '-wd4189',
            '-wd4191',
            '-wd4217',
            '-wd4250',
            '-wd4251',
            '-wd4255',
            '-wd4275',
            '-wd4290',
            '-wd4294',
            '-wd4350',
            '-wd4355',
            '-wd4365',
            '-wd4503',
            '-wd4505',
            '-wd4511',
            '-wd4512',
            '-wd4514',
            '-wd4611',
            '-wd4619',
            '-wd4625',
            '-wd4626',
            '-wd4640',
            '-wd4668',
            '-wd4675',
            '-wd4692',
            '-wd4710',
            '-wd4711',
            '-wd4738',
            '-wd4786',
            '-wd4800',
            '-wd4820',
            '-wd4826',
            '-Zc:wchar_t-',
            '-Zm500'
        ]
        if soenv['EXTERNAL_WARNINGS_NOT_ERRORS'] != 'TRUE':
            flags += ['-WX']
        return flags

    def getCXXFlags(self, soenv):
        flags = [
            '-Gd',
            '-GR',
            '-Gs',
            '-GS',
            '-Gy',
            '-nologo',
            '-Wall',
            '-wd4005',
            '-wd4061',
            '-wd4127',
            '-wd4180',
            '-wd4189',
            '-wd4191',
            '-wd4217',
            '-wd4250',
            '-wd4251',
            '-wd4275',
            '-wd4290',
            '-wd4294',
            '-wd4350',
            '-wd4355',
            '-wd4365',
            '-wd4503',
            '-wd4505',
            '-wd4511',
            '-wd4512',
            '-wd4514',
            '-wd4611',
            '-wd4619',
            '-wd4625',
            '-wd4626',
            '-wd4640',
            '-wd4668',
            '-wd4675',
            '-wd4692',
            '-wd4710',
            '-wd4711',
            '-wd4738',
            '-wd4786',
            '-wd4800',
            '-wd4820',
            '-wd4826',
            '-Zc:wchar_t-',
            '-Zm500'
        ]
        if soenv['EXTERNAL_WARNINGS_NOT_ERRORS'] != 'TRUE':
            flags += ['-WX']
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
            flags += ['-DEXCEPTIONS_ON', '-EHa']
        else:
            flags += ['-DEXCEPTIONS_OFF']
        return flags

    def getCompilerOptFlags(self, enabled, debugLevel):
        if enabled:
            if debugLevel == 2:
                return []
            else:
                return ['-Ob1', '-Oxs', '-Oy-']
        else:
            return ['-Od']

    def getDebugCFlags(self, compiler, debugging, enableSymbols):
        return ['-Zi']

    def getLibraryDefs(self, soenv):
        return ['_DLL_']

    def getLDFlags(self, soenv, debugging, debugLevel):
        flags = [
            '-MACHINE:IX86',
            '-NODEFAULTLIB'
        ]
        if debugging:
            flags += ['-DEBUG']
        return flags;

    def getLDPATH(self, soenv):
        path = []
        ilib = soenv['ILIB']
        ilib = ilib.replace(';', ' ')
        for i in ilib.split(' '):
            if i != '.':
                path += [i]
        return path

    def getLibraryLDFlags(self, soenv, layer, outDirLocation, debugging, debugLevel):
        flags = self.getLDFlags(soenv, debugging, debugLevel)
        flags += [
            '-DLL',
            '-OPT:NOREF',
            '-SAFESEH',
            '-NXCOMPAT',
            '-DYNAMICBASE',
        ]
        return flags

    def getStandardLibs(self):
        return [
            'kernel32',
            'msvcrt',
            'oldnames',
            'uwinapi'
        ]