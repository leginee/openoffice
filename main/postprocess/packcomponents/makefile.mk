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
TARGET = packcomponents

.INCLUDE: settings.mk

#TODO (copied from scp2/source/ooo/makefile.mk):
.IF "$(OS)" != "MACOSX" && "$(OS)" != "WNT" && "$(ENABLE_GTK)" != ""
PKGCONFIG_MODULES=gtk+-2.0
.INCLUDE: pkg_config.mk
GTK_TWO_FOUR=$(shell @+-$(PKG_CONFIG) --exists 'gtk+-2.0 >= 2.4.0' && echo YES)
.END

my_components = \
    bootstrap \
    introspection \
    invocadapt \
    invocation \
    javaloader \
    javavm \
    namingservice \
    proxyfac \
    reflection \
    stocservices \
    abp \
    analysis \
    basprov \
    bib \
    calc \
    chartcontroller \
    chartmodel \
    charttools \
    chartview \
    component/UnoControls/util/ctl \
    component/animations/source/animcore/animcore \
    component/avmedia/util/avmedia \
    component/basctl/util/basctl \
    component/binaryurp/source/binaryurp \
    component/canvas/source/factory/canvasfactory \
    component/canvas/source/simplecanvas/simplecanvas \
    component/canvas/source/vcl/vclcanvas \
    component/comphelper/util/comphelp \
    component/configmgr/source/configmgr \
    component/cui/util/cui \
    component/drawinglayer/drawinglayer \
	component/dbaccess/source/ext/adabas/adabasui \
	component/dbaccess/source/ext/macromigration/dbmm \
	component/dbaccess/source/filter/xml/dbaxml \
	component/dbaccess/util/dba \
	component/dbaccess/util/dbu \
	component/dbaccess/util/sdbt \
    component/dtrans/util/mcnttype \
    component/embeddedobj/util/embobj \
    component/eventattacher/source/evtatt \
    component/fileaccess/util/fileacc \
    component/formula/util/for \
    component/fpicker/source/generic/fpicker \
    component/fpicker/source/office/fps_office \
    component/framework/util/fwk \
    component/framework/util/fwl \
    component/framework/util/fwm \
    component/io/source/acceptor/acceptor \
    component/io/source/connector/connector \
    component/io/source/stm/streams \
    component/io/source/TextInputStream/textinstream \
    component/io/source/TextOutputStream/textoutstream \
    component/javaunohelper/util/juh \
    component/linguistic/source/lng \
    component/oox/util/oox \
    component/package/source/xstor/xstor \
    component/package/util/package2 \
	component/reportdesign/util/rptxml \
	component/reportdesign/util/rpt \
	component/reportdesign/util/rptui \
    component/vbahelper/util/msforms \
    component/remotebridges/source/unourl_resolver/uuresolver \
    component/sax/source/expatwrap/expwrap \
    component/sax/source/fastparser/fastsax \
    component/sc/util/sc \
    component/sc/util/scd \
    component/sc/util/vbaobj \
    component/sd/util/sd \
    component/sd/util/sdd \
    component/sfx2/util/sfx \
    component/slideshow/util/slideshow \
    component/sot/util/sot \
    component/svgio/svgio \
    component/starmath/util/sm \
    component/starmath/util/smd \
    component/svl/source/fsstor/fsstorage \
    component/svl/source/passwordcontainer/passwordcontainer \
    component/svl/util/svl \
    component/svtools/source/hatchwindow/hatchwindowfactory \
    component/svtools/util/svt \
    component/svx/util/svx \
    component/svx/util/svxcore \
    component/svx/util/textconversiondlgs \
    component/sw/util/msword \
    component/sw/util/sw \
    component/sw/util/swd \
    component/sw/util/vbaswobj \
    component/unotools/util/utl \
    component/uui/util/uui \
    component/toolkit/util/ootk \
    component/ucb/source/cacher/cached1 \
    component/ucb/source/core/ucb1 \
    component/ucb/source/sorter/srtrs1 \
    component/ucb/source/ucp/expand/ucpexpand1 \
    component/ucb/source/ucp/ext/ucpext \
    component/ucb/source/ucp/file/ucpfile1 \
    component/ucb/source/ucp/ftp/ucpftp1 \
    component/ucb/source/ucp/hierarchy/ucphier1 \
    component/ucb/source/ucp/package/ucppkg1 \
    component/ucb/source/ucp/tdoc/ucptdoc1 \
    component/ucb/source/ucp/webdav/ucpdav1 \
    component/unoxml/source/rdf/unordf \
    component/unoxml/source/service/unoxml \
    component/writerfilter/util/writerfilter \
    component/xmloff/source/transform/xof \
    component/xmloff/util/xo \
    component/xmlscript/util/xcr \
    date \
    dbase \
    dbp \
    dbpool2 \
    dbtools \
    deployment \
    deploymentgui \
    dlgprov \
    filterconfig1 \
    flash \
    flat \
    frm \
    guesslang \
    i18npool \
    i18nsearch \
    lnth \
    localebe1 \
    log \
	mailmerge \
    migrationoo2 \
    msfilter \
    mysql \
    odbc \
    offacc \
    oooimprovecore \
    pcr \
    pdffilter \
    placeware \
    preload \
    protocolhandler \
    pythonloader \
    pythonscript \
    res \
    sb \
    scn \
    scriptframe \
    sdbc2 \
    spl \
    stringresource \
    svgfilter \
    syssh \
    t602filter \
    tvhlp1 \
    ucpchelp1 \
    updatefeed \
    updchk \
    vbaevents \
    xmlfa \
    xmlfd \
    xmx \
    xsltdlg \
    PresentationMinimizer \
    PresenterScreen

.IF "$(OS)" == "MACOSX"
my_components += component/vcl/vcl.macosx
.ELSE
.IF "$(OS)" == "WNT" || "$(OS)" == "OS2"
my_components += component/vcl/vcl.windows
.ELSE
my_components += component/vcl/vcl.unx
.ENDIF
.ENDIF

.IF "$(ENABLE_ONLINE_UPDATE)"=="YES"
my_components += updchk.uno
.END

.IF "$(OS)" != "WNT" && "$(OS)" != "OS2"
my_components += mozbootstrap
.END

.IF "$(ENABLE_OOOIMPROVEMENT)" != ""
my_components += oooimprovement
.END

.IF "$(ENABLE_COINMP)"=="YES"
my_components += component/sccomp/source/solver/solver
.END

.IF "$(DISABLE_SAXON)" == ""
my_components += XSLTFilter.jar \
    xsltfilter
.END

.IF "$(ENABLE_HYPHEN)" == "YES"
my_components += hyphen
.END

.IF "$(ENABLE_HUNSPELL)" == "YES"
my_components += spell
.END

.IF "$(SYSTEM_LIBWPD)" == "YES"
my_components +=     wpft
.END

.IF "$(OS)" == "WNT"
my_components += component/embeddedobj/source/msole/emboleobj.windows
.ELSE
my_components += component/embeddedobj/source/msole/emboleobj
.END

.IF "$(ENABLE_CAIRO_CANVAS)" == "TRUE"
my_components += component/canvas/source/cairo/cairocanvas
.END

.IF "$(ENABLE_GCONF)" != ""
my_components += gconfbe1
.END

.IF "$(ENABLE_GIO)" != ""
my_components += component/ucb/source/ucp/gio/ucpgio
.END

.IF "$(ENABLE_GNOMEVFS)" != ""
my_components += component/ucb/source/ucp/gvfs/ucpgvfs
.END

.IF "$(ENABLE_KDE)" != ""
my_components += kdebe1
.END

.IF "$(ENABLE_KDE4)" != ""
my_components += kde4be1
.END

.IF "$(ENABLE_OPENGL)" == "TRUE"
my_components += component/slideshow/source/engine/OGLTrans/ogltrans
.END

.IF "$(SOLAR_JAVA)" == "TRUE"
my_components += \
    LuceneHelpWrapper \
    ScriptFramework \
    ScriptProviderForJava \
    XMergeBridge \
    XSLTValidate \
    component/wizards/com/sun/star/wizards/agenda/agenda \
    component/wizards/com/sun/star/wizards/fax/fax \
    component/wizards/com/sun/star/wizards/form/form \
    hsqldb \
    jdbc \
    postgresql \
    component/wizards/com/sun/star/wizards/letter/letter \
    component/wizards/com/sun/star/wizards/query/query \
    component/wizards/com/sun/star/wizards/report/report \
    component/wizards/com/sun/star/wizards/table/table \
    component/wizards/com/sun/star/wizards/web/web
.IF "$(ENABLE_BEANSHELL)" == "YES"
my_components += ScriptProviderForBeanShell
.END
.IF "$(ENABLE_JAVASCRIPT)" == "YES"
my_components += ScriptProviderForJavaScript
.END
.END

.IF "$(WITH_LDAP)" == "YES"
my_components += ldapbe2
.END

.IF "$(ENABLE_NSS_MODULE)" != "NO"
my_components += \
    xmlsecurity \
    xsec_fw \
    xsec_xmlsec
.END

.IF "$(OS)" == "MACOSX"
my_components += \
    component/avmedia/source/macavf/avmediaMacAVF \
    component/avmedia/source/quicktime/avmediaQuickTime \
    component/fpicker/source/aqua/fps_aqua \
    macab1 \
    macbe1 \
    MacOSXSpell
.END

.IF "$(OS)" == "WNT"
my_components += \
    ado \
    component/avmedia/source/win/avmediawin \
    component/dtrans/source/generic/dtrans \
    component/dtrans/util/dnd \
    component/dtrans/util/ftransl \
    component/dtrans/util/sysdtrans \
    component/fpicker/util/fop \
    component/fpicker/util/fps \
    java_uno_accessbridge \
    sysmail \
    winaccessibility \
    wininetbe1
.END

.IF "$(OS)" == "OS2"
my_components += \
    component/dtrans/source/generic/dtrans \
    component/dtrans/util/sysdtrans
.END

.IF "$(OS)" == "WNT" && "$(DISABLE_ATL)" == ""
my_components += \
    emser \
    oleautobridge
.END

.IF "$(OS)" == "WNT" && "$(ENABLE_DIRECTX)" != ""
my_components += \
    component/canvas/source/directx/directx9canvas \
    component/canvas/source/directx/gdipluscanvas
.END

.IF "$(OS)" == "WNT" && "$(ENABLE_DIRECTX)" != "" && "$(USE_DIRECTX5)" != ""
my_components += component/canvas/source/directx/directx5canvas
.END

.IF "$(OS)" == "LINUX" || "$(OS)" == "NETBSD" || \
    ("$(OS)" == "SOLARIS" && "$(CPU)" == "S") || "$(OS)" == "WNT"
my_components += adabas
.END

.IF "$(OS)" != "MACOSX" && "$(OS)" != "WNT" && "$(OS)" != "OS2"
my_components += desktopbe1
.END

.IF "$(GTK_TWO_FOUR)" != ""
my_components += component/fpicker/source/unx/gnome/fps_gnome
.END

.IF "$(OS)" != "MACOSX" && "$(OS)" != "WNT" && "$(ENABLE_KDE4)" != ""
my_components += component/fpicker/source/unx/kde4/fps_kde4
.END

.IF "$(OS)" != "WNT"
my_components += cmdmail
.END

.IF "$(OS)" != "WNT" && "$(ENABLE_EVOAB2)" != ""
my_components += evoab
.END

.IF "$(OS)" != "WNT" && "$(ENABLE_GSTREAMER)" != ""
my_components += component/avmedia/source/gstreamer/avmediagst
.END

.IF "$(OS)" != "WNT" && "$(SOLAR_JAVA)" == "TRUE"
my_components += component/avmedia/source/java/avmedia.jar
.END

my_ooo_components = mailmerge

.INCLUDE: target.mk

ALLTAR : $(MISC)/services.rdb $(MISC)/ooo-services.rdb

$(MISC)/services.rdb .ERRREMOVE : $(SOLARENV)/bin/packcomponents.xslt \
        $(MISC)/services.input $(my_components:^"$(SOLARXMLDIR)/":+".component")
    $(XSLTPROC) --nonet --stringparam prefix $(SOLARXMLDIR)/ -o $@ \
        $(SOLARENV)/bin/packcomponents.xslt $(MISC)/services.input

$(MISC)/services.input : makefile.mk
    echo \
        '<list>$(my_components:^"<filename>":+".component</filename>")</list>' \
        > $@

$(MISC)/ooo-services.rdb .ERRREMOVE : $(SOLARENV)/bin/packcomponents.xslt \
        $(MISC)/ooo-services.input \
        $(my_ooo_components:^"$(SOLARXMLDIR)/":+".component")
    $(XSLTPROC) --nonet --stringparam prefix $(SOLARXMLDIR)/ -o $@ \
        $(SOLARENV)/bin/packcomponents.xslt $(MISC)/ooo-services.input

$(MISC)/ooo-services.input : makefile.mk
    echo '<list>' \
        '$(my_ooo_components:^"<filename>":+".component</filename>")' \
        '</list>' > $@
