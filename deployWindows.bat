@echo off
rem execute this script in a VS Native CMD

rem put all files needed and structured in %DEPLOYMENT_HELPER_PATH%
set DEPLOYMENT_HELPER_PATH=C:\garlic-deployment
if "%DEPLOYMENT_HELPER_PATH%" == "" (
	echo on
	echo Error: Set the correct paths for DEPLOYMENT_HELPER_PATH 
	goto END
)

set QT_PATH=C:\Qt
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

%QT_PATH_RUNTIME%\bin\windeployqt.exe -opengl -webengine bin\garlic-player.exe
missing Qt-Dlls which windeplayqt is not able to copy
copy /Y  %QT_PATH_RUNTIME%\bin\Qt5WebSockets.dll bin\*
copy /Y  %QT_PATH_RUNTIME%\bin\Qt5WebView.dll bin\*
copy /Y  %QT_PATH_RUNTIME%\bin\Qt5XmlPatterns.dll bin\*

copy QtAV-Dlls
copy /Y  %QT_PATH_RUNTIME%\bin\QtAV1.dll bin\*
copy /Y  %QT_PATH_RUNTIME%\bin\QtAVWidgets1.dll bin\*

copy ffmpeg dlls
copy /Y %QT_PATH_RUNTIME%\bin\av*.dll bin\*
copy /Y  %QT_PATH_RUNTIME%\bin\sw*.dll bin\*
copy /Y  %DEPLOYMENT_HELPER_PATH%\libeay32.dll bin\*
copy /Y  %DEPLOYMENT_HELPER_PATH%\ssleay32.dll bin\*

if not exist deployment mkdir deployment
xcopy ..\deployment\win\* deployment\* /sy

if not exist deployment\packages\com.microsoft.vcredist.x64\data mkdir deployment\packages\com.microsoft.vcredist.x64\data
move bin\vcredist_x64.exe deployment\packages\com.microsoft.vcredist.x64\data

if not exist deployment\packages\com.microsoft.directx.xaudio2\data mkdir deployment\packages\com.microsoft.directx.xaudio2\data
copy /Y %DEPLOYMENT_HELPER_PATH%\dx_sdk2010_xaudio2_redist\* deployment\packages\com.microsoft.directx.xaudio2\data\*

if not exist deployment\packages\com.sagiadinos.garlic.x64\data\bin mkdir deployment\packages\com.sagiadinos.garlic.x64\data\bin
xcopy bin\* deployment\packages\com.sagiadinos.garlic.x64\data\bin\* /sy


%QT_PATH%\Tools\QtInstallerFramework\2.0\bin\binarycreator -c  deployment\config\config.xml -p  deployment\packages garlic-installer.exe

:END
cd ..

