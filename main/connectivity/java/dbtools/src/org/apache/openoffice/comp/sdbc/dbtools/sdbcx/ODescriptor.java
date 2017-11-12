/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/

package org.apache.openoffice.comp.sdbc.dbtools.sdbcx;

import org.apache.openoffice.comp.sdbc.dbtools.comphelper.PropertySet;
import org.apache.openoffice.comp.sdbc.dbtools.comphelper.PropertySetAdapter.PropertyGetter;
import org.apache.openoffice.comp.sdbc.dbtools.comphelper.PropertySetAdapter.PropertySetter;
import org.apache.openoffice.comp.sdbc.dbtools.util.PropertyIds;

import com.sun.star.beans.PropertyAttribute;
import com.sun.star.uno.Type;

public class ODescriptor extends PropertySet {
    private String name;
    private final boolean isCaseSensitive;
    
    public ODescriptor(String name, boolean isCaseSensitive, boolean isReadOnly) {
        this.name = name;
        this.isCaseSensitive = isCaseSensitive;
        registerProperties(isReadOnly);
    }

    public ODescriptor(String name, boolean isCaseSensitive) {
        this(name, isCaseSensitive, true);
    }

    private void registerProperties(boolean isReadOnly) {
        registerProperty(PropertyIds.NAME.name, PropertyIds.NAME.id, Type.STRING, isReadOnly ? PropertyAttribute.READONLY : 0,
                new PropertyGetter() {
                    @Override
                    public Object getValue() {
                        return name;
                        
                    }
                },
                isReadOnly ? null : new PropertySetter() {
                    @Override
                    public void setValue(Object value) {
                        name = (String) value;
                    }
                });
    }

    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public boolean isCaseSensitive() {
        return isCaseSensitive;
    }
}
