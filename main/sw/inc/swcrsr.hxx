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


#ifndef _SWCRSR_HXX
#define _SWCRSR_HXX

#include <com/sun/star/i18n/WordType.hpp>

#include <pam.hxx>
#include <tblsel.hxx>
#include <cshtyp.hxx>


struct _SwCursor_SavePos;

namespace com { namespace sun { namespace star { namespace util {
	struct SearchOptions;
} } } }


// ein Basis-Struktur fuer die Parameter der Find-Methoden
// return - Werte vom Found-Aufruf.
const int FIND_NOT_FOUND	= 0;
const int FIND_FOUND		= 1;
const int FIND_NO_RING		= 2;

struct SwFindParas
{
	virtual int Find( SwPaM*, SwMoveFn, const SwPaM*, sal_Bool ) = 0;
	virtual int IsReplaceMode() const = 0;
};

typedef sal_uInt16 SwCursorSelOverFlags;
namespace nsSwCursorSelOverFlags
{
    const SwCursorSelOverFlags SELOVER_NONE                = 0x00;
    const SwCursorSelOverFlags SELOVER_CHECKNODESSECTION   = 0x01;
    const SwCursorSelOverFlags SELOVER_TOGGLE              = 0x02;
    const SwCursorSelOverFlags SELOVER_ENABLEREVDIREKTION  = 0x04;
    const SwCursorSelOverFlags SELOVER_CHANGEPOS           = 0x08;
}

class SwCursor : public SwPaM
{
	friend class SwCrsrSaveState;

	_SwCursor_SavePos* pSavePos;
    long mnRowSpanOffset;        // required for travelling in tabs with rowspans
    sal_uInt8 nCursorBidiLevel;       // bidi level of the cursor
    bool mbColumnSelection;      // true: cursor is aprt of a column selection

    sal_uLong FindAll( SwFindParas& , SwDocPositions, SwDocPositions, FindRanges, sal_Bool& bCancel );

    using SwPaM::Find;

protected:
	virtual _SwCursor_SavePos* CreateNewSavePos() const;
	void SaveState();
	void RestoreState();

	const _SwCursor_SavePos* GetSavePos() const { return pSavePos; }

    virtual const SwCntntFrm* DoSetBidiLevelLeftRight(
        sal_Bool & io_rbLeft, sal_Bool bVisualAllowed, sal_Bool bInsertCrsr);
    virtual void DoSetBidiLevelUpDown();
    virtual bool IsSelOvrCheck(int eFlags);

public:
	// single argument ctors shall be explicit.
	SwCursor( const SwPosition &rPos, SwPaM* pRing, bool bColumnSel );
	virtual ~SwCursor();

	// @@@ semantic: no copy ctor.
	SwCursor( SwCursor& rCpy);

public:

	virtual SwCursor* Create( SwPaM* pRing = 0 ) const;

    virtual short MaxReplaceArived(); //returns RET_YES/RET_CANCEL/RET_NO
    virtual void SaveTblBoxCntnt( const SwPosition* pPos = 0 );

	void FillFindPos( SwDocPositions ePos, SwPosition& rPos ) const;
	SwMoveFnCollection* MakeFindRange( SwDocPositions, SwDocPositions,
										SwPaM* ) const;


	sal_uLong Find( const com::sun::star::util::SearchOptions& rSearchOpt, 
				sal_Bool bSearchInNotes,
				SwDocPositions nStart, SwDocPositions nEnde,
                sal_Bool& bCancel,
                FindRanges = FND_IN_BODY,
				int bReplace = sal_False );
	sal_uLong Find( const SwTxtFmtColl& rFmtColl,
				SwDocPositions nStart, SwDocPositions nEnde,
                sal_Bool& bCancel,
                FindRanges = FND_IN_BODY,
				const SwTxtFmtColl* pReplFmt = 0 );
	sal_uLong Find( const SfxItemSet& rSet, sal_Bool bNoCollections,
				SwDocPositions nStart, SwDocPositions nEnde,
                sal_Bool& bCancel,
                FindRanges = FND_IN_BODY,
				const com::sun::star::util::SearchOptions* pSearchOpt = 0,
				const SfxItemSet* rReplSet = 0 );

    // UI versions
    sal_Bool IsStartWord( sal_Int16 nWordType = com::sun::star::i18n::WordType::ANYWORD_IGNOREWHITESPACES ) const;
    sal_Bool IsEndWord( sal_Int16 nWordType = com::sun::star::i18n::WordType::ANYWORD_IGNOREWHITESPACES ) const;
    sal_Bool IsInWord( sal_Int16 nWordType = com::sun::star::i18n::WordType::ANYWORD_IGNOREWHITESPACES ) const;
    sal_Bool IsStartEndSentence( bool bEnd ) const;
    sal_Bool GoStartWord();
    sal_Bool GoEndWord();
    sal_Bool GoNextWord();
    sal_Bool GoPrevWord();
    sal_Bool SelectWord( ViewShell* pViewShell, const Point* pPt = 0 );

    // API versions of above functions (will be used with a different
    // WordType for the break iterator)
    sal_Bool IsStartWordWT( sal_Int16 nWordType ) const;
    sal_Bool IsEndWordWT( sal_Int16 nWordType ) const;
    sal_Bool IsInWordWT( sal_Int16 nWordType ) const;
    sal_Bool GoStartWordWT( sal_Int16 nWordType );
    sal_Bool GoEndWordWT( sal_Int16 nWordType );
    sal_Bool GoNextWordWT( sal_Int16 nWordType );
    sal_Bool GoPrevWordWT( sal_Int16 nWordType );
    sal_Bool SelectWordWT( ViewShell* pViewShell, sal_Int16 nWordType, const Point* pPt = 0 );

	enum SentenceMoveType
	{
		NEXT_SENT,
		PREV_SENT,
		START_SENT,
		END_SENT
	};
    sal_Bool GoSentence(SentenceMoveType eMoveType);
	sal_Bool GoNextSentence(){return GoSentence(NEXT_SENT);}
    sal_Bool GoEndSentence(){return GoSentence(END_SENT);}
    sal_Bool GoPrevSentence(){return GoSentence(PREV_SENT);}
    sal_Bool GoStartSentence(){return GoSentence(START_SENT);}
    sal_Bool ExpandToSentenceBorders();

    virtual sal_Bool LeftRight( sal_Bool bLeft, sal_uInt16 nCnt, sal_uInt16 nMode,
        sal_Bool bAllowVisual, sal_Bool bSkipHidden, sal_Bool bInsertCrsr );
	sal_Bool UpDown( sal_Bool bUp, sal_uInt16 nCnt, Point* pPt, long nUpDownX );
	sal_Bool LeftRightMargin( sal_Bool bLeftMargin, sal_Bool bAPI = sal_False );
	sal_Bool IsAtLeftRightMargin( sal_Bool bLeftMargin, sal_Bool bAPI = sal_False ) const;
	sal_Bool SttEndDoc( sal_Bool bSttDoc );
	sal_Bool GoPrevNextCell( sal_Bool bNext, sal_uInt16 nCnt );

    sal_Bool Left( sal_uInt16 nCnt, sal_uInt16 nMode, sal_Bool bAllowVisual, sal_Bool bSkipHidden )
                                    { return LeftRight( sal_True, nCnt, nMode, bAllowVisual, bSkipHidden, sal_False ); }
    sal_Bool Right( sal_uInt16 nCnt, sal_uInt16 nMode, sal_Bool bAllowVisual, sal_Bool bSkipHidden )
                                    { return LeftRight( sal_False, nCnt, nMode, bAllowVisual, bSkipHidden, sal_False ); }
    sal_Bool GoNextCell( sal_uInt16 nCnt = 1 )  { return GoPrevNextCell( sal_True, nCnt ); }
	sal_Bool GoPrevCell( sal_uInt16 nCnt = 1 )	{ return GoPrevNextCell( sal_False, nCnt ); }
    virtual sal_Bool GotoTable( const String& rName );
	sal_Bool GotoTblBox( const String& rName );
	sal_Bool GotoRegion( const String& rName );
	sal_Bool GotoFtnAnchor();
	sal_Bool GotoFtnTxt();
	sal_Bool GotoNextFtnAnchor();
	sal_Bool GotoPrevFtnAnchor();
	sal_Bool GotoNextFtnCntnt();
	sal_Bool GotoPrevFtnCntnt();

	sal_Bool MovePara( SwWhichPara, SwPosPara );
	sal_Bool MoveSection( SwWhichSection, SwPosSection );
	sal_Bool MoveTable( SwWhichTable, SwPosTable );
	sal_Bool MoveRegion( SwWhichRegion, SwPosRegion );


	// gibt es eine Selection vom Content in die Tabelle
	// Return Wert gibt an, ob der Crsr auf der alten Position verbleibt
	virtual sal_Bool IsSelOvr( int eFlags =
                                ( nsSwCursorSelOverFlags::SELOVER_CHECKNODESSECTION |
                                  nsSwCursorSelOverFlags::SELOVER_TOGGLE |
                                  nsSwCursorSelOverFlags::SELOVER_CHANGEPOS ));
	virtual sal_Bool IsInProtectTable( sal_Bool bMove = sal_False,
										sal_Bool bChgCrsr = sal_True );
	sal_Bool IsNoCntnt() const;

	void RestoreSavePos();		// Point auf die SavePos setzen

	// sal_True: an die Position kann der Cursor gesetzt werden
	virtual sal_Bool IsAtValidPos( sal_Bool bPoint = sal_True ) const;

	// darf der Cursor in ReadOnlyBereiche?
    virtual bool IsReadOnlyAvailable() const;

    virtual sal_Bool IsSkipOverProtectSections() const;
    virtual sal_Bool IsSkipOverHiddenSections() const;

    sal_uInt8 GetCrsrBidiLevel() const { return nCursorBidiLevel; }
    void SetCrsrBidiLevel( sal_uInt8 nNewLevel ) { nCursorBidiLevel = nNewLevel; }

    bool IsColumnSelection() const { return mbColumnSelection; }
    void SetColumnSelection( bool bNew ) { mbColumnSelection = bNew; }

    long GetCrsrRowSpanOffset() const { return mnRowSpanOffset; }
    void SetCrsrRowSpanOffset( long nNew ) { mnRowSpanOffset = nNew; }

    DECL_FIXEDMEMPOOL_NEWDEL( SwCursor )
};


class SwCrsrSaveState
{
	SwCursor& rCrsr;
public:
	SwCrsrSaveState( SwCursor& rC ) : rCrsr( rC ) { rC.SaveState(); }
	~SwCrsrSaveState() { rCrsr.RestoreState(); }
};

struct _SwCursor_SavePos
{
	sal_uLong nNode;
	xub_StrLen nCntnt;
	_SwCursor_SavePos* pNext;

	_SwCursor_SavePos( const SwCursor& rCrsr )
		: nNode( rCrsr.GetPoint()->nNode.GetIndex() ),
		nCntnt( rCrsr.GetPoint()->nContent.GetIndex() ),
		pNext( 0 )
	{}
	virtual ~_SwCursor_SavePos() {}

	DECL_FIXEDMEMPOOL_NEWDEL( _SwCursor_SavePos )
};



class SwTableCursor : public virtual SwCursor
{

protected:
	sal_uLong nTblPtNd, nTblMkNd;
	xub_StrLen nTblPtCnt, nTblMkCnt;
	SwSelBoxes aSelBoxes;
	sal_Bool bChg : 1;
	sal_Bool bParked : 1;		// Tabellen-Cursor wurde geparkt

    virtual bool IsSelOvrCheck(int eFlags);

public:
	SwTableCursor( const SwPosition &rPos, SwPaM* pRing = 0 );
	SwTableCursor( SwTableCursor& );
	virtual ~SwTableCursor();

    virtual sal_Bool LeftRight( sal_Bool bLeft, sal_uInt16 nCnt, sal_uInt16 nMode,
        sal_Bool bAllowVisual, sal_Bool bSkipHidden, sal_Bool bInsertCrsr );
    virtual sal_Bool GotoTable( const String& rName );

	void InsertBox( const SwTableBox& rTblBox );
	void DeleteBox( sal_uInt16 nPos ) { aSelBoxes.Remove( nPos ); bChg = sal_True; }
	sal_uInt16 GetBoxesCount() const { return aSelBoxes.Count(); }
	const SwSelBoxes& GetBoxes() const { return aSelBoxes; }

		// Baut fuer alle Boxen die Cursor auf
	SwCursor* MakeBoxSels( SwCursor* pAktCrsr );
		// sind irgendwelche Boxen mit einem Schutz versehen?
	sal_Bool HasReadOnlyBoxSel() const;

		// wurde der TabelleCursor veraendert ? Wenn ja speicher gleich
		// die neuen Werte.
	sal_Bool IsCrsrMovedUpdt();
		// wurde der TabelleCursor veraendert ?
	sal_Bool IsCrsrMoved() const
	{
		return	nTblMkNd != GetMark()->nNode.GetIndex() ||
				nTblPtNd != GetPoint()->nNode.GetIndex() ||
				nTblMkCnt != GetMark()->nContent.GetIndex() ||
				nTblPtCnt != GetPoint()->nContent.GetIndex();
	}

	sal_Bool IsChgd() const { return bChg; }

	// Parke den Tabellen-Cursor auf dem StartNode der Boxen.
	void ParkCrsr();

    bool NewTableSelection();
    void ActualizeSelection( const SwSelBoxes &rBoxes );
};

#endif

