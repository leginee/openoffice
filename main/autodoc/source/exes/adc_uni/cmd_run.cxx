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

#include <precomp.h>
#include "cmd_run.hxx"


// NOT FULLY DEFINED SERVICES
#include <cosv/file.hxx>
#include <cosv/x.hxx>
#include <ary/ary.hxx>
#include <ary/cpp/c_gate.hxx>
#include <ary/idl/i_ce.hxx>
#include <ary/idl/i_gate.hxx>
#include <ary/idl/i_module.hxx>
#include <ary/idl/ip_ce.hxx>
#include <autodoc/filecoli.hxx>
#include <autodoc/parsing.hxx>
#include <autodoc/prs_code.hxx>
#include <autodoc/prs_docu.hxx>
#include <parser/unoidl.hxx>
#include <adc_cl.hxx>
#include "adc_cmd_parse.hxx"
#include "adc_cmds.hxx"

namespace autodoc
{
namespace command
{
namespace run
{

Parser::Parser( const Parse & i_command )
    :   rCommand(i_command),
        pCppParser(),
        pCppDocuInterpreter(),
        pIdlParser()
{
}

Parser::~Parser()
{
}

bool
Parser::Perform()
{
    Cout() << "Parsing the repository "
              << rCommand.ReposyName()
              << " ..."
              << Endl();
  try
  {
    ::ary::Repository &
        rAry = CommandLine::Get_().TheRepository();
    rAry.Set_Title(rCommand.ReposyName());

    Dyn< FileCollector_Ifc >
        pFiles( ParseToolsFactory().Create_FileCollector(6000) );

    bool bIDL = false;
    bool bCpp = false;

    command::Parse::ProjectIterator
        itEnd = rCommand.ProjectsEnd();
    for ( command::Parse::ProjectIterator it = rCommand.ProjectsBegin();
          it != itEnd;
          ++it )
    {
        uintt nCount = GatherFiles( *pFiles, *(*it) );
        Cout() << nCount
             << " files found to parse in project "
             << (*it)->Name()
             << "."
             << Endl();

        switch ( (*it)->Language().eLanguage )
        {
            case command::S_LanguageInfo::idl:
            {
                Get_IdlParser().Run(*pFiles);
                bIDL = true;
            }   break;
            case command::S_LanguageInfo::cpp:
            {
                Get_CppParser().Run( *pFiles );
                bCpp = true;
            }   break;
            default:
                Cerr() << "Project in yet unimplemented language skipped."
                       << Endl();
        }
    }	// end for

    if (bCpp)
    {
	    rAry.Gate_Cpp().Calculate_AllSecondaryInformation();
    }
    if (bIDL)
    {
	    rAry.Gate_Idl().Calculate_AllSecondaryInformation(
	                        rCommand.DevelopersManual_RefFilePath() );

//        ::ary::idl::SecondariesPilot &
//            rIdl2sPilot = rAry.Gate_Idl().Secondaries();
//
//        rIdl2sPilot.CheckAllInterfaceBases( rAry.Gate_Idl() );
//        rIdl2sPilot.Connect_Types2Ces();
//        rIdl2sPilot.Gather_CrossReferences();
//
//        if (NOT rCommand.DevelopersManual_RefFilePath().empty())
//        {
//            csv::File
//                aFile(rCommand.DevelopersManual_RefFilePath(), csv::CFM_READ);
//            if ( aFile.open() )
//            {
//                rIdl2sPilot.Read_Links2DevManual(aFile);
//     	        aFile.close();
//            }
//        }
    }   // endif (bIDL)

    return true;

  }   // end try
  catch (csv::Exception & xx)
  {
    xx.GetInfo(Cerr());
    Cerr() << " program will exit." << Endl();

    return false;
  }
}

CodeParser_Ifc &
Parser::Get_CppParser()
{
    if ( NOT pCppParser )
        Create_CppParser();
    return *pCppParser;
}

IdlParser &
Parser::Get_IdlParser()
{
    if ( NOT pIdlParser )
        Create_IdlParser();
    return *pIdlParser;
}

void
Parser::Create_CppParser()
{
    pCppParser          = ParseToolsFactory().Create_Parser_Cplusplus();
    pCppDocuInterpreter = ParseToolsFactory().Create_DocuParser_AutodocStyle();

    pCppParser->Setup( CommandLine::Get_().TheRepository(),
                       *pCppDocuInterpreter,
                       CommandLine::Get_().IgnoreDefines() );
}

void
Parser::Create_IdlParser()
{
    pIdlParser = new IdlParser(CommandLine::Get_().TheRepository());
}

const ParseToolsFactory_Ifc &
Parser::ParseToolsFactory()
{
    return ParseToolsFactory_Ifc::GetIt_();
}

uintt
Parser::GatherFiles( FileCollector_Ifc &    o_rFiles,
                     const S_ProjectData &  i_rProject )
{
    uintt ret = 0;
    o_rFiles.EraseAll();

    typedef StringVector                StrVector;
    typedef StrVector::const_iterator   StrIterator;
    const S_Sources &
        rSources = i_rProject.Sources();
    const StrVector &
        rExtensions = i_rProject.Language().aExtensions;

    StrIterator     it;
    StrIterator     itTreesEnd  = rSources.aTrees.end();
    StrIterator     itDirsEnd   = rSources.aDirectories.end();
    StrIterator     itFilesEnd  = rSources.aFiles.end();
    StrIterator     itExt;
    StrIterator     itExtEnd    = rExtensions.end();

    csv::StreamStr aDir(500);
    i_rProject.RootDirectory().Get( aDir );

    uintt nProjectDir_AddPosition =
            ( strcmp(aDir.c_str(),".\\") == 0 OR strcmp(aDir.c_str(),"./") == 0 )
                ?   0
                :   uintt( aDir.tellp() );

    for ( it = rSources.aDirectories.begin();
          it != itDirsEnd;
          ++it )
    {
        aDir.seekp( nProjectDir_AddPosition );
        aDir << *it;

        for ( itExt = rExtensions.begin();
              itExt != itExtEnd;
              ++itExt )
        {
            ret += o_rFiles.AddFilesFrom( aDir.c_str(),
                                          *itExt,
                                          FileCollector_Ifc::flat );
        }   // end for itExt
    }   // end for it
    for ( it = rSources.aTrees.begin();
          it != itTreesEnd;
          ++it )
    {
        aDir.seekp( nProjectDir_AddPosition );
        aDir << *it;

        for ( itExt = rExtensions.begin();
              itExt != itExtEnd;
              ++itExt )
        {
            ret += o_rFiles.AddFilesFrom( aDir.c_str(),
                                          *itExt,
                                          FileCollector_Ifc::recursive );
        }   // end for itExt
    }   // end for it
    for ( it = rSources.aFiles.begin();
          it != itFilesEnd;
          ++it )
    {
        aDir.seekp( nProjectDir_AddPosition );
        aDir << *it;

        o_rFiles.AddFile( aDir.c_str() );
    }   // end for it
    ret += rSources.aFiles.size();

    return ret;
}


}   // namespace run
}   // namespace command


#if 0
inline const ParseToolsFactory_Ifc &
CommandRunner::ParseToolsFactory()
    { return ParseToolsFactory_Ifc::GetIt_(); }


inline const command::S_LanguageInfo &
CommandRunner::Get_ProjectLanguage( const command::Parse &          i_rCommand,
                                    const command::S_ProjectData &  i_rProject )
{
    if ( i_rProject.pLanguage )
        return *i_rProject.pLanguage;
    return *i_rCommand.GlobalLanguageInfo();
}

inline bool
CommandRunner::HasParsedCpp() const
    { return pCppParser; }
inline bool
CommandRunner::HasParsedIdl() const
    { return pIdlParser; }





CommandRunner::CommandRunner()
    :   pCommandLine(0),
        pReposy(0),
        pNewReposy(0),
        nResultCode(0)
{
    Cout() << "\nAutodoc version 2.2.1"
           << "\n-------------------"
           << "\n" << Endl();
}

CommandRunner::~CommandRunner()
{
    ary::Repository::Destroy_();
    Cout() << "\n" << Endl();
}

void
CommandRunner::Run( const CommandLine & i_rCL )
{
    ary::Repository::Destroy_();
//  ary::Repository::Destroy_();
    pReposy = 0;
    pNewReposy = 0;
    nResultCode = 0;
    pCommandLine = &i_rCL;

    pCommandLine->Run();
}

void
CommandRunner::Parse()
{
    try
    {

    csv_assert( pCommandLine->Cmd_Parse() != 0 );
    const command::Parse &
        rCmd = *pCommandLine->Cmd_Parse();

    Cout() << "Parsing the repository "
              << rCmd.ReposyName()
              << " ..."
              << Endl();

    if ( pReposy == 0 )
        pReposy = & ary::Repository::Create_( rCmd.ReposyName(), 0 );
    if ( pNewReposy == 0 )
        pNewReposy = & ary::Repository::Create_( rCmd.ReposyName() );

    Dyn< FileCollector_Ifc > pFiles;
    pFiles      = ParseToolsFactory().Create_FileCollector(6000);

    bool bCpp = false;
    bool bIDL = false;

    command::Parse::ProjectIterator itEnd = rCmd.ProjectsEnd();
    for ( command::Parse::ProjectIterator it = rCmd.ProjectsBegin();
          it != itEnd;
          ++it )
    {

        uintt nCount = GatherFiles( *pFiles, rCmd, *(*it) );
        Cout() << nCount
             << " files found to parse in project "
             << (*it)->Name()
             << "."
             << Endl();


        switch ( Get_ProjectLanguage(rCmd, *(*it)).eLanguage )
        {
            case command::S_LanguageInfo::cpp:
            {
                Get_CppParser().Run( (*it)->Name(),
                                     (*it)->RootDirectory(),
                                     *pFiles );
                bCpp = true;
            }   break;
            case command::S_LanguageInfo::idl:
            {
                Get_IdlParser().Run(*pFiles);
                bIDL = true;
            }   break;
            default:
                Cerr() << "Project in yet unimplemented language skipped."
                       << Endl();
        }
    }	// end for

    if (bCpp)
	    pReposy->RwGate_Cpp().Connect_AllTypes_2_TheirRelated_CodeEntites();
    if (bIDL)
    {
        pNewReposy->Gate_Idl().Secondaries().Connect_Types2Ces();
        pNewReposy->Gate_Idl().Secondaries().Gather_CrossReferences();
    }

    }   // end try
    catch (csv::Exception & xx)
    {
        xx.GetInfo(Cerr());
        Cerr() << " program will exit." << Endl();
        nResultCode = 1;
    }
    catch (...)
    {
        Cerr() << "Unknown exception -   program will exit." << Endl();
        nResultCode = 1;
    }
}

void
CommandRunner::Load()
{
    Cout() << "This would load the repository from the directory "
              << pCommandLine->Cmd_Load()->ReposyDir()
              << "."
              << Endl();
}


void
CommandRunner::Save()
{
    Cout() << "This would save the repository into the directory "
              << pCommandLine->Cmd_Save()->ReposyDir()
              << "."
              << Endl();
}


void
CommandRunner::CreateHtml()
{
    Cout() << "Creating HTML-output into the directory "
              << pCommandLine->Cmd_CreateHtml()->OutputDir()
              << "."
              << Endl();

    if ( HasParsedCpp() )
        CreateHtml_NewStyle();
    if ( HasParsedIdl() )
        CreateHtml_OldIdlStyle();
}



void
CommandRunner::CreateXml()
{
    Cout() << "This would create the XML-output into the directory "
              << pCommandLine->Cmd_CreateXml()->OutputDir()
              << "."
              << Endl();
}

CodeParser_Ifc &
CommandRunner::Get_CppParser()
{
    if ( NOT pCppParser )
        Create_CppParser();
    return *pCppParser;
}

IdlParser &
CommandRunner::Get_IdlParser()
{
    if ( NOT pIdlParser )
        Create_IdlParser();
    return *pIdlParser;
}

void
CommandRunner::Create_CppParser()
{
    pCppParser          = ParseToolsFactory().Create_Parser_Cplusplus();
    pCppDocuInterpreter = ParseToolsFactory().Create_DocuParser_AutodocStyle();

    pCppParser->Setup( *pReposy,
                       *pCppDocuInterpreter );
}

void
CommandRunner::Create_IdlParser()
{
    pIdlParser = new IdlParser(*pNewReposy);
}

uintt
CommandRunner::GatherFiles( FileCollector_Ifc &            o_rFiles,
                            const command::Parse &         i_rCommand,
                            const command::S_ProjectData & i_rProject )
{
    uintt ret = 0;
    o_rFiles.EraseAll();

    typedef StringVector                StrVector;
    typedef StrVector::const_iterator   StrIterator;
    const command::S_Sources &
        rSources = i_rProject.aFiles;
    const StrVector &
        rExtensions = Get_ProjectLanguage(i_rCommand,i_rProject).aExtensions;

    StrIterator     it;
    StrIterator     itDirsEnd   = rSources.aDirectories.end();
    StrIterator     itTreesEnd  = i_rProject.aFiles.aTrees.end();
    StrIterator     itFilesEnd  = i_rProject.aFiles.aFiles.end();
    StrIterator     itExt;
    StrIterator     itExtEnd    = rExtensions.end();

    csv::StreamStr aDir(500);
    i_rProject.aRootDirectory.Get( aDir );

    uintt nProjectDir_AddPosition =
            ( strcmp(aDir.c_str(),".\\") == 0 OR strcmp(aDir.c_str(),"./") == 0 )
                ?   0
                :   uintt( aDir.tellp() );

    for ( it = rSources.aDirectories.begin();
          it != itDirsEnd;
          ++it )
    {
        aDir.seekp( nProjectDir_AddPosition );
        aDir << *it;

        for ( itExt = rExtensions.begin();
              itExt != itExtEnd;
              ++itExt )
        {
            ret += o_rFiles.AddFilesFrom( aDir.c_str(),
                                          *itExt,
                                          FileCollector_Ifc::flat );
        }   // end for itExt
    }   // end for it
    for ( it = rSources.aTrees.begin();
          it != itTreesEnd;
          ++it )
    {
        aDir.seekp( nProjectDir_AddPosition );
        aDir << *it;

        for ( itExt = rExtensions.begin();
              itExt != itExtEnd;
              ++itExt )
        {
            ret += o_rFiles.AddFilesFrom( aDir.c_str(),
                                          *itExt,
                                          FileCollector_Ifc::recursive );
        }   // end for itExt
    }   // end for it
    for ( it = rSources.aFiles.begin();
          it != itFilesEnd;
          ++it )
    {
        aDir.seekp( nProjectDir_AddPosition );
        aDir << *it;

        o_rFiles.AddFile( aDir.c_str() );
    }   // end for it
    ret += rSources.aFiles.size();

    return ret;
}

void
CommandRunner::CreateHtml_NewStyle()
{
    const ary::cpp::DisplayGate &
            rGate = pReposy->DisplayGate_Cpp();

    Dyn< autodoc::HtmlDisplay_UdkStd > pHtmlDisplay;
            pHtmlDisplay = DisplayToolsFactory_Ifc::GetIt_()
                                .Create_HtmlDisplay_UdkStd();

    pHtmlDisplay->Run( pCommandLine->Cmd_CreateHtml()->OutputDir(),
                       rGate,
                       DisplayToolsFactory_Ifc::GetIt_().Create_StdFrame() );
}

void
CommandRunner::CreateHtml_OldIdlStyle()
{
    ary::idl::Gate &
        rAryGate            = pNewReposy->Gate_Idl();

    // Read DevManualLinkFile:
    // KORR_FUTURE
    csv::File
        aFile("devmanref.txt", csv::CFM_READ);
    if ( aFile.open() )
    {
        rAryGate.Secondaries().Read_Links2DevManual(aFile);
     	aFile.close();
    }

    // New Style Output
    Dyn<autodoc::HtmlDisplay_Idl_Ifc> pNewDisplay;
        pNewDisplay         = DisplayToolsFactory_Ifc::GetIt_()
                                .Create_HtmlDisplay_Idl();
    pNewDisplay->Run( pCommandLine->Cmd_CreateHtml()->OutputDir(),
                      rAryGate,
                      DisplayToolsFactory_Ifc::GetIt_().Create_StdFrame() );
}
#endif // 0

}   // namespace autodoc




