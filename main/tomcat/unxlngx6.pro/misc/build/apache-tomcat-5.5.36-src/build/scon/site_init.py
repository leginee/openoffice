#**************************************************************
#  
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#  
#**************************************************************

from SCons.Script import *
import sys
import os

siteDir, myFilename = os.path.split(os.path.abspath(__file__))
mainDir, siteDir2 = os.path.split(siteDir)

# Or should we patch set_soenv to generate us a .py file?
def _loadJavaProperties(filepath):
    props = {}
    with open(filepath, "rt") as f:
        for line in f:
            l = line.strip()
            if l and not l.startswith('#'):
                eq = l.find('=')
                if eq >= 0:
                    key = l[:eq].strip()
                    value = l[(eq+1):].strip()
                    props[key] = value
    return props

soenv = _loadJavaProperties(mainDir + os.sep + 'ant.properties')

############################
# Product and debug settings
############################

PRODUCT = (soenv.get('PRODUCT') == 'TRUE')

if 'DEBUG' in soenv:
    DEBUGLEVEL = 2
elif PRODUCT:
    DEBUGLEVEL = 1
else:
    DEBUGLEVEL = 0

DEBUGGING = False
if DEBUGLEVEL == 2:
    DEBUGGING = True

if soenv.get('ENABLE_SYMBOLS') == 'SMALL':
    DEBUGGING = True
elif soenv.get('ENABLE_SYMBOLS') == 'TRUE':
    DEBUGGING = True

if soenv.get('ENABLE_CRASHDUMP') == 'TRUE':
    DEBUGGING = True

#####################
# Precompiled headers
#####################

ENABLE_PCH = (soenv.get('ENABLE_PCH') == 'TRUE')

#######
# Paths
#######

OUTDIR = soenv['SOLARVERSION'] + "/" + soenv['INPATH']
WORKDIR = OUTDIR + "/workdir"
OUTDIRLOCATION = OUTDIR + os.sep + 'lib'

###########
# Platforms
###########

if soenv['OS'] == 'FREEBSD':
    from freebsd import *
    platform = FreeBSD()
elif soenv['OS'] == 'WNT':
    from windows import *
    platform = Windows()
else:
    raise Exception ('Unsupported OS: ' + soenv['OS'])

GUI = platform.getGUI()
COMID = platform.getCOMID()

######
# Defs
######

GLOBALDEFS = [
    '_REENTRANT',
    'CUI',
    'ENABLE_LAYOUT_EXPERIMENTAL=0',
    'ENABLE_LAYOUT=0',
    'OSL_DEBUG_LEVEL=' + str(DEBUGLEVEL),
    'SOLAR_JAVA',
    'SUPD=' + soenv['UPD'],
    'VCL'
]

GLOBALDEFS += platform.getOSDefs(soenv)
GLOBALDEFS += platform.getCompilerDefs(soenv)
GLOBALDEFS += platform.getCPUDefs(soenv)

if PRODUCT:
    GLOBALDEFS += ['PRODUCT', 'PRODUCT_FULL']
else:
    GLOBALDEFS += ['DBG_UTIL', 'STLP_DEBUG']

if DEBUGLEVEL == 2:
    GLOBALDEFS += ['DEBUG'];
else:
    GLOBALDEFS += ['OPTIMIZE', 'NDEBUG']

if 'ENABLE_GTK' in soenv:
    GLOBALDEFS += ['ENABLE_GTK']
if 'ENABLE_KDE' in soenv:
    GLOBALDEFS += ['ENABLE_KDE']
if 'ENABLE_KDE4' in soenv:
    GLOBALDEFS += ['ENABLE_KDE4']
if 'ENABLE_GRAPHITE' in soenv:
    GLOBALDEFS += ['ENABLE_GRAPHITE']


###########
# Libraries
###########

# FIXME: this probably belongs elsewhere
UCBHELPER_MAJOR=4
UDK_MAJOR=3

if GUI == 'UNX' or soenv['COM'] == 'GCC':
    if GUI+soenv['COM'] == 'WNTGCC':
        CPPULIB = 'cppu' + UDK_MAJOR
        CPPUHELPERLIB = 'cppuhelper' + UDK_MAJOR + COMID
    elif GUI == 'OS2':
        CPPULIB = 'cppu' + UDK_MAJOR
        CPPUHELPERLIB = 'cppuh' + UDK_MAJOR
    else:
        CPPULIB = 'uno_cppu'
        CPPUHELPERLIB = 'uno_cppuhelper' + COMID

    if GUI+soenv['COM'] == 'WNTGCC' or GUI == 'OS2':
        SALLIB = 'sal' + UDK_MAJOR
    else:
        SALLIB = 'uno_sal'

    TOOLSLIB = 'tl'

    if GUI == 'OS2':
        UCBHELPERLIB = 'ucbh' + UCBHELPER_MAJOR
    else:
        UCBHELPERLIB=  'ucbhelper' + COMID

    UNOTOOLSLIB = 'utl'

else:
    CPPULIB = 'icppu'
    CPPUHELPERLIB = 'icppuhelper'
    SALLIB = 'isal'
    TOOLSLIB = 'itools'
    UCBHELPERLIB = 'iucbhelper'
    UNOTOOLSLIB = 'iutl'

STDLIBS = platform.getStandardLibs()

###########
# API
###########

def SetUpSharedLibraryEnvironment(name, layer):
    env = Environment()
    env.Replace(CC=soenv['CC'])
    env.Replace(CXX=soenv['CXX'])

    env.Append(CPPPATH=platform.getInclude(soenv))

    defines = GLOBALDEFS
    defines += platform.getLibraryDefs(soenv)
    env.Append(CPPDEFINES=defines)

    cflags = platform.getCFlags(soenv)
    cxxflags = platform.getCXXFlags(soenv)
    if DEBUGGING:
        cflags += platform.getDebugCFlags(soenv['COM'], soenv['ENABLE_SYMBOLS'])
        cxxflags += platform.getDebugCFlags(soenv['COM'], soenv['ENABLE_SYMBOLS'])
    env.Append(CFLAGS=cflags)
    env.Append(CXXFLAGS=cxxflags)

    env.Append(LINKFLAGS=platform.getLibraryLDFlags(soenv, layer, OUTDIRLOCATION, DEBUGGING, DEBUGLEVEL))
    env.Append(LIBPATH=platform.getLDPATH(soenv))

    return env


def AddAPI(env, names):
    for name in names:
        env.Append(CPPPATH=OUTDIR + '/inc/' + name)

def AddExceptionSharedObjects(env, names):
    return AddCxxSharedObjects(env, names,
        platform.getExceptionFlags(soenv, True)
        + platform.getCompilerOptFlags(DEBUGLEVEL < 2, DEBUGLEVEL))

def AddNoExceptionSharedObjects(env, names):
    return AddCxxSharedObjects(env, names,
        platform.getExceptionFlags(soenv, False)
        + platform.getCompilerOptFlags(DEBUGLEVEL < 2, DEBUGLEVEL))

def AddCxxSharedObjects(env, names, flags):
    objects = []
    for name in names:
        objects.append(env.SharedObject(name, CXXFLAGS=flags))
    return objects

