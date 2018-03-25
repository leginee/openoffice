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



#include "wrapper.hxx"

#include <com/sun/star/awt/XLayoutRoot.hpp>
#include <com/sun/star/awt/XLayoutContainer.hpp>
#include <comphelper/processfactory.hxx>
#include <layout/core/helper.hxx>
#include <tools/debug.hxx>

using namespace ::com::sun::star;

namespace layout
{

Container::Container( Context const* context, char const* pId )
    : mxContainer( context->GetPeerHandle( pId ), uno::UNO_QUERY )
{
    if ( !mxContainer.is() )
    {
        DBG_ERROR1( "Error: failed to associate container with '%s'", pId );
    }
}

Container::Container( rtl::OUString const& rName, sal_Int32 nBorder )
{
    mxContainer = layoutimpl::WidgetFactory::createContainer( rName );

    uno::Reference< beans::XPropertySet > xProps( mxContainer, uno::UNO_QUERY_THROW );
    xProps->setPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "Border" ) ),
                              uno::Any( nBorder ) );
}

void Container::Add( Window *pChild )
{
    if ( pChild )
    {
        uno::Reference< awt::XLayoutConstrains > xChild( pChild->GetPeer(), uno::UNO_QUERY );
        mxContainer->addChild( xChild );
    }
}

void Container::Add( Container *pChild )
{
    if ( pChild )
    {
        uno::Reference< awt::XLayoutConstrains > xChild( pChild->getImpl(), uno::UNO_QUERY );
        mxContainer->addChild( xChild );
    }
}

void Container::Remove( Window *pChild )
{
    if ( pChild )
    {
        uno::Reference< awt::XLayoutConstrains > xChild( pChild->GetPeer(), uno::UNO_QUERY );
        mxContainer->removeChild( xChild );
    }
}

void Container::Remove( Container *pChild )
{
    if ( pChild )
    {
        uno::Reference< awt::XLayoutConstrains > xChild( pChild->getImpl(), uno::UNO_QUERY );
        mxContainer->removeChild( xChild );
    }
}

void Container::Clear()
{
    css::uno::Sequence< css::uno::Reference < css::awt::XLayoutConstrains > > children;
    children = mxContainer->getChildren();
    for (int i = 0; i < children.getLength(); i++)
        mxContainer->removeChild( children[i] );
}

void Container::ShowAll( bool bShow )
{
    struct inner
    {
        static void setChildrenVisible( uno::Reference < awt::XLayoutContainer > xCont,
                                        bool bVisible )  /* auxiliary */
        {
            if ( xCont.is() )
            {
                uno::Sequence< uno::Reference < awt::XLayoutConstrains > > aChildren;
                aChildren = xCont->getChildren();
                for (int i = 0; i < aChildren.getLength(); i++)
                {
                    uno::Reference < awt::XLayoutConstrains > xChild( aChildren[ i ] );

                    uno::Reference< awt::XWindow > xWin( xChild, uno::UNO_QUERY);
                    if ( xWin.is() )
                        xWin->setVisible( bVisible );

                    uno::Reference < awt::XLayoutContainer > xChildCont(
                        xChild, uno::UNO_QUERY );
                    setChildrenVisible( xChildCont, bVisible );
                }
            }
        }
    };

    inner::setChildrenVisible( mxContainer, bShow );
}

void Container::Show()
{
    ShowAll( true );
}

void Container::Hide()
{
    ShowAll( false );
}

Table::Table( sal_Int32 nBorder, sal_Int32 nColumns )
    : Container( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "table" ) ), nBorder )
{
    uno::Reference< beans::XPropertySet > xProps( mxContainer, uno::UNO_QUERY_THROW );
    xProps->setPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "Columns" ) ),
                              uno::Any( nColumns ) );
}

void Table::Add( Window *window, bool bXExpand, bool bYExpand,
                 sal_Int32 nXSpan, sal_Int32 nYSpan )
{
    if ( !window )
        return;
    WindowImpl *pImpl = window->getImpl();
    uno::Reference< awt::XLayoutConstrains > xChild( pImpl->mxWindow,
                                                     uno::UNO_QUERY );
    mxContainer->addChild( xChild );
    setProps( xChild, bXExpand, bYExpand, nXSpan, nYSpan );
}

void Table::Add( Container *pContainer, bool bXExpand, bool bYExpand,
                 sal_Int32 nXSpan, sal_Int32 nYSpan )
{
    if ( !pContainer )
        return;
    uno::Reference< awt::XLayoutConstrains > xChild( pContainer->getImpl(),
                                                     uno::UNO_QUERY );
    mxContainer->addChild( xChild );
    setProps( xChild, bXExpand, bYExpand, nXSpan, nYSpan );
}

void Table::setProps( uno::Reference< awt::XLayoutConstrains > xChild,
                      bool bXExpand, bool bYExpand, sal_Int32 nXSpan, sal_Int32 nYSpan )
{
    uno::Reference< beans::XPropertySet > xProps
        ( mxContainer->getChildProperties( xChild ), uno::UNO_QUERY_THROW );
    xProps->setPropertyValue( rtl::OUString ( RTL_CONSTASCII_USTRINGPARAM( "XExpand" ) ),
                              uno::Any( bXExpand ) );
    xProps->setPropertyValue( rtl::OUString ( RTL_CONSTASCII_USTRINGPARAM( "YExpand" ) ),
                              uno::Any( bYExpand ) );
    xProps->setPropertyValue( rtl::OUString ( RTL_CONSTASCII_USTRINGPARAM( "ColSpan" ) ),
                              uno::Any( nXSpan ) );
    xProps->setPropertyValue( rtl::OUString ( RTL_CONSTASCII_USTRINGPARAM( "RowSpan" ) ),
                              uno::Any( nYSpan ) );
}

Box::Box( rtl::OUString const& rName, sal_Int32 nBorder, bool bHomogeneous )
    : Container( rName, nBorder )
{
    uno::Reference< beans::XPropertySet > xProps( mxContainer, uno::UNO_QUERY_THROW );
    xProps->setPropertyValue( rtl::OUString ( RTL_CONSTASCII_USTRINGPARAM( "Homogeneous" ) ),
                              uno::Any( bHomogeneous ) );
}

void Box::Add( Window *window, bool bExpand, bool bFill, sal_Int32 nPadding)
{
    if ( !window )
        return;
    WindowImpl *pImpl = window->getImpl();
    uno::Reference< awt::XLayoutConstrains > xChild( pImpl->mxWindow,
                                                     uno::UNO_QUERY );

    mxContainer->addChild( xChild );
    setProps( xChild, bExpand, bFill, nPadding );
}

void Box::Add( Container *pContainer, bool bExpand, bool bFill, sal_Int32 nPadding)
{
    if ( !pContainer )
        return;

    uno::Reference< awt::XLayoutConstrains > xChild( pContainer->getImpl(),
                                                     uno::UNO_QUERY );
    mxContainer->addChild( xChild );
    setProps( xChild, bExpand, bFill, nPadding );
}

void Box::setProps( uno::Reference< awt::XLayoutConstrains > xChild,
                    bool bExpand, bool bFill, sal_Int32 nPadding )
{
    uno::Reference< beans::XPropertySet > xProps
        ( mxContainer->getChildProperties( xChild ), uno::UNO_QUERY_THROW );

    xProps->setPropertyValue( rtl::OUString ( RTL_CONSTASCII_USTRINGPARAM( "Expand" ) ),
                              uno::Any( bExpand ) );
    xProps->setPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "Fill" ) ),
                              uno::Any( bFill ) );
    xProps->setPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "Padding" ) ),
                              uno::Any( nPadding ) );
}

Table::Table( Context const* context, char const* pId )
    : Container( context, pId )
{
}

Box::Box( Context const* context, char const* pId )
    : Container( context, pId )
{
}

HBox::HBox( sal_Int32 nBorder, bool bHomogeneous )
    : Box( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "hbox" ) ),
           nBorder, bHomogeneous )
{
}

HBox::HBox( Context const* context, char const* pId )
    : Box( context, pId )
{
}

VBox::VBox( sal_Int32 nBorder, bool bHomogeneous )
    : Box( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "vbox" ) ),
           nBorder, bHomogeneous )
{
}

VBox::VBox( Context const* context, char const* pId )
    : Box( context, pId )
{
}

} // namespace layout
