# Microsoft Developer Studio Project File - Name="Demeter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Demeter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Demeter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Demeter.mak" CFG="Demeter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Demeter - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Demeter - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Demeter"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Demeter - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DEMETER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TERRAIN_EXPORTS" /D "_PROTECT_ACCESS_" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib /nologo /dll /machine:I386 /out:"../bin/Demeter.dll" /implib:"../lib/Demeter.lib" /libpath:"../lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Demeter - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DEMETER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TERRAIN_EXPORTS" /D "_PROTECT_ACCESS_" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib /nologo /dll /debug /machine:I386 /out:"../bin/DemeterDebug.dll" /implib:"../lib/DemeterDebug.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Demeter - Win32 Release"
# Name "Demeter - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\Brush.cpp
# End Source File
# Begin Source File

SOURCE=..\src\DemeterEntryPoint.cpp
# End Source File
# Begin Source File

SOURCE=..\src\DetailTexture.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Globals.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Loader.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Plane.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Settings.cpp
# End Source File
# Begin Source File

SOURCE=..\src\SkyBox.cpp
# End Source File
# Begin Source File

SOURCE=..\src\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Terrain.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TerrainBlock.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TerrainLattice.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TerrainVertex.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TextureCell.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TextureFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TextureGenerator.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TextureSet.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Triangle.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TriangleFan.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TriangleStrip.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Vector.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\Demeter\BitArray.h
# End Source File
# Begin Source File

SOURCE=..\include\Demeter\Brush.h
# End Source File
# Begin Source File

SOURCE=..\include\Demeter\Demeter.h
# End Source File
# Begin Source File

SOURCE=..\include\Demeter\DemeterException.h
# End Source File
# Begin Source File

SOURCE=..\include\Demeter\Globals.h
# End Source File
# Begin Source File

SOURCE=..\include\Demeter\Loader.h
# End Source File
# Begin Source File

SOURCE=..\include\Demeter\SkyBox.h
# End Source File
# Begin Source File

SOURCE=..\include\Demeter\Terrain.h
# End Source File
# Begin Source File

SOURCE=..\include\Demeter\TerrainBlock.h
# End Source File
# Begin Source File

SOURCE=..\include\Demeter\TerrainLattice.h
# End Source File
# Begin Source File

SOURCE=..\include\Demeter\TerrainVertex.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
