#**************************************************************
#  
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#  
#**************************************************************



PRJ = ..
PRJNAME = postprocess
TARGET = packregistry

MY_XCS = registry/schema/org/openoffice
MY_XCU = registry/data/org/openoffice
MY_MOD = registry/spool

MY_XCDS = \
    $(MISC)/base.xcd \
    $(MISC)/brand.xcd \
    $(MISC)/calc.xcd \
    $(MISC)/cjk.xcd \
    $(MISC)/ctl.xcd \
    $(MISC)/draw.xcd \
    $(MISC)/graphicfilter.xcd \
    $(MISC)/impress.xcd \
    $(MISC)/korea.xcd \
    $(MISC)/lingucomponent.xcd \
    $(MISC)/main.xcd \
    $(MISC)/math.xcd \
    $(MISC)/oooimprovement.xcd \
    $(MISC)/palm.xcd \
    $(MISC)/pocketexcel.xcd \
    $(MISC)/pocketword.xcd \
    $(MISC)/pyuno.xcd \
    $(MISC)/w4w.xcd \
    $(MISC)/writer.xcd \
    $(MISC)/xsltfilter.xcd
.IF "$(ENABLE_ONLINE_UPDATE)" == "YES"
MY_XCDS += \
    $(MISC)/onlineupdate.xcd
.END

MY_DEPS_base = main
MY_FILES_base = \
    $(MY_MOD)/fcfg_database_filters.xcu \
    $(MY_MOD)/fcfg_database_others.xcu \
    $(MY_MOD)/fcfg_database_types.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-base.xcu \
    $(MY_MOD)/org/openoffice/Setup-base.xcu

#MY_FILES_brand = \
#    $(MY_XCU)/Office/Compatibility.xcu \
#    $(MY_MOD)/org/openoffice/Office/Common-brand.xcu \
#    $(MY_MOD)/org/openoffice/Office/UI-brand.xcu \
#    $(MY_MOD)/org/openoffice/Setup-brand.xcu
#.IF "$(ENABLE_SVCTAGS)" == "YES"
#MY_FILES_brand += \
#    $(MY_MOD)/org/openoffice/Office/Jobs/Jobs-registration.xcu
#.END

MY_DEPS_calc = main
MY_FILES_calc = \
    $(MY_XCS)/Office/UI/CalcCommands.xcs \
    $(MY_XCS)/Office/UI/CalcWindowState.xcs \
    $(MY_XCU)/Office/UI/CalcCommands.xcu \
    $(MY_XCU)/Office/UI/CalcWindowState.xcu \
    $(MY_MOD)/DataAccess/calc.xcu \
    $(MY_MOD)/fcfg_calc_filters.xcu \
    $(MY_MOD)/fcfg_calc_types.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-calc.xcu \
    $(MY_MOD)/org/openoffice/Office/Embedding-calc.xcu \
    $(MY_MOD)/org/openoffice/Setup-calc.xcu

MY_FILES_cjk = \
    $(MY_MOD)/org/openoffice/Office/Common-cjk.xcu \
    $(MY_MOD)/org/openoffice/Office/Writer-cjk.xcu

MY_DEPS_ctl = main
MY_FILES_ctl = \
    $(MY_MOD)/org/openoffice/Office/Common-ctl.xcu

MY_DEPS_draw = main
MY_FILES_draw = \
    $(MY_XCS)/Office/UI/DrawWindowState.xcs \
    $(MY_XCU)/Office/UI/DrawWindowState.xcu \
    $(MY_MOD)/fcfg_draw_filters.xcu \
    $(MY_MOD)/fcfg_draw_types.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-draw.xcu \
    $(MY_MOD)/org/openoffice/Office/Embedding-draw.xcu \
    $(MY_MOD)/org/openoffice/Setup-draw.xcu

MY_DEPS_graphicfilter = main
MY_FILES_graphicfilter = \
    $(MY_MOD)/fcfg_drawgraphics_filters.xcu \
    $(MY_MOD)/fcfg_drawgraphics_types.xcu \
    $(MY_MOD)/fcfg_impressgraphics_filters.xcu \
    $(MY_MOD)/fcfg_impressgraphics_types.xcu

MY_DEPS_impress = main
MY_FILES_impress = \
    $(MY_XCS)/Office/UI/Effects.xcs \
    $(MY_XCS)/Office/UI/ImpressWindowState.xcs \
    $(MY_XCU)/Office/UI/Effects.xcu \
    $(MY_XCU)/Office/UI/ImpressWindowState.xcu \
    $(MY_MOD)/fcfg_impress_filters.xcu \
    $(MY_MOD)/fcfg_impress_types.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-impress.xcu \
    $(MY_MOD)/org/openoffice/Office/Embedding-impress.xcu \
    $(MY_MOD)/org/openoffice/Office/Jobs/Jobs-presenterscreen.xcu \
    $(MY_MOD)/org/openoffice/Office/ProtocolHandler/ProtocolHandler-presenterscreen.xcu \
    $(MY_XCS)/Office/PresenterScreen.xcs \
    $(MY_XCU)/Office/PresenterScreen.xcu \
    $(MY_MOD)/org/openoffice/Setup-impress.xcu

MY_FILES_korea = \
    $(MY_MOD)/org/openoffice/Office/Common-korea.xcu

MY_DEPS_lingucomponent = main
MY_FILES_lingucomponent = \
    $(MY_XCU)/Office/Linguistic-lingucomponent-hyphenator.xcu \
    $(MY_XCU)/Office/Linguistic-lingucomponent-spellchecker.xcu \
    $(MY_XCU)/Office/Linguistic-lingucomponent-thesaurus.xcu \

MY_FILES_main = \
    $(MY_XCS)/FirstStartWizard.xcs \
    $(MY_XCS)/Inet.xcs \
    $(MY_XCS)/Interaction.xcs \
    $(MY_XCS)/LDAP.xcs \
    $(MY_XCS)/Office/Accelerators.xcs \
    $(MY_XCS)/Office/Addons.xcs \
    $(MY_XCS)/Office/Calc.xcs \
    $(MY_XCS)/Office/CalcAddIns.xcs \
    $(MY_XCS)/Office/Canvas.xcs \
    $(MY_XCS)/Office/Chart.xcs \
    $(MY_XCS)/Office/Commands.xcs \
    $(MY_XCS)/Office/Common.xcs \
    $(MY_XCS)/Office/Compatibility.xcs \
    $(MY_XCS)/Office/DataAccess/Drivers.xcs \
    $(MY_XCS)/Office/DataAccess.xcs \
    $(MY_XCS)/Office/Draw.xcs \
    $(MY_XCS)/Office/Embedding.xcs \
    $(MY_XCS)/Office/Events.xcs \
    $(MY_XCS)/Office/ExtendedColorScheme.xcs \
    $(MY_XCS)/Office/ExtensionDependencies.xcs \
    $(MY_XCS)/Office/ExtensionManager.xcs \
    $(MY_XCS)/Office/FormWizard.xcs \
    $(MY_XCS)/Office/Histories.xcs \
    $(MY_XCS)/Office/Impress.xcs \
    $(MY_XCS)/Office/Java.xcs \
    $(MY_XCS)/Office/Jobs.xcs \
    $(MY_XCS)/Office/Labels.xcs \
    $(MY_XCS)/Office/Linguistic.xcs \
    $(MY_XCS)/Office/Logging.xcs \
    $(MY_XCS)/Office/Math.xcs \
    $(MY_XCS)/Office/OOoImprovement/Settings.xcs \
    $(MY_XCS)/Office/OptionsDialog.xcs \
    $(MY_XCS)/Office/Paths.xcs \
    $(MY_XCS)/Office/ProtocolHandler.xcs \
    $(MY_XCS)/Office/Recovery.xcs \
    $(MY_XCS)/Office/SFX.xcs \
    $(MY_XCS)/Office/Scripting.xcs \
    $(MY_XCS)/Office/Security.xcs \
    $(MY_XCS)/Office/Substitution.xcs \
    $(MY_XCS)/Office/TabBrowse.xcs \
    $(MY_XCS)/Office/TableWizard.xcs \
    $(MY_XCS)/Office/TypeDetection.xcs \
    $(MY_XCS)/Office/UI/BaseWindowState.xcs \
    $(MY_XCS)/Office/UI/BasicIDECommands.xcs \
    $(MY_XCS)/Office/UI/BasicIDEWindowState.xcs \
    $(MY_XCS)/Office/UI/BibliographyCommands.xcs \
    $(MY_XCS)/Office/UI/BibliographyWindowState.xcs \
    $(MY_XCS)/Office/UI/Category.xcs \
    $(MY_XCS)/Office/UI/ChartCommands.xcs \
    $(MY_XCS)/Office/UI/ChartWindowState.xcs \
    $(MY_XCS)/Office/UI/Commands.xcs \
    $(MY_XCS)/Office/UI/Controller.xcs \
    $(MY_XCS)/Office/UI/DbBrowserWindowState.xcs \
    $(MY_XCS)/Office/UI/DbQueryWindowState.xcs \
    $(MY_XCS)/Office/UI/DbRelationWindowState.xcs \
    $(MY_XCS)/Office/UI/DbTableDataWindowState.xcs \
    $(MY_XCS)/Office/UI/DbTableWindowState.xcs \
    $(MY_XCS)/Office/UI/DbuCommands.xcs \
    $(MY_XCS)/Office/UI/DrawImpressCommands.xcs \
    $(MY_XCS)/Office/UI/Factories.xcs \
    $(MY_XCS)/Office/UI/GenericCategories.xcs \
    $(MY_XCS)/Office/UI/GenericCommands.xcs \
    $(MY_XCS)/Office/UI/GlobalSettings.xcs \
    $(MY_XCS)/Office/UI/Sidebar.xcs \
    $(MY_XCS)/Office/UI/StartModuleCommands.xcs \
    $(MY_XCS)/Office/UI/StartModuleWindowState.xcs \
    $(MY_XCS)/Office/UI/WindowContentFactories.xcs \
    $(MY_XCS)/Office/UI/WindowState.xcs \
    $(MY_XCS)/Office/UI.xcs \
    $(MY_XCS)/Office/Views.xcs \
    $(MY_XCS)/Office/WebWizard.xcs \
    $(MY_XCS)/Office/Writer.xcs \
    $(MY_XCS)/Office/WriterWeb.xcs \
    $(MY_XCS)/Setup.xcs \
    $(MY_XCS)/System.xcs \
    $(MY_XCS)/TypeDetection/Filter.xcs \
    $(MY_XCS)/TypeDetection/GraphicFilter.xcs \
    $(MY_XCS)/TypeDetection/Misc.xcs \
    $(MY_XCS)/TypeDetection/Types.xcs \
    $(MY_XCS)/TypeDetection/UISort.xcs \
    $(MY_XCS)/UserProfile.xcs \
    $(MY_XCS)/VCL.xcs \
    $(MY_XCS)/ucb/Configuration.xcs \
    $(MY_XCS)/ucb/Hierarchy.xcs \
    $(MY_XCS)/ucb/InteractionHandler.xcs \
    $(MY_XCS)/ucb/Store.xcs \
    $(MY_XCU)/FirstStartWizard.xcu \
    $(MY_XCU)/Inet.xcu \
    $(MY_XCU)/Interaction.xcu \
    $(MY_XCU)/Office/Accelerators.xcu \
    $(MY_XCU)/Office/Calc.xcu \
    $(MY_XCU)/Office/Canvas.xcu \
    $(MY_XCU)/Office/Common.xcu \
    $(MY_XCU)/Office/Compatibility.xcu \
    $(MY_XCU)/Office/DataAccess.xcu \
    $(MY_XCU)/Office/Embedding.xcu \
    $(MY_XCU)/Office/ExtensionDependencies.xcu \
    $(MY_XCU)/Office/ExtensionManager.xcu \
    $(MY_XCU)/Office/FormWizard.xcu \
    $(MY_XCU)/Office/Histories.xcu \
    $(MY_XCU)/Office/Impress.xcu \
    $(MY_XCU)/Office/Jobs.xcu \
    $(MY_XCU)/Office/Labels.xcu \
    $(MY_XCU)/Office/Logging.xcu \
    $(MY_XCU)/Office/Math.xcu \
    $(MY_XCU)/Office/Paths.xcu \
    $(MY_XCU)/Office/ProtocolHandler.xcu \
    $(MY_XCU)/Office/SFX.xcu \
    $(MY_XCU)/Office/Scripting.xcu \
    $(MY_XCU)/Office/Security.xcu \
    $(MY_XCU)/Office/TableWizard.xcu \
    $(MY_XCU)/Office/UI/BaseWindowState.xcu \
    $(MY_XCU)/Office/UI/BasicIDECommands.xcu \
    $(MY_XCU)/Office/UI/BasicIDEWindowState.xcu \
    $(MY_XCU)/Office/UI/BibliographyCommands.xcu \
    $(MY_XCU)/Office/UI/ChartCommands.xcu \
    $(MY_XCU)/Office/UI/ChartWindowState.xcu \
    $(MY_XCU)/Office/UI/Controller.xcu \
    $(MY_XCU)/Office/UI/DbBrowserWindowState.xcu \
    $(MY_XCU)/Office/UI/DbQueryWindowState.xcu \
    $(MY_XCU)/Office/UI/DbRelationWindowState.xcu \
    $(MY_XCU)/Office/UI/DbTableDataWindowState.xcu \
    $(MY_XCU)/Office/UI/DbTableWindowState.xcu \
    $(MY_XCU)/Office/UI/DbuCommands.xcu \
    $(MY_XCU)/Office/UI/DrawImpressCommands.xcu \
    $(MY_XCU)/Office/UI/Factories.xcu \
    $(MY_XCU)/Office/UI/GenericCategories.xcu \
    $(MY_XCU)/Office/UI/GenericCommands.xcu \
    $(MY_XCU)/Office/UI/Sidebar.xcu \
    $(MY_XCU)/Office/UI/StartModuleCommands.xcu \
    $(MY_XCU)/Office/UI/StartModuleWindowState.xcu \
    $(MY_XCU)/Office/UI.xcu \
    $(MY_XCU)/Office/Views.xcu \
    $(MY_XCU)/Office/WebWizard.xcu \
    $(MY_XCU)/Office/Writer.xcu \
    $(MY_XCU)/Setup.xcu \
    $(MY_XCU)/System.xcu \
    $(MY_XCU)/TypeDetection/UISort.xcu \
    $(MY_XCU)/UserProfile.xcu \
    $(MY_XCU)/VCL.xcu \
    $(MY_XCU)/ucb/Configuration.xcu \
    $(MY_MOD)/DataAccess/dbase.xcu \
    $(MY_MOD)/DataAccess/flat.xcu \
    $(MY_MOD)/DataAccess/mysql.xcu \
    $(MY_MOD)/DataAccess/odbc.xcu \
    $(MY_MOD)/fcfg_base_filters.xcu \
    $(MY_MOD)/fcfg_base_others.xcu \
    $(MY_MOD)/fcfg_base_types.xcu \
    $(MY_MOD)/fcfg_chart_filters.xcu \
    $(MY_MOD)/fcfg_chart_others.xcu \
    $(MY_MOD)/fcfg_chart_types.xcu \
    $(MY_MOD)/fcfg_internalgraphics_filters.xcu \
    $(MY_MOD)/fcfg_internalgraphics_types.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-brand.xcu \
    $(MY_MOD)/org/openoffice/Office/Embedding-chart.xcu \
    $(MY_MOD)/org/openoffice/Office/UI-brand.xcu \
    $(MY_MOD)/org/openoffice/Setup-brand.xcu \
    $(MY_MOD)/org/openoffice/Setup-start.xcu \
    $(MY_MOD)/org/openoffice/TypeDetection/UISort-calc.xcu \
    $(MY_MOD)/org/openoffice/TypeDetection/UISort-draw.xcu \
    $(MY_MOD)/org/openoffice/TypeDetection/UISort-impress.xcu \
    $(MY_MOD)/org/openoffice/TypeDetection/UISort-math.xcu \
    $(MY_MOD)/org/openoffice/TypeDetection/UISort-writer.xcu
.IF "$(ENABLE_SVCTAGS)" == "YES"
MY_FILES_main += \
    $(MY_MOD)/org/openoffice/Office/Jobs/Jobs-registration.xcu
.END
.IF "$(GUIBASE)" == "aqua"
MY_FILES_main += \
    $(MY_MOD)/DataAccess/macab.xcu \
    $(MY_MOD)/org/openoffice/Inet-macosx.xcu \
    $(MY_MOD)/org/openoffice/Office/Accelerators-macosx.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-macosx.xcu \
    $(MY_MOD)/org/openoffice/Office/Paths-macosx.xcu
        # Inet-macosx.xcu must come after Inet.xcu
.ELIF "$(GUIBASE)" == "unx"
MY_FILES_main += \
    $(MY_MOD)/org/openoffice/Inet-unixdesktop.xcu \
    $(MY_MOD)/org/openoffice/Office/Accelerators-unxwnt.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-UseOOoFileDialogs.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-unixdesktop.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-unx.xcu \
    $(MY_MOD)/org/openoffice/Office/Paths-unixdesktop.xcu \
    $(MY_MOD)/org/openoffice/Office/Paths-unxwnt.xcu \
    $(MY_MOD)/org/openoffice/VCL-unixdesktop.xcu
        # Inet-unixdesktop.xcu must come after Inet.xcu
        # VCL-unixdesktop.xcu must come after VCL.xcu
.ELIF "$(GUIBASE)" == "WIN"
MY_FILES_main += \
    $(MY_MOD)/DataAccess/ado.xcu \
    $(MY_MOD)/org/openoffice/Inet-wnt.xcu \
    $(MY_MOD)/org/openoffice/Office/Accelerators-unxwnt.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-wnt.xcu \
    $(MY_MOD)/org/openoffice/Office/Paths-unxwnt.xcu
        # Inet-wnt.xcu must come after Inet.xcu
.ELIF "$(GUIBASE)" == "os2"
MY_FILES_main += \
    $(MY_MOD)/org/openoffice/Inet-wnt.xcu \
    $(MY_MOD)/org/openoffice/Office/Accelerators-unxwnt.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-wnt.xcu \
    $(MY_MOD)/org/openoffice/Office/Paths-unxwnt.xcu
        # Inet-wnt.xcu must come after Inet.xcu
.ELSE
ERROR : unknown-GUIBASE
.END
.IF "$(OS)" == "WNT" || "$(OS)" == "LINUX" || "$(OS)" == "OS2" || \
        ("$(OS)" == "SOLARIS" && "$(CPU)" == "S") || "$(OS)" == "NETBSD"
MY_FILES_main += $(MY_MOD)/DataAccess/adabas.xcu
.END
.IF "$(ENABLE_EVOAB2)" == "TRUE"
MY_FILES_main += $(MY_MOD)/DataAccess/evoab2.xcu
.END
.IF "$(SOLAR_JAVA)" == "TRUE"
MY_FILES_main += \
    $(MY_MOD)/DataAccess/hsqldb.xcu \
    $(MY_MOD)/DataAccess/sdbc_jdbc.xcu \
    $(MY_MOD)/DataAccess/sdbc_postgresql.xcu
.END

.IF "$(SYSTEM_LIBTEXTCAT_DATA)" != ""
MY_FILES_main += $(MY_MOD)/org/openoffice/Office/Paths-externallibtextcatdata.xcu
.ELSE
MY_FILES_main += $(MY_MOD)/org/openoffice/Office/Paths-internallibtextcatdata.xcu
.END

MY_DEPS_math = main
MY_FILES_math = \
    $(MY_XCS)/Office/UI/MathCommands.xcs \
    $(MY_XCS)/Office/UI/MathWindowState.xcs \
    $(MY_XCU)/Office/UI/MathCommands.xcu \
    $(MY_XCU)/Office/UI/MathWindowState.xcu \
    $(MY_MOD)/fcfg_math_filters.xcu \
    $(MY_MOD)/fcfg_math_types.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-math.xcu \
    $(MY_MOD)/org/openoffice/Office/Embedding-math.xcu \
    $(MY_MOD)/org/openoffice/Setup-math.xcu

MY_DEPS_onlineupdate = main
MY_FILES_onlineupdate = \
    $(MY_MOD)/org/openoffice/Office/Addons/Addons-onlineupdate.xcu \
    $(MY_MOD)/org/openoffice/Office/Jobs/Jobs-onlineupdate.xcu

MY_DEPS_oooimprovement = main
MY_FILES_oooimprovement = \
    $(MY_MOD)/org/openoffice/Office/Jobs/Jobs-oooimprovement.xcu \
    $(MY_MOD)/org/openoffice/Office/Logging/Logging-oooimprovement.xcu

MY_DEPS_palm = main
MY_FILES_palm = \
    $(MY_MOD)/fcfg_palm_filters.xcu \
    $(MY_MOD)/fcfg_palm_types.xcu

MY_DEPS_pocketexcel = main
MY_FILES_pocketexcel = \
    $(MY_MOD)/fcfg_pocketexcel_filters.xcu \
    $(MY_MOD)/fcfg_pocketexcel_types.xcu

MY_DEPS_pocketword = main
MY_FILES_pocketword = \
    $(MY_MOD)/fcfg_pocketword_filters.xcu \
    $(MY_MOD)/fcfg_pocketword_types.xcu

MY_DEPS_pyuno = main
MY_FILES_pyuno = \
    $(MY_MOD)/org/openoffice/Office/Scripting-python.xcu

MY_DEPS_w4w = main
MY_FILES_w4w = \
    $(MY_MOD)/fcfg_w4w_filters.xcu \
    $(MY_MOD)/fcfg_w4w_types.xcu

MY_DEPS_writer = main
MY_FILES_writer = \
    $(MY_XCS)/Office/UI/WriterCommands.xcs \
    $(MY_XCS)/Office/UI/WriterFormWindowState.xcs \
    $(MY_XCS)/Office/UI/WriterGlobalWindowState.xcs \
    $(MY_XCS)/Office/UI/WriterReportWindowState.xcs \
    $(MY_XCS)/Office/UI/WriterWebWindowState.xcs \
    $(MY_XCS)/Office/UI/WriterWindowState.xcs \
    $(MY_XCS)/Office/UI/XFormsWindowState.xcs \
    $(MY_XCU)/Office/UI/WriterCommands.xcu \
    $(MY_XCU)/Office/UI/WriterFormWindowState.xcu \
    $(MY_XCU)/Office/UI/WriterGlobalWindowState.xcu \
    $(MY_XCU)/Office/UI/WriterReportWindowState.xcu \
    $(MY_XCU)/Office/UI/WriterWebWindowState.xcu \
    $(MY_XCU)/Office/UI/WriterWindowState.xcu \
    $(MY_XCU)/Office/UI/XFormsWindowState.xcu \
    $(MY_MOD)/fcfg_global_filters.xcu \
    $(MY_MOD)/fcfg_global_types.xcu \
    $(MY_MOD)/fcfg_web_filters.xcu \
    $(MY_MOD)/fcfg_web_types.xcu \
    $(MY_MOD)/fcfg_writer_filters.xcu \
    $(MY_MOD)/fcfg_writer_types.xcu \
    $(MY_MOD)/org/openoffice/Office/Common-writer.xcu \
    $(MY_MOD)/org/openoffice/Office/Embedding-writer.xcu \
    $(MY_MOD)/org/openoffice/Setup-writer.xcu

MY_DEPS_xsltfilter = main
MY_FILES_xsltfilter = \
    $(MY_MOD)/fcfg_xslt_filters.xcu \
    $(MY_MOD)/fcfg_xslt_types.xcu

.IF "$(GUIBASE)" == "unx" && \
        (("$(ENABLE_GCONF)" == "TRUE" && "$(ENABLE_LOCKDOWN)" == "YES") || \
         "$(ENABLE_GIO)" == "TRUE")
MY_XCDS += $(MISC)/gnome.xcd
MY_DEPS_gnome = main
.IF "$(ENABLE_GCONF)" == "TRUE" && "$(ENABLE_LOCKDOWN)" == "YES"
MY_FILES_gnome += \
    $(MY_MOD)/org/openoffice/Office/Common-gconflockdown.xcu \
    $(MY_MOD)/org/openoffice/Office/Recovery-gconflockdown.xcu \
    $(MY_MOD)/org/openoffice/UserProfile-gconflockdown.xcu \
    $(MY_MOD)/org/openoffice/VCL-gconflockdown.xcu
.END
.IF "$(ENABLE_GIO)" == "TRUE"
MY_FILES_gnome += \
    $(MY_MOD)/org/openoffice/ucb/Configuration-gio.xcu
.END
.END

.IF "$(ENABLE_OPENGL)" == "TRUE"
MY_XCDS += $(MISC)/ogltrans.xcd
MY_DEPS_ogltrans = main
MY_FILES_ogltrans = \
    $(MY_MOD)/org/openoffice/Office/Impress-ogltrans.xcu
.END

.IF "$(GUIBASE)" == "WIN" || "$(GUIBASE)" == "os2"
MY_XCDS += $(MISC)/forcedefault.xcd
MY_DEPS_forcedefault = main
MY_FILES_forcedefault = \
    $(MY_MOD)/org/openoffice/Office/Linguistic-ForceDefaultLanguage.xcu
.END

.INCLUDE : settings.mk
.INCLUDE : target.mk

ALLTAR : \
    $(MY_XCDS) \
    $(MISC)/lang/Langpack-{$(alllangiso)}.xcd \
    $(MISC)/lang/fcfg_langpack_{$(alllangiso)}.xcd \
    $(MISC)/lang/registry_{$(alllangiso)}.xcd

{$(MY_XCDS)} : $$(MY_FILES_$$(@:b):^"$(SOLARXMLDIR)/")

$(MISC)/%.xcd .ERRREMOVE : $(MISC)/%.list
    $(XSLTPROC) --nonet --stringparam prefix $(SOLARXMLDIR)/ -o $@ \
        $(SOLARENV)/bin/packregistry.xslt $<

$(MISC)/%.list : makefile.mk
    - $(RM) $@
    echo '<list>' $(foreach,i,$(MY_DEPS_$(@:b)) '<dependency file="$i"/>') \
        $(foreach,i,$(MY_FILES_$(@:b)) '<filename>$i</filename>') '</list>' > $@

$(MISC)/lang/Langpack-{$(alllangiso)}.xcd : $(SOLARXMLDIR)/$(MY_MOD)/$$(@:b).xcu

$(MISC)/lang/Langpack-%.xcd .ERRREMOVE :
    $(MKDIRHIER) $(@:d)
    - $(RM) $(MISC)/$(@:b).list
    echo '<list><dependency file="main"/>' \
        '<filename>$(MY_MOD)/$(@:b).xcu</filename></list>' > $(MISC)/$(@:b).list
    $(XSLTPROC) --nonet --stringparam prefix $(SOLARXMLDIR)/ -o $@ \
        $(SOLARENV)/bin/packregistry.xslt $(MISC)/$(@:b).list

$(MISC)/lang/fcfg_langpack_{$(alllangiso)}.xcd : $(SOLARPCKDIR)/$$(@:b).zip

# It can happen that localized $(SOLARPCKDIR)/fcfg_langpack_*.zip contain
# zero-sized org/openoffice/TypeDectection/Filter.xcu; filter them out in the
# find shell command below (see issue 110041):

$(MISC)/lang/fcfg_langpack_%.xcd .ERRREMOVE :
    $(MKDIRHIER) $(@:d)
    rm -rf $(MISC)/$(@:b).unzip
    mkdir $(MISC)/$(@:b).unzip
    cd $(MISC)/$(@:b).unzip && unzip $(SOLARPCKDIR)/$(@:b).zip
    - $(RM) $(MISC)/$(@:b).list
    # filter out filenames starting with "."
    echo '<list>' $(foreach,i,$(shell cd $(MISC) && \
        find $(@:b).unzip -name \[!.\]\*.xcu -size +0c -print) \
        '<filename>$i</filename>') '</list>' > $(MISC)/$(@:b).list
    $(XSLTPROC) --nonet --stringparam prefix $(PWD)/$(MISC)/ -o $@ \
        $(SOLARENV)/bin/packregistry.xslt $(MISC)/$(@:b).list

$(MISC)/lang/registry_{$(alllangiso)}.xcd : $(SOLARPCKDIR)/$$(@:b).zip \
        $(SOLARPCKDIR)/fcfg_drivers_$$(@:b:s/registry_//).zip

$(MISC)/lang/registry_%.xcd .ERRREMOVE :
    $(MKDIRHIER) $(@:d)
    rm -rf $(MISC)/$(@:b).unzip
    mkdir $(MISC)/$(@:b).unzip
    cd $(MISC)/$(@:b).unzip && unzip $(SOLARPCKDIR)/$(@:b).zip
    rm -rf $(MISC)/fcfg_drivers_$*.unzip
    mkdir $(MISC)/fcfg_drivers_$*.unzip
    cd $(MISC)/fcfg_drivers_$*.unzip && \
        unzip $(SOLARPCKDIR)/fcfg_drivers_$*.zip
    - $(RM) $(MISC)/$(@:b).list
    # filter out filenames starting with "."
    echo '<list>' $(foreach,i,$(shell cd $(MISC) && \
        find $(@:b).unzip fcfg_drivers_$*.unzip -name \[!.\]\*.xcu -print) \
        '<filename>$i</filename>') '</list>' > $(MISC)/$(@:b).list
    $(XSLTPROC) --nonet --stringparam prefix $(PWD)/$(MISC)/ -o $@ \
        $(SOLARENV)/bin/packregistry.xslt $(MISC)/$(@:b).list
