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

import os

_siteDir, _myFilename = os.path.split(os.path.abspath(__file__))
_mainDir, _siteDir2 = os.path.split(_siteDir)

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

soenv = _loadJavaProperties(_mainDir + '/ant.properties')

