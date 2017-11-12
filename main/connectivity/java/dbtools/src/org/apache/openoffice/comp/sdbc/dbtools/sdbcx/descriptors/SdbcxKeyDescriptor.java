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
import org.apache.openoffice.comp.sdbc.dbtools.sdbcx.ODescriptor;
import org.apache.openoffice.comp.sdbc.dbtools.util.PropertyIds;

import com.sun.star.container.XNameAccess;
import com.sun.star.lang.XServiceInfo;
import com.sun.star.sdbcx.XColumnsSupplier;
import com.sun.star.uno.Type;

public class SdbcxKeyDescriptor extends ODescriptor implements XColumnsSupplier, XServiceInfo {
    
    private static final String[] services = {
            "com.sun.star.sdbcx.KeyDescriptor"
    };
    
    protected int type;
    protected String referencedTable;
    protected int updateRule;
    protected int deleteRule;
    
    private SdbcxKeyColumnDescriptorContainer columns;
    
    public SdbcxKeyDescriptor(boolean isCaseSensitive) {
        super("", isCaseSensitive, false);
        registerProperties();
        columns = new SdbcxKeyColumnDescriptorContainer(this, isCaseSensitive());
    }
    
    private void registerProperties() {
        registerProperty(PropertyIds.TYPE.name, PropertyIds.TYPE.id, Type.LONG, (short)0,
                new PropertyGetter() {
                    @Override
                    public Object getValue() {
                        return type;
                    }
                },
                new PropertySetter() {
                    @Override
                    public void setValue(Object value) {
                        type = (int) value;
                    }
                });
        registerProperty(PropertyIds.REFERENCEDTABLE.name, PropertyIds.REFERENCEDTABLE.id, Type.STRING, (short)0,
                new PropertyGetter() {
                    @Override
                    public Object getValue() {
                        return referencedTable;
                        
                    }
                },
                new PropertySetter() {
                    @Override
                    public void setValue(Object value) {
                        referencedTable = (String) value;
                    }
                });
        registerProperty(PropertyIds.UPDATERULE.name, PropertyIds.UPDATERULE.id, Type.LONG, (short)0,
                new PropertyGetter() {
                    @Override
                    public Object getValue() {
                        return updateRule;
                        
                    }
                },
                new PropertySetter() {
                    @Override
                    public void setValue(Object value) {
                        updateRule = (int) value;
                    }
                });
        registerProperty(PropertyIds.DELETERULE.name, PropertyIds.DELETERULE.id, Type.LONG, (short)0,
                new PropertyGetter() {
                    @Override
                    public Object getValue() {
                        return deleteRule;
                        
                    }
                },
                new PropertySetter() {
                    @Override
                    public void setValue(Object value) {
                        deleteRule = (int) value;
                    }
                });
    }
    
    @Override
    public XNameAccess getColumns() {
        return columns;
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
