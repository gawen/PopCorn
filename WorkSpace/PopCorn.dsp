# Microsoft Developer Studio Project File - Name="PopCorn" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=POPCORN - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PopCorn.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PopCorn.mak" CFG="POPCORN - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PopCorn - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PopCorn___Win32_Release"
# PROP BASE Intermediate_Dir "PopCorn___Win32_Release"
# PROP BASE Cmd_Line "NMAKE /f PopCorn.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "PopCorn.exe"
# PROP BASE Bsc_Name "PopCorn.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PopCorn___Win32_Release"
# PROP Intermediate_Dir "PopCorn___Win32_Release"
# PROP Cmd_Line "make Makefile.win"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "PopCorn.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""
# Begin Target

# Name "PopCorn - Win32 Release"

!IF  "$(CFG)" == "PopCorn - Win32 Release"

!ENDIF 

# Begin Group "PopCorn"

# PROP Default_Filter ""
# Begin Group "Configuration"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\AudioConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\src\EventsConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VideoConfig.cpp
# End Source File
# End Group
# Begin Group "2d"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Engine2d.cpp
# End Source File
# End Group
# Begin Group "Extensions"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\GLExtensions.cpp
# End Source File
# End Group
# Begin Group "Audio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\ObjSound.cpp
# End Source File
# End Group
# Begin Group "Objets"

# PROP Default_Filter ""
# Begin Group "Graphique"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Obj3dMeshes.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Obj3DSObject.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjList.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjParticles.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjPlane.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjShapes.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjTeapot.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjTerrain.cpp
# End Source File
# End Group
# Begin Group "Fichier"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\ObjConfigFile.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjLinesFile.cpp
# End Source File
# End Group
# Begin Group "Système"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Objects.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjFunction.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\Object.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ParticlesPlugIn_Flam.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Picking.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Scene.cpp
# End Source File
# End Group
# Begin Group "Materiaux"

# PROP Default_Filter ""
# Begin Group "Graphique No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\ObjImage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TexImage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TexImageVid.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TextureColorVid.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TextureMulti.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\Material.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjFont.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\Camera.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Debug.cpp
# End Source File
# Begin Source File

SOURCE=..\src\DynamicArray.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Kernel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Lights.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=..\src\PopCorn.cpp
# End Source File
# Begin Source File

SOURCE=..\src\popcorn.hpp
# End Source File
# Begin Source File

SOURCE=..\src\Timer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Types.cpp
# End Source File
# End Group
# Begin Group "WaterQuest"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\wq\src\debug_.cpp
# End Source File
# Begin Source File

SOURCE=..\wq\src\main.cpp
# End Source File
# Begin Source File

SOURCE=..\wq\src\menu.cpp
# End Source File
# Begin Source File

SOURCE=..\wq\src\splash.cpp
# End Source File
# Begin Source File

SOURCE=..\wq\src\video.cpp
# End Source File
# Begin Source File

SOURCE=..\wq\src\waterquest.hpp
# End Source File
# End Group
# End Target
# End Project
