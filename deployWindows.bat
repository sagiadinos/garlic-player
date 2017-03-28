@echo off
rem execute this script in a VS Native CMD

set QT_PATH=
set QT_PATH_RUNTIME=%QT_PATH%\5.8\msvc2015_64
if "%QT_PATH%" == "" (
	echo on
	echo Error: Set the correct paths for QT_PATH 
	goto END
)

if not exist build mkdir build
cd build

%QT_PATH_RUNTIME%\bin\qmake.exe ..\src\complete.pro
%QT_PATH%\Tools\QtCreator\bin\jom.exe

rem put all files needed and structured in C:\deployment\*

xcopy C:\deployment\bin\* ..\deployment\win\packages\com.sagiadinos.garlic.x64\data\bin\* /sy
copy ..\bin\garlic-player.exe ..\deployment\win\packages\com.sagiadinos.garlic.x64\data\bin\garlic-player.exe
copy C:\deployment\dx_sdk2010_xaudio2_redist\* ..\deployment\win\packages\com.microsoft.directx.xaudio2\data\*
copy C:\deployment\vc_redist.x64.exe ..\deployment\win\packages\com.microsoft.vcredist.x64\data\*

%QT_PATH%\Tools\QtInstallerFramework\2.0\bin\binarycreator -c  ..\deployment\win\config\config.xml -p  ..\deployment\win\packages ..\garlic-installer.exe


cd ..

:END
