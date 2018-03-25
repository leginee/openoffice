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
#include "precompiled_sc.hxx"



#include "global.hxx"
#include "scresid.hxx"
#include "impex.hxx"
#include "asciiopt.hxx"
#include "asciiopt.hrc"
#include <tools/debug.hxx>
#include <rtl/tencinfo.h>
#include <unotools/transliterationwrapper.hxx>
// ause
#include "editutil.hxx"

// ============================================================================

static const sal_Char __FAR_DATA pStrFix[] = "FIX";
static const sal_Char __FAR_DATA pStrMrg[] = "MRG";


// ============================================================================

ScAsciiOptions::ScAsciiOptions() :
	bFixedLen		( sal_False ),
	aFieldSeps		( ';' ),
	bMergeFieldSeps	( sal_False ),
    bQuotedFieldAsText(false),
    bDetectSpecialNumber(false),
	cTextSep		( cDefaultTextSep ),
	eCharSet		( gsl_getSystemTextEncoding() ),
    eLang           ( LANGUAGE_SYSTEM ),
	bCharSetSystem	( sal_False ),
	nStartRow		( 1 ),
	nInfoCount		( 0 ),
    pColStart       ( NULL ),
	pColFormat		( NULL )
{
}


ScAsciiOptions::ScAsciiOptions(const ScAsciiOptions& rOpt) :
	bFixedLen		( rOpt.bFixedLen ),
	aFieldSeps		( rOpt.aFieldSeps ),
	bMergeFieldSeps	( rOpt.bMergeFieldSeps ),
    bQuotedFieldAsText(rOpt.bQuotedFieldAsText),
    bDetectSpecialNumber(rOpt.bDetectSpecialNumber),
	cTextSep		( rOpt.cTextSep ),
	eCharSet		( rOpt.eCharSet ),
    eLang           ( rOpt.eLang ),
	bCharSetSystem	( rOpt.bCharSetSystem ),
	nStartRow		( rOpt.nStartRow ),
	nInfoCount		( rOpt.nInfoCount )
{
	if (nInfoCount)
	{
		pColStart = new xub_StrLen[nInfoCount];
		pColFormat = new sal_uInt8[nInfoCount];
		for (sal_uInt16 i=0; i<nInfoCount; i++)
		{
			pColStart[i] = rOpt.pColStart[i];
			pColFormat[i] = rOpt.pColFormat[i];
		}
	}
	else
	{
		pColStart = NULL;
		pColFormat = NULL;
	}
}


ScAsciiOptions::~ScAsciiOptions()
{
	delete[] pColStart;
	delete[] pColFormat;
}


void ScAsciiOptions::SetColInfo( sal_uInt16 nCount, const xub_StrLen* pStart, const sal_uInt8* pFormat )
{
	delete[] pColStart;
	delete[] pColFormat;

	nInfoCount = nCount;

	if (nInfoCount)
	{
		pColStart = new xub_StrLen[nInfoCount];
		pColFormat = new sal_uInt8[nInfoCount];
		for (sal_uInt16 i=0; i<nInfoCount; i++)
		{
			pColStart[i] = pStart[i];
			pColFormat[i] = pFormat[i];
		}
	}
	else
	{
		pColStart = NULL;
		pColFormat = NULL;
	}
}


void ScAsciiOptions::SetColumnInfo( const ScCsvExpDataVec& rDataVec )
{
	delete[] pColStart;
    pColStart = NULL;
	delete[] pColFormat;
    pColFormat = NULL;

    nInfoCount = static_cast< sal_uInt16 >( rDataVec.size() );
    if( nInfoCount )
	{
        pColStart = new xub_StrLen[ nInfoCount ];
        pColFormat = new sal_uInt8[ nInfoCount ];
        for( sal_uInt16 nIx = 0; nIx < nInfoCount; ++nIx )
		{
            pColStart[ nIx ] = rDataVec[ nIx ].mnIndex;
            pColFormat[ nIx ] = rDataVec[ nIx ].mnType;
		}
	}
}


ScAsciiOptions&	ScAsciiOptions::operator=( const ScAsciiOptions& rCpy )
{
	SetColInfo( rCpy.nInfoCount, rCpy.pColStart, rCpy.pColFormat );

	bFixedLen		= rCpy.bFixedLen;
	aFieldSeps		= rCpy.aFieldSeps;
	bMergeFieldSeps	= rCpy.bMergeFieldSeps;
    bQuotedFieldAsText = rCpy.bQuotedFieldAsText;
	cTextSep		= rCpy.cTextSep;
	eCharSet		= rCpy.eCharSet;
	bCharSetSystem	= rCpy.bCharSetSystem;
	nStartRow		= rCpy.nStartRow;

	return *this;
}


sal_Bool ScAsciiOptions::operator==( const ScAsciiOptions& rCmp ) const
{
	if ( bFixedLen		 == rCmp.bFixedLen &&
		 aFieldSeps		 == rCmp.aFieldSeps &&
		 bMergeFieldSeps == rCmp.bMergeFieldSeps &&
         bQuotedFieldAsText == rCmp.bQuotedFieldAsText &&
		 cTextSep		 == rCmp.cTextSep &&
		 eCharSet		 == rCmp.eCharSet &&
		 bCharSetSystem  == rCmp.bCharSetSystem &&
		 nStartRow		 == rCmp.nStartRow &&
		 nInfoCount		 == rCmp.nInfoCount )
	{
		DBG_ASSERT( !nInfoCount || (pColStart && pColFormat && rCmp.pColStart && rCmp.pColFormat),
					 "0-Zeiger in ScAsciiOptions" );
		for (sal_uInt16 i=0; i<nInfoCount; i++)
			if ( pColStart[i] != rCmp.pColStart[i] ||
				 pColFormat[i] != rCmp.pColFormat[i] )
				return sal_False;

		return sal_True;
	}
	return sal_False;
}

//
//	Der Options-String darf kein Semikolon mehr enthalten (wegen Pickliste)
//	darum ab Version 336 Komma stattdessen
//


void ScAsciiOptions::ReadFromString( const String& rString )
{
	xub_StrLen nCount = rString.GetTokenCount(',');
	String aToken;
	xub_StrLen nSub;
	xub_StrLen i;

		//
		//	Feld-Trenner
		//

	if ( nCount >= 1 )
	{
		bFixedLen = bMergeFieldSeps = sal_False;
		aFieldSeps.Erase();

		aToken = rString.GetToken(0,',');
		if ( aToken.EqualsAscii(pStrFix) )
			bFixedLen = sal_True;
		nSub = aToken.GetTokenCount('/');
		for ( i=0; i<nSub; i++ )
		{
			String aCode = aToken.GetToken( i, '/' );
			if ( aCode.EqualsAscii(pStrMrg) )
				bMergeFieldSeps = sal_True;
			else
			{
				sal_Int32 nVal = aCode.ToInt32();
				if ( nVal )
					aFieldSeps += (sal_Unicode) nVal;
			}
		}
	}

		//
		//	Text-Trenner
		//

	if ( nCount >= 2 )
	{
		aToken = rString.GetToken(1,',');
		sal_Int32 nVal = aToken.ToInt32();
		cTextSep = (sal_Unicode) nVal;
	}

		//
		//	Zeichensatz
		//

	if ( nCount >= 3 )
	{
		aToken = rString.GetToken(2,',');
		eCharSet = ScGlobal::GetCharsetValue( aToken );
	}

		//
		//	Startzeile
		//

    if ( nCount >= 4 )
	{
        aToken = rString.GetToken(3,',');
		nStartRow = aToken.ToInt32();
	}

		//
		//	Spalten-Infos
		//

    if ( nCount >= 5 )
	{
		delete[] pColStart;
		delete[] pColFormat;

        aToken = rString.GetToken(4,',');
		nSub = aToken.GetTokenCount('/');
		nInfoCount = nSub / 2;
		if (nInfoCount)
		{
			pColStart = new xub_StrLen[nInfoCount];
			pColFormat = new sal_uInt8[nInfoCount];
			for (sal_uInt16 nInfo=0; nInfo<nInfoCount; nInfo++)
			{
				pColStart[nInfo]  = (xub_StrLen) aToken.GetToken( 2*nInfo, '/' ).ToInt32();
				pColFormat[nInfo] = (sal_uInt8) aToken.GetToken( 2*nInfo+1, '/' ).ToInt32();
			}
		}
		else
		{
			pColStart = NULL;
			pColFormat = NULL;
		}
	}

    // Language
    if (nCount >= 6)
    {    
        aToken = rString.GetToken(5, ',');
        eLang = static_cast<LanguageType>(aToken.ToInt32());
    }

    // Import quoted field as text.
    if (nCount >= 7)
    {
        aToken = rString.GetToken(6, ',');
        bQuotedFieldAsText = aToken.EqualsAscii("true") ? true : false;
    }

    // Detect special numbers.
    if (nCount >= 8)
    {
        aToken = rString.GetToken(7, ',');
        bDetectSpecialNumber = aToken.EqualsAscii("true") ? true : false;
    }
    else
        bDetectSpecialNumber = sal_True;    // default of versions that didn't add the parameter

    // 9th token is used for "Save as shown" in export options
}


String ScAsciiOptions::WriteToString() const
{
	String aOutStr;

		//
		//	Feld-Trenner
		//

	if ( bFixedLen )
		aOutStr.AppendAscii(pStrFix);
	else if ( !aFieldSeps.Len() )
		aOutStr += '0';
	else
	{
		xub_StrLen nLen = aFieldSeps.Len();
		for (xub_StrLen i=0; i<nLen; i++)
		{
			if (i)
				aOutStr += '/';
			aOutStr += String::CreateFromInt32(aFieldSeps.GetChar(i));
		}
		if ( bMergeFieldSeps )
		{
			aOutStr += '/';
			aOutStr.AppendAscii(pStrMrg);
		}
	}

	aOutStr += ',';					// Token-Ende

		//
		//	Text-Trenner
		//

	aOutStr += String::CreateFromInt32(cTextSep);
	aOutStr += ',';					// Token-Ende

		//
		//	Zeichensatz
		//

	if ( bCharSetSystem )			// force "SYSTEM"
		aOutStr += ScGlobal::GetCharsetString( RTL_TEXTENCODING_DONTKNOW );
	else
		aOutStr += ScGlobal::GetCharsetString( eCharSet );
	aOutStr += ',';					// Token-Ende

		//
		//	Startzeile
		//

	aOutStr += String::CreateFromInt32(nStartRow);
	aOutStr += ',';					// Token-Ende

		//
		//	Spalten-Infos
		//

	DBG_ASSERT( !nInfoCount || (pColStart && pColFormat), "0-Zeiger in ScAsciiOptions" );
	for (sal_uInt16 nInfo=0; nInfo<nInfoCount; nInfo++)
	{
		if (nInfo)
			aOutStr += '/';
		aOutStr += String::CreateFromInt32(pColStart[nInfo]);
		aOutStr += '/';
		aOutStr += String::CreateFromInt32(pColFormat[nInfo]);
	}

    // #i112025# the options string is used in macros and linked sheets,
    // so new options must be added at the end, to remain compatible

    aOutStr += ',';

    // Language
    aOutStr += String::CreateFromInt32(eLang);
    aOutStr += ',';

    // Import quoted field as text.
    aOutStr += String::CreateFromAscii(bQuotedFieldAsText ? "true" : "false");
    aOutStr += ',';

    // Detect special numbers.
    aOutStr += String::CreateFromAscii(bDetectSpecialNumber ? "true" : "false");

    // 9th token is used for "Save as shown" in export options

	return aOutStr;
}

#if 0
//	Code, um die Spalten-Liste aus einem Excel-kompatiblen String zu erzeugen:
//	(im Moment nicht benutzt)

void ScAsciiOptions::InterpretColumnList( const String& rString )
{
	//	Eingabe ist 1-basiert, pColStart fuer FixedLen ist 0-basiert

	//	Kommas durch Semikolon ersetzen

	String aSemiStr = rString;
	sal_uInt16 nPos = 0;
	do
		nPos = aSemiStr.SearchAndReplace( ',', ';', nPos );
	while ( nPos != STRING_NOTFOUND );

	//	Eintraege sortieren

	sal_uInt16 nCount = aSemiStr.GetTokenCount();
	sal_uInt16* pTemp = new sal_uInt16[nCount+1];
	pTemp[0] = 1;									// erste Spalte faengt immer bei 1 an
	sal_uInt16 nFound = 1;
	sal_uInt16 i,j;
	for (i=0; i<nCount; i++)
	{
		sal_uInt16 nVal = (sal_uInt16) aSemiStr.GetToken(i);
		if (nVal)
		{
			sal_Bool bThere = sal_False;
			nPos = 0;
			for (j=0; j<nFound; j++)
			{
				if ( pTemp[j] == nVal )
					bThere = sal_True;
				else if ( pTemp[j] < nVal )
					nPos = j+1;
			}
			if ( !bThere )
			{
				if ( nPos < nFound )
					memmove( &pTemp[nPos+1], &pTemp[nPos], (nFound-nPos)*sizeof(sal_uInt16) );
				pTemp[nPos] = nVal;
				++nFound;
			}
		}
	}

	//	Eintraege uebernehmen

	delete[] pColStart;
	delete[] pColFormat;
	nInfoCount = nFound;
	if (nInfoCount)
	{
		pColStart = new sal_uInt16[nInfoCount];
		pColFormat = new sal_uInt8[nInfoCount];
		for (i=0; i<nInfoCount; i++)
		{
			pColStart[i] = pTemp[i] - 1;
			pColFormat[i] = SC_COL_STANDARD;
		}
	}
	else
	{
		pColStart = NULL;
		pColFormat = NULL;
	}

	bFixedLen = sal_True;			// sonst macht's keinen Sinn

	//	aufraeumen

	delete[] pTemp;
}
#endif

