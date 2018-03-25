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
#include <adc_cl.hxx>


// NOT FULLY DEFINED SERVICES
#include <algorithm>
#include <cosv/x.hxx>
#include <cosv/file.hxx>
#include <cosv/tpl/tpltools.hxx>
#include <ary/ary.hxx>
#include <tools/tkpchars.hxx>
#include <adc_msg.hxx>
#include "adc_cmds.hxx"
#include "adc_cmd_parse.hxx"
#include "cmd_sincedata.hxx"


namespace autodoc
{

CommandLine * CommandLine::pTheInstance_ = 0;

const char * const C_sUserGuide =
"\n\n\n"
"               General Use of Autodoc\n"
"               ----------------------\n"
"\n"
"   Example for C++:\n"
"\n"
"   -html <OutputDirectory> -name \"UDK 3.x anything\" -lg c++\n"
"        -p <ProjName> <ProjectRootDirectory>\n"
"            -t <SourceDir_relativeToProjectRoot>\n"
"\n"
"   There may be several projects specified by -p.\n"
"\n"
"\n"
"   Example for IDL:\n"
"\n"
"   -html <OutputDirectory> -name \"UDK 3.x anything\" -lg idl\n"
"         -t <SourceDir1> <SourceDir2>\n"
"\n"
"   For both languages, instead of or in addition to -t may be\n"
"   used -d (no subdirectories) or -f (just one file). There can\n"
"   be multiple arguments after each of these options (-t -d -f).\n"
"\n"
"\n"
"           Replacing @since Tag Content\n"
"           ----------------------------\n"
"\n"
"   In both languages you can give a transformation file to replace\n"
"   entries in @since tags by different entries.\n"
"   This file is given by the option\n"
"       -sincefile <TransformationFilePath>\n"
"   This option has to appear between the -html and the -lg option.\n"
"   Example:\n"
"   -html <OutputDirectory> -sincefile replacesince.txt\n"
"       -name \"UDK 3.x anything\" -lg idl -t <SourceDir>\n"
"\n"
"\n";


#if 0   // FUTURE
"\n\n\n"
"               Use of Autodoc\n"
"               --------------\n"
"\n"
"   Basics:\n"
"\n"
"   Autodoc may perform different tasks.\n"
"\n"
"   Possible tasks are\n"
"       - parsing source code\n"
"       - creating HTML-output.\n"
"   On the command line each task starts with a specific\n"
"   option:\n"
"       '-parse' for parsing source code,\n"
"       '-html' for creating HTML.\n"
"   All command line options, related to one task, have to follow before\n"
"   the starting option of the next task.\n"
"\n"
"   Within the task '-parse', there may be defined different projects.\n"
"   A project definition is started with '-p'.\n"
"   All not project specific options within the task '-parse' have to\n"
"   appear in front of the first '-p'.\n"
"   There can be no project at all. Then all options, available for\n"
"   projects, can be used like for one nameless default project, without using\n"
"   '-p', but these options still have to appear behind all other\n"
"   options of the task '-parse'.\n"
"\n"
"\n"
"   Legend:\n"
"\n"
"       <SomeText>      Describes an argument.\n"
"       'SomeText'      Within '...' is the literal value of an argument.\n"
"       +               There can be multiple arguments.\n"
"       |               Separator for alternative literal values of an argument.\n"
"\n"
"\n"
"   Syntax:\n"
"\n"
"   -parse\n"
"       -name <RepositoryName>]\n"
"       -lg 'c++'|'idl'\n"
"       -extg <AdditonalExtensions>+\n"
"       -docg 'usehtml'\n"
"       -p <ProjectName> <ProjectRootDir>\n"
"           -l 'c++'|'idl'\n"
"           -ext <AdditonalExtensions>+\n"
"           -doc 'usehtml'\n"
"           -d <SourceDir_relative2ProjectRootDir_nosubdirs>+\n"
"           -t <SourceTree_relative2ProjectRootDir>+\n"
"           -f <SourceFile_relative2ProjectRootDir>+\n"
"   -html <OutputDir>\n"
"       -xlinks <Namespace> <ExternLinksRootDir>\n"
"   -i <CommandFilePath>\n"
"   -v <VerboseNr>\n"
"\n"
"\n"
"   Detailed Options Description:\n"
"\n"
"   Option      Arguments\n"
"   ----------------------------------------------------------\n"
"\n"
"   -parse      \n\n"
"               Starts the task \"Parse source code\".\n"
"               May be omitted, if it would be the first option on the\n"
"               command line.\n"
"\n"
"   -name       <RepositoryName>\n\n"
"               This name is used for naming the repository in\n"
"               human readable output. In future it may be used also for\n"
"               identifiing a repository in searches.\n"
"\n"
"   -lg         'c++|'idl'\n\n"
"               Identifies the programming language to be parsed.\n"
"                   'c++':  C++\n"
"                           Files with extensions '.h', '.hxx' are parsed.\n"
"                   'idl':  UNO-IDL\n"
"                           Files with extensions '.idl' are parsed.\n"
"               Here the language is set globally for all projects.\n"
"               A project can override this by using '-l'.\n"
"\n"
"   -extg       <.AdditionalExtension>+\n\n"
"               Has to follow immediately behind '-lg'.\n"
"               Specifies additional extensions, that will be recognised as\n"
"               source code files of the previously specified programming\n"
"               language.  Each extension has to start with '.'.\n"
"               It is possible to include extensionless files, too,\n"
"               by the argument '.'\n"
"               Here these extensions are set globally for all projects.\n"
"               A project can override this by using '-l' and '-ext'.\n"
"\n"
"   -docg       'html'|'nohtml'\n\n"
"               Specifies the default for all comments in source code, so \n"
"               that HTML-tags are interpreted as such or else treated as\n"
"               regular text.\n"
"               Without this option, the default is 'nohtml'.\n"
"               Here the default is set globally for all projects.\n"
"               A project can override this by using '-doc'.\n"
"\n"
"   -p          <ProjectName> <ProjectRootDirectory>\n\n"
"               ProjectName is used in output as human readable identifier\n"
"               for the project. ProjectRootDirectory is the path,\n"
"               where the arguments of '-d', '-t' and '-f' are relative to.\n"
"               This option can be omitted, then there is no project name\n"
"               and all paths are relative to the current working directory.\n"
"\n"
"   -l          'c++|'idl'\n\n"
"               Overrides -lg and -extg for the current project, which is\n"
"               specified by the last previous '-p'.\n"
"               For details see at option '-lg'.\n"
"\n"
"   -ext        <.AdditionalExtension>+\n\n"
"               Can be used only immediately behind '-l'.\n"
"               Overrides -extg for the current project, which is\n"
"               specified by the last previous '-p'.\n"
"               For details see at option '-extg'.\n"
"\n"
"   -doc        'html'|'nohtml'\n\n"
"               Overrides -docg for the current project, which is\n"
"               specified by the last previous '-p'.\n"
"               For details see at option '-docg'.\n"
"\n"
"   -d          <SourceDir_relative2ProjectRootDir_nosubdirs>+\n\n"
"               For the current project all files in the given\n"
"               directories are parsed, which have valid extensions.\n"
"               Subdirectories are NOT parsed.\n"
"\n"
"   -t          <SourceTree_relative2ProjectRootDir>+\n\n"
"               For the current project all files in the given\n"
"               directories AND its subdirectories are parsed, which\n"
"               have valid extensions.\n"
"\n"
"   -f          <SourceFile_relative2ProjectRootDir>+\n\n"
"               For the current project and language the given files\n"
"               are parsed. It doesn't matter, if their extensions match\n"
"               the valid extensions.\n"
"\n"
"   -html       <OutputRootDir>\n\n"
"               Starts the task \"Create HTML output\".\n"
"\n"
"   -xlinks     <Namespace> <ExternLinksRootDir>\n\n"
"               This option allows, to create links to external\n"
"               HTML-documents.\n"
"               For all source code objects (like classes or functions)\n"
"               which belong in the given namespace, the given root\n"
"               directory is used as a base for links to them.\n"
"               Presently, this works only for C++-mappings of IDL-items.\n"
"               The given namespace has to be absolute.\n"
"\n"
"   -i          <CommandFilePath>\n\n"
"               This option is replaced by the contents of the given\n"
"               file. The file has to be ASCII and each option\n"
"               has to start in the first column of a new line.\n"
"               So each valid line starts with a '-'.\n"
"               Empty lines are allowed.\n"
"               Comment lines have to start with '#'\n"
"\n"
"   -v          <VerboseNumber>\n\n"
"               Show details during parsing:\n"
"                   2    shows each parsed letter,\n"
"                   4    shows stored objects.\n"
"                   1    shows recognised tokens.\n"
"               These bit-values can be combined.\n"
"               This option suppresses errors, because of\n"
"               missing output options (no '-html').\n";
#endif // 0, FUTURE


CommandLine::CommandLine()
    :   nDebugStyle(0),
        pSinceTransformator(new command::SinceTagTransformationData),
        aCommands(),
        bInitOk(false),
        pCommand_CreateHtml(0),
        pReposy( & ary::Repository::Create_() ),
        bCpp(false),
        bIdl(false)
{
    csv_assert(pTheInstance_ == 0);
    pTheInstance_ = this;
}

CommandLine::~CommandLine()
{
    csv::erase_container_of_heap_ptrs(aCommands);
    pTheInstance_ = 0;
}

int
CommandLine::Run() const
{
    Cout() << "\nAutodoc version 2.2.5"
           << "\n---------------------"
           << "\n" << Endl();

    bool
        ok = true;
    for ( CommandList::const_iterator it = aCommands.begin();
          ok AND it != aCommands.end();
          ++it )
    {
        ok = (*it)->Run();
    }

    if (pCommand_CreateHtml != 0)
    {
        StreamStr aDiagnosticMessagesFile(700);
        aDiagnosticMessagesFile
            << pCommand_CreateHtml->OutputDir()
            << csv::ploc::Delimiter()
            << "Autodoc_DiagnosticMessages.txt";
        TheMessages().WriteFile(aDiagnosticMessagesFile.c_str());
    }

    return ok ? 0 : 1;
}

CommandLine &
CommandLine::Get_()
{
    csv_assert(pTheInstance_ != 0);
    return *pTheInstance_;
}

bool
CommandLine::DoesTransform_SinceTag() const
{
    return pSinceTransformator->DoesTransform();
}

//bool
//CommandLine::Strip_SinceTagText( String & io_sSinceTagValue ) const
//{
//    return pSinceTransformator->StripSinceTagText(io_sSinceTagValue);
//}

const String &
CommandLine::DisplayOf_SinceTagValue( const String & i_sVersionNumber ) const
{
    return pSinceTransformator->DisplayOf(i_sVersionNumber);
}

void
CommandLine::do_Init( int                 argc,
                      char *              argv[] )
{
  try
  {
    bInitOk = false;
    StringVector    aParameters;

    char * * itpEnd = &argv[0] + argc;
    for ( char * * itp = &argv[1]; itp != itpEnd; ++itp )
    {
     	if ( strncmp(*itp, "-I:", 3) != 0 )
            aParameters.push_back(String(*itp));
        else
            load_IncludedCommands(aParameters, (*itp)+3);
    }

    StringVector::const_iterator itEnd = aParameters.end();
    for ( StringVector::const_iterator it = aParameters.begin();
          it != itEnd;
        )
    {
        if ( *it == command::C_opt_Verbose )
            do_clVerbose(it,itEnd);
        else if ( *it == command::C_opt_LangAll
                  OR *it == command::C_opt_Name
                  OR *it == command::C_opt_DevmanFile )
            do_clParse(it,itEnd);
        else if (*it == command::C_opt_CreateHtml)
            do_clCreateHtml(it,itEnd);
        else if (*it == command::C_opt_SinceFile)
            do_clSinceFile(it,itEnd);
        else if (*it == command::C_opt_IgnoreDefine)
        {
            AddIgnoreDefine(*(++it));
            ++it;
        }
        else if (*it == command::C_opt_ExternNamespace)
        {
            sExternNamespace = *(++it);
            ++it;
            if ( strncmp(sExternNamespace.c_str(), "::", 2) != 0)
            {
             	throw command::X_CommandLine(
                        "-extnsp needs an absolute qualified namespace, starting with \"::\"."
                        );
            }
        }
        else if (*it == command::C_opt_ExternRoot)
        {
            ++it;
            StreamLock sl(1000);
            if ( csv::compare(*it, 0, "http://", 7) != 0 )
            {
                sl() << "http://" << *it;
            }
            if ( *(sl().end()-1) != '/')
                sl() << '/';
            sExternRoot = sl().c_str();

            ++it;
        }
//        else if (*it == command::C_opt_CreateXml)
//            do_clCreateXml(it,itEnd);
//        else if (command::C_opt_Load)
//            do_clLoad(it,itEnd);
//        else if (*it == command::C_opt_Save)
//            do_clSave(it,itEnd);
        else if (*it == "-h" OR *it == "-?" OR *it == "?")
            // Leads to displaying help, because bInitOk stays on false.
         	return;
        else if ( *it == command::C_opt_Parse )
            // Only for backwards compatibility.
            //   Just ignore "-parse".
            ++it;
        else
        {
            StreamLock sl(200);
         	throw command::X_CommandLine(
                            sl() << "Unknown commandline option \""
                                 << *it
                                 << "\"."
                                 << c_str );
        }
    }   // end for
    sort_Commands();

    bInitOk = true;

  }   // end try
  catch ( command::X_CommandLine & xxx )
  {
    xxx.Report( Cerr() );
  }
  catch ( csv::Exception & xxx )
  {
    xxx.GetInfo( Cerr() );
  }
}

void
CommandLine::do_PrintUse() const
{
    Cout() << C_sUserGuide << Endl();
}

bool
CommandLine::inq_CheckParameters() const
{
    if (NOT bInitOk OR aCommands.size() == 0)
        return false;
    return true;
}

void
CommandLine::load_IncludedCommands( StringVector &      out,
                                    const char *        i_filePath )
{
    CharacterSource
        aIncludedCommands;
    csv::File
        aFile(i_filePath, csv::CFM_READ);
    if (NOT aFile.open())
    {
     	Cerr() << "Command include file \""
               << i_filePath
               << "\" not found."
               << Endl();
        throw command::X_CommandLine("Invalid file in option -I:<command-file>.");
    }
    aIncludedCommands.LoadText(aFile);
    aFile.close();

    bool bInToken = false;
    StreamLock aTransmit(200);
    for ( ; NOT aIncludedCommands.IsFinished(); aIncludedCommands.MoveOn() )
    {
        if (bInToken)
        {
            if (aIncludedCommands.CurChar() <= 32)
            {
                const char *
                    pToken = aIncludedCommands.CutToken();
                bInToken = false;

             	if ( strncmp(pToken, "-I:", 3) != 0 )
             	{
             	    aTransmit().seekp(0);
             	    aTransmit() << pToken;
             	    aTransmit().replace_all('\\', *csv::ploc::Delimiter());
             	    aTransmit().replace_all('/', *csv::ploc::Delimiter());
                    out.push_back(String(aTransmit().c_str()));
             	}
                else
                    load_IncludedCommands(out, pToken+3);
            }
        }
        else
        {
            if (aIncludedCommands.CurChar() > 32)
            {
                aIncludedCommands.CutToken();
                bInToken = true;
            }
        }   // endif (bInToken) else

    }   // end while()
}

namespace
{
inline int
v_nr(StringVector::const_iterator it)
{
 	return int( *(*it).c_str() ) - int('0');
}
}   // anonymous namespace

void
CommandLine::do_clVerbose(  opt_iter &          it,
                            opt_iter            itEnd )
{
    ++it;
    if ( it == itEnd ? true : v_nr(it) < 0 OR v_nr(it) > 7 )
        throw command::X_CommandLine( "Missing or invalid number in -v option." );
    nDebugStyle = v_nr(it);
    ++it;
}

void
CommandLine::do_clParse( opt_iter &          it,
                         opt_iter            itEnd )
{
    command::Command *
        pCmd_Parse = new command::Parse;
    pCmd_Parse->Init(it, itEnd);
    aCommands.push_back(pCmd_Parse);
}

void
CommandLine::do_clCreateHtml( opt_iter &          it,
                              opt_iter            itEnd )
{
    pCommand_CreateHtml = new command::CreateHtml;
    pCommand_CreateHtml->Init(it, itEnd);
    aCommands.push_back(pCommand_CreateHtml);
}

void
CommandLine::do_clSinceFile( opt_iter &          it,
                             opt_iter            itEnd )
{
    pSinceTransformator->Init(it, itEnd);
}


namespace
{

struct Less_RunningRank
{
    bool                operator()(
                            const command::Command * const &
                                                i1,
                            const command::Command * const &
                                                i2 ) const
                        { return i1->RunningRank() < i2->RunningRank(); }
};

}   // anonymous namespace



void
CommandLine::sort_Commands()
{
    std::sort( aCommands.begin(),
               aCommands.end(),
               Less_RunningRank() );
}

}   // namespace autodoc
