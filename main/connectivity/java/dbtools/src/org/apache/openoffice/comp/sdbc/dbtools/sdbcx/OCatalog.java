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

import org.apache.openoffice.comp.sdbc.dbtools.util.ComposeRule;
import org.apache.openoffice.comp.sdbc.dbtools.util.DbTools;

import com.sun.star.container.XNameAccess;
import com.sun.star.lang.XServiceInfo;
import com.sun.star.lib.uno.helper.ComponentBase;
import com.sun.star.sdbc.SQLException;
import com.sun.star.sdbc.XDatabaseMetaData;
import com.sun.star.sdbc.XRow;
import com.sun.star.sdbcx.XGroupsSupplier;
import com.sun.star.sdbcx.XTablesSupplier;
import com.sun.star.sdbcx.XUsersSupplier;
import com.sun.star.sdbcx.XViewsSupplier;

/** Base expects the containers returned by X(Tables/Views/Groups/Users)Supplier
 * to be the same throughout the lifetime of the catalog!!
 */
public abstract class OCatalog extends ComponentBase
        implements XTablesSupplier, XViewsSupplier, XUsersSupplier, XGroupsSupplier, XServiceInfo {

    private static final String[] services = {
            "com.sun.star.sdbcx.DatabaseDefinition"
    };
    
    // Deleted on destruction, weakly held by caller:
    protected OContainer tables;
    protected OContainer views;
    protected OContainer groups;
    protected OContainer users;
    protected XDatabaseMetaData metadata;
    
    public OCatalog(XDatabaseMetaData metadata) {
        this.metadata = metadata; 
    }
    
    @Override
    protected synchronized void postDisposing() {
        if (tables != null) {
            tables.dispose();
        }
        if (views != null) {
            views.dispose();
        }
        if (groups != null) {
            groups.dispose();
        }
        if (users != null) {
            users.dispose();
        }
    }
    
    // XServiceInfo
    
    @Override
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
    
    // X(Tables/Views/Groups/Users)Supplier
    
    @Override
    public synchronized XNameAccess getTables() {
        checkDisposed();
        if (tables == null) {
            refreshTables();
        }
        return tables;
    }
    
    @Override
    public synchronized XNameAccess getViews() {
        checkDisposed();
        if (views == null) {
            refreshViews();
        }
        return views;            
    }
    
    @Override
    public synchronized XNameAccess getGroups() {
        checkDisposed();
        if (groups == null) {
            refreshGroups();
        }
        return groups;
    }
    
    @Override
    public synchronized XNameAccess getUsers() {
        checkDisposed();
        if (users == null) {
            refreshUsers();
        }
        return users;
    }
    
    public synchronized void refreshObjects() {
        checkDisposed();
        refreshTables();
        refreshViews();
        refreshGroups();
        refreshUsers();
    }
    
    /**
     * Builds the name which should be used to access the object later on in the collection.
     * Will only be called in fillNames.
     * @param  row The current row from the resultset
     */
    protected String buildName(XRow row) throws SQLException {
        String catalog = row.getString(1);
        if (row.wasNull()) {
            catalog = "";
        }
        String schema = row.getString(2);
        if (row.wasNull()) {
            schema = "";
        }
        String table = row.getString(3);
        if (row.wasNull()) {
            table = "";
        }
        return DbTools.composeTableName(metadata, catalog, schema, table, false, ComposeRule.InDataManipulation);
    }
    
    public abstract void refreshTables();
    public abstract void refreshViews();
    public abstract void refreshGroups();
    public abstract void refreshUsers();
}
