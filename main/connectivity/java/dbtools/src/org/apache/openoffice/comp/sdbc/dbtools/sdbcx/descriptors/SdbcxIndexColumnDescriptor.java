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

package org.apache.openoffice.comp.sdbc.dbtools.sdbcx.descriptors;

import org.apache.openoffice.comp.sdbc.dbtools.comphelper.PropertySetAdapter.PropertyGetter;
import org.apache.openoffice.comp.sdbc.dbtools.comphelper.PropertySetAdapter.PropertySetter;
import org.apache.openoffice.comp.sdbc.dbtools.util.PropertyIds;

import com.sun.star.lang.XServiceInfo;
import com.sun.star.uno.Type;

public class SdbcxIndexColumnDescriptor extends SdbcxColumnDescriptor implements XServiceInfo {

    private static final String[] services = {
            "com.sun.star.sdbcx.IndexColumnDescriptor"
    };

    protected boolean isAscending;
    
    public SdbcxIndexColumnDescriptor(boolean isCaseSensitive) {
        super(isCaseSensitive);
        registerProperties();
    }
    
    private void registerProperties() {
        registerProperty(PropertyIds.ISASCENDING.name, PropertyIds.ISASCENDING.id, Type.BOOLEAN, (short)0,
                new PropertyGetter() {
                    @Override
                    public Object getValue() {
                        return isAscending;
                        
                    }
                },
                new PropertySetter() {
                    @Override
                    public void setValue(Object value) {
                        isAscending = (boolean) value;
                    }
                });
    }
    
    // XServiceInfo
    
    public String getImplementationName() {
        return getClass().getName();
    }
    
    @Override
    public String[] getSupportedServiceNames() {
        return services.clone();
    }
    
    @Override
    public boolean supportsService(String serviceName) {
        for (String service : getSupportedServiceNames()) {
            if (service.equals(serviceName)) {
                return true;
            }
        }
        return false;
    }
}
