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


#ifndef RPT_XMLREPORT_HXX
#define RPT_XMLREPORT_HXX

#include "xmlReportElementBase.hxx"
#include <vector>

namespace rptxml
{
	class ORptFilter;
	class OXMLReport : public OXMLReportElementBase, public IMasterDetailFieds
	{
        ::com::sun::star::uno::Reference< ::com::sun::star::report::XReportDefinition > m_xComponent;
        ::std::vector< ::rtl::OUString> m_aMasterFields;
        ::std::vector< ::rtl::OUString> m_aDetailFields;
        OXMLReport(const OXMLReport&);
        void operator =(const OXMLReport&);
	public:

		OXMLReport( ORptFilter& rImport, sal_uInt16 nPrfx,
					const ::rtl::OUString& rLName,
					const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList > & xAttrList 
                    ,const ::com::sun::star::uno::Reference< ::com::sun::star::report::XReportDefinition >& _xComponent
					,OXMLTable* _pContainer);
		virtual ~OXMLReport();

		virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
					const ::rtl::OUString& rLocalName,
					const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList > & xAttrList );

        virtual void EndElement();

        virtual void addMasterDetailPair(const ::std::pair< ::rtl::OUString,::rtl::OUString >& _aPair);

    private:
        /** initializes our object's properties whose runtime (API) default is different from the file
            format default.
        */
        void    impl_initRuntimeDefaults() const;
	};
// -----------------------------------------------------------------------------
} // namespace rptxml
// -----------------------------------------------------------------------------

#endif // RPT_XMLREPORT_HXX
