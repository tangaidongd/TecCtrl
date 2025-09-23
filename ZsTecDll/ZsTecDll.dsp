# Microsoft Developer Studio Project File - Name="ZsTecDll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ZsTecDll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ZsTecDll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ZsTecDll.mak" CFG="ZsTecDll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ZsTecDll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ZsTecDll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ZsTecDll - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "ZsTecDll - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy debug\*.dll export	copy debug\*.lib export	update-x32d
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ZsTecDll - Win32 Release"
# Name "ZsTecDll - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ZsTecDll.cpp
# End Source File
# Begin Source File

SOURCE=.\ZsTecDll.def
# End Source File
# Begin Source File

SOURCE=.\ZsTecDll.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ZsTecDll.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ZsTecDll.rc2
# End Source File
# End Group
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\TecBase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TecBase.h
# End Source File
# Begin Source File

SOURCE=.\src\TecV1.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TecV1.h
# End Source File
# Begin Source File

SOURCE=.\src\TecV2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TecV2.h
# End Source File
# Begin Source File

SOURCE=.\src\zsTec.cpp
# End Source File
# Begin Source File

SOURCE=.\src\zsTec.h
# End Source File
# End Group
# Begin Group "export"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\export\zsTecDll.h
# End Source File
# End Group
# Begin Group "libs"

# PROP Default_Filter ""
# Begin Group "RsCommDll"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libs\RsCommDll\RsCommDll.config
# End Source File
# Begin Source File

SOURCE=.\libs\RsCommDll\RsCommDll.h
# End Source File
# End Group
# Begin Group "ZsFdb"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libs\ZsFdb\ZsFdb.config
# End Source File
# Begin Source File

SOURCE=.\libs\ZsFdb\ZsFdb.h
# End Source File
# End Group
# Begin Group "ZsIni"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libs\ZsIni\ZsIni.h
# End Source File
# Begin Source File

SOURCE=.\libs\ZsIni\ZsIni_config.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
