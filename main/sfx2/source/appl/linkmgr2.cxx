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

// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_sfx2.hxx"

#include <sfx2/linkmgr.hxx>
#include <com/sun/star/document/UpdateDocMode.hpp>
#include <sfx2/objsh.hxx>
#include <svl/urihelper.hxx>
#include <sot/formats.hxx>
#include <tools/urlobj.hxx>
#include <sot/exchange.hxx>
#include <tools/debug.hxx>
#include <vcl/msgbox.hxx>
#include <sfx2/lnkbase.hxx>
#include <sfx2/app.hxx>
#include <vcl/graph.hxx>
#include <svl/stritem.hxx>
#include <svl/eitem.hxx>
#include <svl/intitem.hxx>
#include <unotools/localfilehelper.hxx>
#include <i18npool/mslangid.hxx>
#include <sfx2/request.hxx>
#include <vcl/dibtools.hxx>

#include "fileobj.hxx"
#include "impldde.hxx"
#include "app.hrc"
#include "sfx2/sfxresid.hxx"

#define _SVSTDARR_STRINGSDTOR
#include <svl/svstdarr.hxx>

namespace sfx2
{

class SvxInternalLink : public sfx2::SvLinkSource
{
public:
	SvxInternalLink() {}

    virtual sal_Bool Connect( sfx2::SvBaseLink* );
};


SV_IMPL_PTRARR( SvBaseLinks, SvBaseLinkRefPtr )

LinkManager::LinkManager(SfxObjectShell* p)
	: pPersist(p),
	mUpdateAsked(sal_False),
	mAutoAskUpdateAllLinks(sal_False)
{
}

LinkManager::~LinkManager()
{
	SvBaseLinkRef** ppRef = (SvBaseLinkRef**)aLinkTbl.GetData();
	for( sal_uInt16 n = aLinkTbl.Count(); n; --n, ++ppRef )
	{
		if( (*ppRef)->Is() )
		{
			(*(*ppRef))->Disconnect();
            (*(*ppRef))->SetLinkManager( NULL );
		}
		delete *ppRef;
	}
}


/************************************************************************
|*    LinkManager::Remove()
|*
|*    Description
*************************************************************************/

void LinkManager::Remove( SvBaseLink *pLink )
{
	// do not insert links double
	int bFound = sal_False;
	SvBaseLinkRef** ppRef = (SvBaseLinkRef**)aLinkTbl.GetData();
	for( sal_uInt16 n = aLinkTbl.Count(); n; --n, ++ppRef )
	{
		if( pLink == *(*ppRef) )
		{
			(*(*ppRef))->Disconnect();
            (*(*ppRef))->SetLinkManager( NULL );
			(*(*ppRef)).Clear();
			bFound = sal_True;
		}

		// if there are still some empty ones, get rid of them
		if( !(*ppRef)->Is() )
		{
			delete *ppRef;
			aLinkTbl.Remove( aLinkTbl.Count() - n, 1 );
			if( bFound )
				return ;
			--ppRef;
		}
	}
}


void LinkManager::Remove( sal_uInt16 nPos, sal_uInt16 nCnt )
{
	if( nCnt && nPos < aLinkTbl.Count() )
	{
		if( nPos + nCnt > aLinkTbl.Count() )
			nCnt = aLinkTbl.Count() - nPos;

		SvBaseLinkRef** ppRef = (SvBaseLinkRef**)aLinkTbl.GetData() + nPos;
		for( sal_uInt16 n = nCnt; n; --n, ++ppRef )
		{
			if( (*ppRef)->Is() )
			{
				(*(*ppRef))->Disconnect();
                (*(*ppRef))->SetLinkManager( NULL );
			}
			delete *ppRef;
		}
		aLinkTbl.Remove( nPos, nCnt );
	}
}


sal_Bool LinkManager::Insert( SvBaseLink* pLink )
{

	// do not insert links double
	for( sal_uInt16 n = 0; n < aLinkTbl.Count(); ++n )
	{
		SvBaseLinkRef* pTmp = aLinkTbl[ n ];
		if( !pTmp->Is() )
			aLinkTbl.DeleteAndDestroy( n-- );

		if( pLink == *pTmp )
			return sal_False;
	}

	SvBaseLinkRef* pTmp = new SvBaseLinkRef( pLink );
    pLink->SetLinkManager( this );
	aLinkTbl.Insert( pTmp, aLinkTbl.Count() );
	if (mAutoAskUpdateAllLinks)
	{
		Window *parent = NULL;
		SfxObjectShell* persist = GetPersist();
		if (persist != NULL)
			parent = GetPersist()->GetDialogParent();

		SetUserAllowsLinkUpdate(pLink, GetUserAllowsLinkUpdate(parent));
	}

	return sal_True;
}


sal_Bool LinkManager::InsertLink( SvBaseLink * pLink,
								sal_uInt16 nObjType,
								sal_uInt16 nUpdateMode,
								const String* pName )
{
	// in any case: do this first
	pLink->SetObjType( nObjType );
	if( pName )
		pLink->SetName( *pName );
	pLink->SetUpdateMode( nUpdateMode );
	return Insert( pLink );
}


sal_Bool LinkManager::InsertDDELink( SvBaseLink * pLink,
									const String& rServer,
									const String& rTopic,
									const String& rItem )
{
	if( !( OBJECT_CLIENT_SO & pLink->GetObjType() ) )
		return sal_False;

	String sCmd;
	::sfx2::MakeLnkName( sCmd, &rServer, rTopic, rItem );

	pLink->SetObjType( OBJECT_CLIENT_DDE );
	pLink->SetName( sCmd );
	return Insert( pLink );
}


sal_Bool LinkManager::InsertDDELink( SvBaseLink * pLink )
{
	DBG_ASSERT( OBJECT_CLIENT_SO & pLink->GetObjType(), "no OBJECT_CLIENT_SO" );
	if( !( OBJECT_CLIENT_SO & pLink->GetObjType() ) )
		return sal_False;

	if( pLink->GetObjType() == OBJECT_CLIENT_SO )
		pLink->SetObjType( OBJECT_CLIENT_DDE );

	return Insert( pLink );
}


// ask for the strings to be used in the dialog
sal_Bool LinkManager::GetDisplayNames( const SvBaseLink * pLink,
										String* pType,
										String* pFile,
										String* pLinkStr,
										String* pFilter ) const
{
	sal_Bool bRet = sal_False;
	const String sLNm( pLink->GetLinkSourceName() );
	if( sLNm.Len() )
	{
		switch( pLink->GetObjType() )
		{
		    case OBJECT_CLIENT_FILE:
		    case OBJECT_CLIENT_GRF:
		    case OBJECT_CLIENT_OLE:
			    {
				    sal_uInt16 nPos = 0;
				    String sFile( sLNm.GetToken( 0, ::sfx2::cTokenSeperator, nPos ) );
				    String sRange( sLNm.GetToken( 0, ::sfx2::cTokenSeperator, nPos ) );

				    if( pFile )
					    *pFile = sFile;
				    if( pLinkStr )
					    *pLinkStr = sRange;
				    if( pFilter )
					    *pFilter = sLNm.Copy( nPos );

				    if( pType )
				    {
					    sal_uInt16 nObjType = pLink->GetObjType();
					    *pType = String( SfxResId(
								    ( OBJECT_CLIENT_FILE == nObjType || OBJECT_CLIENT_OLE == nObjType )
										    ? RID_SVXSTR_FILELINK
										    : RID_SVXSTR_GRAFIKLINK ));
				    }
				    bRet = sal_True;
			    }
			    break;
		    case OBJECT_CLIENT_DDE:
	            {
		            sal_uInt16 nTmp = 0;
		            String sCmd( sLNm );
		            String sServer( sCmd.GetToken( 0, cTokenSeperator, nTmp ) );
		            String sTopic( sCmd.GetToken( 0, cTokenSeperator, nTmp ) );

		            if( pType )
			            *pType = sServer;
		            if( pFile )
			            *pFile = sTopic;
		            if( pLinkStr )
			            *pLinkStr = sCmd.Copy( nTmp );
		            bRet = sal_True;
		        }
		        break;
	        default:
	            break;
	    }
	}
	
	return bRet;
}

void LinkManager::SetAutoAskUpdateAllLinks()
{
	mAutoAskUpdateAllLinks = sal_True;
}

sal_Bool LinkManager::GetUserAllowsLinkUpdate(Window *pParentWin)
{
	if (!mUpdateAsked)
	{
		if (QueryBox(pParentWin, WB_YES_NO | WB_DEF_NO, SfxResId(STR_QUERY_UPDATE_LINKS)).Execute() == RET_YES)
			mAllowUpdate = sal_True;
		else
			mAllowUpdate = sal_False;
		mUpdateAsked = sal_True;
	}
	return mAllowUpdate;
}

void LinkManager::SetUserAllowsLinkUpdate(SvBaseLink *pLink, sal_Bool allows)
{
	SfxObjectShell* pShell = pLink->GetLinkManager()->GetPersist();

	if (pShell)
	{
		comphelper::EmbeddedObjectContainer& rEmbeddedObjectContainer = pShell->getEmbeddedObjectContainer();
		rEmbeddedObjectContainer.setUserAllowsLinkUpdate(allows);
	}
}


void LinkManager::UpdateAllLinks( 
    sal_Bool bAskUpdate, 
    sal_Bool /*bCallErrHdl*/,
    sal_Bool bUpdateGrfLinks, 
    Window* pParentWin )
{
	SvStringsDtor aApps, aTopics, aItems;
	String sApp, sTopic, sItem;

	// first create a copy of the array, so that updated links to not interfere with ... in between!!
	SvPtrarr aTmpArr( 255, 50 );
	sal_uInt16 n;
	for( n = 0; n < aLinkTbl.Count(); ++n )
	{
		SvBaseLink* pLink = *aLinkTbl[ n ];
		if( !pLink )
		{
			Remove( n-- );
			continue;
		}
		aTmpArr.Insert( pLink, aTmpArr.Count() );
	}

	for( n = 0; n < aTmpArr.Count(); ++n )
	{
		SvBaseLink* pLink = (SvBaseLink*)aTmpArr[ n ];

		// first search the entry in the array
		sal_uInt16 nFndPos = USHRT_MAX;
		for( sal_uInt16 i = 0; i < aLinkTbl.Count(); ++i )
			if( pLink == *aLinkTbl[ i ] )
			{
				nFndPos = i;
				break;
			}

		if( USHRT_MAX == nFndPos )
			continue;					// was not already existing!

		// do not update graphic links yet
		if( !pLink->IsVisible() ||
			( !bUpdateGrfLinks && OBJECT_CLIENT_GRF == pLink->GetObjType() ))
			continue;

		sal_Bool allows = sal_True;
			
		if (bAskUpdate)
		{
			allows = GetUserAllowsLinkUpdate(pParentWin);
		}

		SetUserAllowsLinkUpdate(pLink, allows);
	    bAskUpdate = sal_False;		// one time is OK

		if (allows)
			pLink->Update();
	}
}

/************************************************************************
|*    SvBaseLink::CreateObject()
|*
|*    Description
*************************************************************************/

SvLinkSourceRef LinkManager::CreateObj( SvBaseLink * pLink )
{
	switch( pLink->GetObjType() )
	{
	    case OBJECT_CLIENT_FILE:
	    case OBJECT_CLIENT_GRF:
	    case OBJECT_CLIENT_OLE:
		    return new SvFileObject;
	    case OBJECT_INTERN:
		    return new SvxInternalLink;
        case OBJECT_CLIENT_DDE:
		    return new SvDDEObject;
	    default:
        	return SvLinkSourceRef();
   	}
}

sal_Bool LinkManager::InsertServer( SvLinkSource* pObj )
{
	// do not insert double
	if( !pObj || USHRT_MAX != aServerTbl.GetPos( pObj ) )
		return sal_False;

	aServerTbl.Insert( pObj, aServerTbl.Count() );
	return sal_True;
}


void LinkManager::RemoveServer( SvLinkSource* pObj )
{
	sal_uInt16 nPos = aServerTbl.GetPos( pObj );
	if( USHRT_MAX != nPos )
		aServerTbl.Remove( nPos, 1 );
}


void MakeLnkName( String& rName, const String* pType, const String& rFile,
					const String& rLink, const String* pFilter )
{
	if( pType )
		(rName = *pType).EraseLeadingChars().EraseTrailingChars() += cTokenSeperator;
	else if( rName.Len() )
		rName.Erase();

	((rName += rFile).EraseLeadingChars().EraseTrailingChars() +=
		cTokenSeperator ).EraseLeadingChars().EraseTrailingChars() += rLink;
	if( pFilter )
		((rName += cTokenSeperator ) += *pFilter).EraseLeadingChars().EraseTrailingChars();
}

sal_Bool LinkManager::InsertFileLink( sfx2::SvBaseLink& rLink,
									sal_uInt16 nFileType,
									const String& rFileNm,
									const String* pFilterNm,
									const String* pRange )
{
	if( !( OBJECT_CLIENT_SO & rLink.GetObjType() ))
		return sal_False;

	String sCmd( rFileNm );
	sCmd += ::sfx2::cTokenSeperator;
	if( pRange )
		sCmd += *pRange;
	if( pFilterNm )
		( sCmd += ::sfx2::cTokenSeperator ) += *pFilterNm;

	return InsertLink( &rLink, nFileType, sfx2::LINKUPDATE_ONCALL, &sCmd );
}

sal_Bool LinkManager::InsertFileLink( sfx2::SvBaseLink& rLink )
{
	if( OBJECT_CLIENT_FILE == ( OBJECT_CLIENT_FILE & rLink.GetObjType() ))
		return InsertLink( &rLink, rLink.GetObjType(), sfx2::LINKUPDATE_ONCALL );
	return sal_False;
}

// a transfer will be discontinued, therefore cancel all DownloadMedia
// (at the moment only interesting for the FileLinks!)
void LinkManager::CancelTransfers()
{
	SvFileObject* pFileObj;
	sfx2::SvBaseLink* pLnk;

	const sfx2::SvBaseLinks& rLnks = GetLinks();
	for( sal_uInt16 n = rLnks.Count(); n; )
		if( 0 != ( pLnk = &(*rLnks[ --n ])) &&
			OBJECT_CLIENT_FILE == (OBJECT_CLIENT_FILE & pLnk->GetObjType()) &&
			0 != ( pFileObj = (SvFileObject*)pLnk->GetObj() ) )
//			0 != ( pFileObj = (SvFileObject*)SvFileObject::ClassFactory()->
//									CastAndAddRef( pLnk->GetObj() )) )
			pFileObj->CancelTransfers();
}

	// to send status information from the FileObject to the BaseLink, there is an own ClipboardId.
	// The SvData object has then the respective information as string.
	// Currently this will be used for FileObject in connection with JavaScript
	// - that needs information about Load/Abort/Error
sal_uIntPtr LinkManager::RegisterStatusInfoId()
{
	static sal_uIntPtr nFormat = 0;

	if( !nFormat )
	{
// how does the new interface look like?
//		nFormat = Exchange::RegisterFormatName( "StatusInfo vom SvxInternalLink" );
		nFormat = SotExchange::RegisterFormatName(
					String::CreateFromAscii( RTL_CONSTASCII_STRINGPARAM(
								"StatusInfo vom SvxInternalLink" )));
	}
	return nFormat;
}

// ----------------------------------------------------------------------

sal_Bool LinkManager::GetGraphicFromAny( const String& rMimeType,
								const ::com::sun::star::uno::Any & rValue,
								Graphic& rGrf )
{
	sal_Bool bRet = sal_False;
	::com::sun::star::uno::Sequence< sal_Int8 > aSeq;
	if( rValue.hasValue() && ( rValue >>= aSeq ) )
	{
		SvMemoryStream aMemStm( (void*)aSeq.getConstArray(), aSeq.getLength(),
								STREAM_READ );
		aMemStm.Seek( 0 );

		switch( SotExchange::GetFormatIdFromMimeType( rMimeType ) )
		{
		case SOT_FORMATSTR_ID_SVXB:
			{
				aMemStm >> rGrf;
				bRet = sal_True;
			}
			break;
		case FORMAT_GDIMETAFILE:
			{
				GDIMetaFile aMtf;
				aMtf.Read( aMemStm );
				rGrf = aMtf;
				bRet = sal_True;
			}
			break;
		case FORMAT_BITMAP:
			{
				Bitmap aBmp;
                ReadDIB(aBmp, aMemStm, true);
				rGrf = aBmp;
				bRet = sal_True;
			}
			break;
		}
	}
	return bRet;
}


// ----------------------------------------------------------------------
String lcl_DDE_RelToAbs( const String& rTopic, const String& rBaseURL )
{
	String sRet;
	INetURLObject aURL( rTopic );
	if( INET_PROT_NOT_VALID == aURL.GetProtocol() )
        utl::LocalFileHelper::ConvertSystemPathToURL( rTopic, rBaseURL, sRet );
	if( !sRet.Len() )
        sRet = URIHelper::SmartRel2Abs( INetURLObject(rBaseURL), rTopic, URIHelper::GetMaybeFileHdl(), true );
	return sRet;
}

sal_Bool SvxInternalLink::Connect( sfx2::SvBaseLink* pLink )
{
	SfxObjectShell* pFndShell = 0;
	sal_uInt16 nUpdateMode = com::sun::star::document::UpdateDocMode::NO_UPDATE;
	String sTopic, sItem, sReferer;
	if( pLink->GetLinkManager() &&
		pLink->GetLinkManager()->GetDisplayNames( pLink, 0, &sTopic, &sItem )
		&& sTopic.Len() )
	{
		// for the moment run through the DocumentShells and search for the ones with names:

	    com::sun::star::lang::Locale aLocale;
	    MsLangId::convertLanguageToLocale( LANGUAGE_SYSTEM, aLocale );
		CharClass aCC( aLocale );
		
        String sNm( sTopic ), sTmp;
		aCC.toLower( sNm );

		TypeId aType( TYPE(SfxObjectShell) );

		sal_Bool bFirst = sal_True;
        SfxObjectShell* pShell = pLink->GetLinkManager()->GetPersist();
        if( pShell && pShell->GetMedium() )
		{
            sReferer = pShell->GetMedium()->GetBaseURL();
			SFX_ITEMSET_ARG( pShell->GetMedium()->GetItemSet(), pItem, SfxUInt16Item, SID_UPDATEDOCMODE, sal_False );
			if ( pItem )
				nUpdateMode = pItem->GetValue();
		}

        String sNmURL( lcl_DDE_RelToAbs( sTopic, sReferer ) );
		aCC.toLower( sNmURL );

		if ( !pShell )
		{
			bFirst = sal_False;
            pShell = SfxObjectShell::GetFirst( &aType, sal_False );
		}

		while( pShell )
		{
			if( !sTmp.Len() )
			{
				sTmp = pShell->GetTitle( SFX_TITLE_FULLNAME );
                sTmp = lcl_DDE_RelToAbs(sTmp, sReferer );
			}


			aCC.toLower( sTmp );
			if( sTmp == sNmURL )		// these we want to have
			{
				pFndShell = pShell;
				break;
			}

			if( bFirst )
			{
				bFirst = sal_False;
                pShell = SfxObjectShell::GetFirst( &aType, sal_False );
			}
			else
                pShell = SfxObjectShell::GetNext( *pShell, &aType, sal_False );

			sTmp.Erase();
		}
	}

	// empty topics are not allowed - which document is it
	if( !sTopic.Len() )
		return sal_False;

	if( !pFndShell )
	{
		// try to load the file:
		INetURLObject aURL( sTopic );
		INetProtocol eOld = aURL.GetProtocol();
        aURL.SetURL( sTopic = lcl_DDE_RelToAbs( sTopic, sReferer ) );
		if( INET_PROT_NOT_VALID != eOld ||
			INET_PROT_HTTP != aURL.GetProtocol() )
		{
			SfxStringItem aName( SID_FILE_NAME, sTopic );
            SfxBoolItem aMinimized(SID_MINIMIZED, sal_True);
            SfxBoolItem aHidden(SID_HIDDEN, sal_True);
            SfxStringItem aTarget( SID_TARGETNAME, String::CreateFromAscii("_blank") );
			SfxStringItem aReferer( SID_REFERER, sReferer );
			SfxUInt16Item aUpdate( SID_UPDATEDOCMODE, nUpdateMode );
            SfxBoolItem aReadOnly(SID_DOC_READONLY, sal_True);

            // #i14200# (DDE-link crashes wordprocessor)
            SfxAllItemSet aArgs( SFX_APP()->GetPool() );
            aArgs.Put(aReferer);
            aArgs.Put(aTarget);
            aArgs.Put(aHidden);
            aArgs.Put(aMinimized);
            aArgs.Put(aName);
			aArgs.Put(aUpdate);
			aArgs.Put(aReadOnly);
            pFndShell = SfxObjectShell::CreateAndLoadObject( aArgs );
		}
	}

	sal_Bool bRet = sal_False;
	if( pFndShell )
	{
		sfx2::SvLinkSource* pNewSrc = pFndShell->DdeCreateLinkSource( sItem );
		if( pNewSrc )
		{
			bRet = sal_True;

			::com::sun::star::datatransfer::DataFlavor aFl;
			SotExchange::GetFormatDataFlavor( pLink->GetContentType(), aFl );

			pLink->SetObj( pNewSrc );
			pNewSrc->AddDataAdvise( pLink, aFl.MimeType,
								sfx2::LINKUPDATE_ONCALL == pLink->GetUpdateMode()
									? ADVISEMODE_ONLYONCE
									: 0 );
		}
	}
	return bRet;
}


}

