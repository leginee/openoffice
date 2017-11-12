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

import java.util.List;

import org.apache.openoffice.comp.sdbc.dbtools.comphelper.CompHelper;
import org.apache.openoffice.comp.sdbc.dbtools.comphelper.PropertySetAdapter.PropertyGetter;
import org.apache.openoffice.comp.sdbc.dbtools.sdbcx.descriptors.SdbcxIndexDescriptor;
import org.apache.openoffice.comp.sdbc.dbtools.util.DbTools;
import org.apache.openoffice.comp.sdbc.dbtools.util.PropertyIds;

import com.sun.star.beans.PropertyAttribute;
import com.sun.star.beans.XPropertySet;
import com.sun.star.container.ElementExistException;
import com.sun.star.container.XNameAccess;
import com.sun.star.lang.XServiceInfo;
import com.sun.star.sdbc.SQLException;
import com.sun.star.sdbcx.XColumnsSupplier;
import com.sun.star.sdbcx.XDataDescriptorFactory;
import com.sun.star.uno.Type;

public class OIndex extends ODescriptor implements XColumnsSupplier, XDataDescriptorFactory, XServiceInfo {
    
    private static final String[] services = {
            "com.sun.star.sdbcx.Index"
    };

    protected String catalogName;
    protected boolean isUnique;
    protected boolean isPrimaryKeyIndex;
    protected boolean isClustered;
    private OTable table;
    private OContainer columns;
    
    public OIndex(String name, boolean isCaseSensitive, String catalogName,
            boolean isUnique, boolean isPrimaryKeyIndex, boolean isClustered, List<String> columnNames, OTable table) throws ElementExistException {
        super(name, isCaseSensitive);
        this.catalogName = catalogName;
        this.isUnique = isUnique;
        this.isPrimaryKeyIndex = isPrimaryKeyIndex;
        this.isClustered = isClustered;
        this.table = table;
        columns = new OIndexColumnContainer(this, this, columnNames);
        registerProperties();
    }
    
    private void registerProperties() {
        registerProperty(PropertyIds.CATALOG.name, PropertyIds.CATALOG.id, Type.STRING, (short)PropertyAttribute.READONLY,
                new PropertyGetter() {
                    @Override
                    public Object getValue() {
                        return catalogName;
                    }
                }, null);
        registerProperty(PropertyIds.ISPRIMARYKEYINDEX.name, PropertyIds.ISPRIMARYKEYINDEX.id, Type.BOOLEAN, (short)PropertyAttribute.READONLY,
                new PropertyGetter() {
                    @Override
                    public Object getValue() {
                        return isPrimaryKeyIndex;
                    }
                }, null);
        registerProperty(PropertyIds.ISCLUSTERED.name, PropertyIds.ISCLUSTERED.id, Type.BOOLEAN, (short)PropertyAttribute.READONLY,
                new PropertyGetter() {
                    @Override
                    public Object getValue() {
                        return isClustered;
                    }
                }, null);
        registerProperty(PropertyIds.ISUNIQUE.name, PropertyIds.ISUNIQUE.id, Type.BOOLEAN, (short)PropertyAttribute.READONLY,
                new PropertyGetter() {
                    @Override
                    public Object getValue() {
                        return isUnique;
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
        SdbcxIndexDescriptor descriptor = new SdbcxIndexDescriptor(isCaseSensitive());
        CompHelper.copyProperties(this, descriptor);
        try {
            DbTools.cloneDescriptorColumns(this, descriptor);
        } catch (SQLException sqlException) {
        }
        return descriptor;
    }
    
    @Override
    public XNameAccess getColumns() {
        return columns;
    }
    
    public OTable getTable() {
        return table;
    }

    @Override
    public String toString() {
        return "OIndex [catalogName=" + catalogName + ", isUnique=" + isUnique + ", isPrimaryKeyIndex=" + isPrimaryKeyIndex + ", isClustered=" + isClustered
                + ", name=" + getName() + "]";
    }
}
