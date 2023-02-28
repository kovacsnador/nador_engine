!include x64.nsh
Name "atlas_maker SDK for GCC 64-bit"
OutFile "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}atlas_maker-2.1.91-gcc64.exe"
InstallDir "c:\atlas_maker-gcc64"

SetCompressor bzip2

Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

Section "atlas_maker SDK for GCC 64-bit (required)"
!ifdef WIN64
	${If} ${RunningX64}
	${DisableX64FSRedirection}
	${Endif}
!endif
	SectionIn RO
!ifdef GCC
	IfFileExists $SYSDIR/libturbojpeg.dll exists 0
!else
	IfFileExists $SYSDIR/turbojpeg.dll exists 0
!endif
	goto notexists
	exists:
!ifdef GCC
	MessageBox MB_OK "An existing version of the atlas_maker SDK for GCC 64-bit is already installed.  Please uninstall it first."
!else
	MessageBox MB_OK "An existing version of the atlas_maker SDK for GCC 64-bit or the TurboJPEG SDK is already installed.  Please uninstall it first."
!endif
	quit

	notexists:
	SetOutPath $SYSDIR
!ifdef GCC
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\libturbojpeg.dll"
!else
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}turbojpeg.dll"
!endif
	SetOutPath $INSTDIR\bin
!ifdef GCC
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\libturbojpeg.dll"
!else
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}turbojpeg.dll"
!endif
!ifdef GCC
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\libjpeg-62.dll"
!else
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}jpeg62.dll"
!endif
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}cjpeg.exe"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}djpeg.exe"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}jpegtran.exe"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}tjbench.exe"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}rdjpgcom.exe"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}wrjpgcom.exe"
	SetOutPath $INSTDIR\lib
!ifdef GCC
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\libturbojpeg.dll.a"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\libturbojpeg.a"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\libjpeg.dll.a"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\libjpeg.a"
!else
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}turbojpeg.lib"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}turbojpeg-static.lib"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}jpeg.lib"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\${BUILDDIR}jpeg-static.lib"
!endif
	SetOutPath $INSTDIR\lib\pkgconfig
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\pkgscripts\libjpeg.pc"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\pkgscripts\libturbojpeg.pc"
	SetOutPath $INSTDIR\lib\cmake\atlas_maker
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\pkgscripts\atlas_makerConfig.cmake"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\pkgscripts\atlas_makerConfigVersion.cmake"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\win\atlas_makerTargets.cmake"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\win\atlas_makerTargets-release.cmake"
!ifdef JAVA
	SetOutPath $INSTDIR\classes
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\java\turbojpeg.jar"
!endif
	SetOutPath $INSTDIR\include
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\jconfig.h"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\jerror.h"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\jmorecfg.h"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\jpeglib.h"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\turbojpeg.h"
	SetOutPath $INSTDIR\doc
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\README.ijg"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\README.md"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\LICENSE.md"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\example.c"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\libjpeg.txt"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\structure.txt"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\usage.txt"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\wizard.txt"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\tjexample.c"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\java\TJExample.java"
!ifdef GCC
	SetOutPath $INSTDIR\man\man1
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\cjpeg.1"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\djpeg.1"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\jpegtran.1"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\rdjpgcom.1"
	File "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg\wrjpgcom.1"
!endif

	WriteRegStr HKLM "SOFTWARE\atlas_maker-gcc64 2.1.91" "Install_Dir" "$INSTDIR"

	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\atlas_maker-gcc64 2.1.91" "DisplayName" "atlas_maker SDK v2.1.91 for GCC 64-bit"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\atlas_maker-gcc64 2.1.91" "UninstallString" '"$INSTDIR\uninstall_2.1.91.exe"'
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\atlas_maker-gcc64 2.1.91" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\atlas_maker-gcc64 2.1.91" "NoRepair" 1
	WriteUninstaller "uninstall_2.1.91.exe"
SectionEnd

Section "Uninstall"
!ifdef WIN64
	${If} ${RunningX64}
	${DisableX64FSRedirection}
	${Endif}
!endif

	SetShellVarContext all

	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\atlas_maker-gcc64 2.1.91"
	DeleteRegKey HKLM "SOFTWARE\atlas_maker-gcc64 2.1.91"

!ifdef GCC
	Delete $INSTDIR\bin\libjpeg-62.dll
	Delete $INSTDIR\bin\libturbojpeg.dll
	Delete $SYSDIR\libturbojpeg.dll
	Delete $INSTDIR\lib\libturbojpeg.dll.a
	Delete $INSTDIR\lib\libturbojpeg.a
	Delete $INSTDIR\lib\libjpeg.dll.a
	Delete $INSTDIR\lib\libjpeg.a
!else
	Delete $INSTDIR\bin\jpeg62.dll
	Delete $INSTDIR\bin\turbojpeg.dll
	Delete $SYSDIR\turbojpeg.dll
	Delete $INSTDIR\lib\jpeg.lib
	Delete $INSTDIR\lib\jpeg-static.lib
	Delete $INSTDIR\lib\turbojpeg.lib
	Delete $INSTDIR\lib\turbojpeg-static.lib
!endif
	Delete $INSTDIR\lib\pkgconfig\libjpeg.pc
	Delete $INSTDIR\lib\pkgconfig\libturbojpeg.pc
	Delete $INSTDIR\lib\cmake\atlas_maker\atlas_makerConfig.cmake
	Delete $INSTDIR\lib\cmake\atlas_maker\atlas_makerConfigVersion.cmake
	Delete $INSTDIR\lib\cmake\atlas_maker\atlas_makerTargets.cmake
	Delete $INSTDIR\lib\cmake\atlas_maker\atlas_makerTargets-release.cmake
!ifdef JAVA
	Delete $INSTDIR\classes\turbojpeg.jar
!endif
	Delete $INSTDIR\bin\cjpeg.exe
	Delete $INSTDIR\bin\djpeg.exe
	Delete $INSTDIR\bin\jpegtran.exe
	Delete $INSTDIR\bin\tjbench.exe
	Delete $INSTDIR\bin\rdjpgcom.exe
	Delete $INSTDIR\bin\wrjpgcom.exe
	Delete $INSTDIR\include\jconfig.h
	Delete $INSTDIR\include\jerror.h
	Delete $INSTDIR\include\jmorecfg.h
	Delete $INSTDIR\include\jpeglib.h
	Delete $INSTDIR\include\turbojpeg.h
	Delete $INSTDIR\uninstall_2.1.91.exe
	Delete $INSTDIR\doc\README.ijg
	Delete $INSTDIR\doc\README.md
	Delete $INSTDIR\doc\LICENSE.md
	Delete $INSTDIR\doc\example.c
	Delete $INSTDIR\doc\libjpeg.txt
	Delete $INSTDIR\doc\structure.txt
	Delete $INSTDIR\doc\usage.txt
	Delete $INSTDIR\doc\wizard.txt
	Delete $INSTDIR\doc\tjexample.c
	Delete $INSTDIR\doc\TJExample.java
!ifdef GCC
	Delete $INSTDIR\man\man1\cjpeg.1
	Delete $INSTDIR\man\man1\djpeg.1
	Delete $INSTDIR\man\man1\jpegtran.1
	Delete $INSTDIR\man\man1\rdjpgcom.1
	Delete $INSTDIR\man\man1\wrjpgcom.1
!endif

	RMDir "$INSTDIR\include"
	RMDir "$INSTDIR\lib\pkgconfig"
	RMDir "$INSTDIR\lib\cmake\atlas_maker"
	RMDir "$INSTDIR\lib\cmake"
	RMDir "$INSTDIR\lib"
	RMDir "$INSTDIR\doc"
!ifdef GCC
	RMDir "$INSTDIR\man\man1"
	RMDir "$INSTDIR\man"
!endif
!ifdef JAVA
	RMDir "$INSTDIR\classes"
!endif
	RMDir "$INSTDIR\bin"
	RMDir "$INSTDIR"

SectionEnd
