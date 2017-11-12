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

import org.apache.openoffice.comp.sdbc.dbtools.comphelper.CompHelper;
import org.apache.openoffice.comp.sdbc.dbtools.comphelper.PropertySetAdapter.PropertyGetter;
import org.apache.openoffice.comp.sdbc.dbtools.sdbcx.descriptors.SdbcxKeyColumnDescriptor;
import org.apache.openoffice.comp.sdbc.dbtools.util.PropertyIds;

import com.sun.star.beans.PropertyAttribute;
import com.sun.star.beans.XPropertySet;
import com.sun.star.lang.XServiceInfo;
import com.sun.star.uno.Type;

public class OKeyColumn extends OColumn implements XServiceInfo {
    
    private static final String[] services = {
            "com.sun.star.sdbcx.KeyColumn"
    };
    
    protected String referencedColumn;
    
    protected OKeyColumn(boolean isCaseSensitive) {
        super(isCaseSensitive);
        registerProperties();
    }
    
    public OKeyColumn(
            final String referencedColumn,
            final String name,
            final String typeName,
            final String defaultValue,
            final String description,
            final int isNullable,
            final int precision,
            final int scale,
            final int type,
            final boolean isAutoIncrement,
            final boolean isRowVersion,
            final boolean isCurrency,
            final boolean isCaseSensitive) {
        super(name, typeName, defaultValue, description, isNullable,
                precision, scale, type, isAutoIncrement, isRowVersion, isCurrency, isCaseSensitive);
        this.referencedColumn = referencedColumn;
        registerProperties();
    }
    
    private void registerProperties() {
        registerProperty(PropertyIds.RELATEDCOLUMN.name, PropertyIds.RELATEDCOLUMN.id, Type.STRING, PropertyAttribute.READONLY,
                new PropertyGetter() {
                    @Override
                    public Object getValue() {
                        return referencedColumn;
                        
                    }
                }, null);
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

    // XDataDescriptorFactory
    
    @Override
    public XPropertySet createDataDescriptor() {
        SdbcxKeyColumnDescriptor descriptor = new SdbcxKeyColumnDescriptor(isCaseSensitive());
        synchronized (this) {
            CompHelper.copyProperties(this, descriptor);
        }
        return descriptor;
    }
}
