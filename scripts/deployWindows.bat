@echo off
rem =============================================== 
rem execute this script in a VS Native CMD
rem =============================================== 
if "%VSINSTALLDIR%" =="" (
	call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
)

rem put all files needed and structured in %DEPLOYMENT_HELPER_PATH%
set DEPLOYMENT_HELPER_PATH=
if "%DEPLOYMENT_HELPER_PATH%" == "" (
	@echo on
	echo Error: Set the correct paths for DEPLOYMENT_HELPER_PATH 
	@echo off
	goto END
)

set QT_PATH=
set QT_PATH_RUNTIME=%QT_PATH%\5.8\msvc2015_64
if "%QT_PATH%" == "" (
	@echo on
	echo Error: Set the correct paths for QT_PATH 
	@echo off
	goto END
)

set SEVEN_ZIP_PATH="C:\Program Files\7-Zip\7z.exe"
rem not work with numbers  %7ZIP_PATH%
if not exist %SEVEN_ZIP_PATH% (
	@echo on
	echo Error: You need 7zip from http://www.7-zip.org/
	@echo off
	goto END
)

if not exist build mkdir build
cd build

%QT_PATH_RUNTIME%\bin\qmake.exe ..\src\complete.pro
%QT_PATH%\Tools\QtCreator\bin\jom.exe

%QT_PATH_RUNTIME%\bin\windeployqt.exe -opengl -webengine bin\garlic-player.exe
rem copy missing Qt-Dlls which windeplayqt seems is not able to handle
copy /Y  %QT_PATH_RUNTIME%\bin\Qt5WebSockets.dll bin\*
copy /Y  %QT_PATH_RUNTIME%\bin\Qt5WebView.dll bin\*
copy /Y  %QT_PATH_RUNTIME%\bin\Qt5XmlPatterns.dll bin\*

rem copy QtAV-Dlls
copy /Y  %QT_PATH_RUNTIME%\bin\QtAV1.dll bin\*
copy /Y  %QT_PATH_RUNTIME%\bin\QtAVWidgets1.dll bin\*

rem copy ffmpeg dlls
copy /Y %QT_PATH_RUNTIME%\bin\av*.dll bin\*
copy /Y  %QT_PATH_RUNTIME%\bin\sw*.dll bin\*
copy /Y  %DEPLOYMENT_HELPER_PATH%\libeay32.dll bin\*
copy /Y  %DEPLOYMENT_HELPER_PATH%\ssleay32.dll bin\*

if not exist deployment mkdir deployment
xcopy ..\deployment\win\* deployment\* /sy

rem create copy Visual Studio runtime
if not exist deployment\packages\com.microsoft.vcredist.x64\data mkdir deployment\packages\com.microsoft.vcredist.x64\data
move bin\vcredist_x64.exe deployment\packages\com.microsoft.vcredist.x64\data

rem create copy xaudio2 install
if not exist deployment\packages\com.microsoft.directx.xaudio2\data mkdir deployment\packages\com.microsoft.directx.xaudio2\data
copy /Y %DEPLOYMENT_HELPER_PATH%\dx_sdk2010_xaudio2_redist\* deployment\packages\com.microsoft.directx.xaudio2\data\*

rem create/copy bin data icon and start-batch into install dir
if not exist deployment\packages\com.sagiadinos.garlic.x64\data\bin mkdir deployment\packages\com.sagiadinos.garlic.x64\data\bin
xcopy bin\* deployment\packages\com.sagiadinos.garlic.x64\data\bin\* /sy
copy ..\deployment\win\garlic-player.ico deployment\packages\com.sagiadinos.garlic.x64\data\bin
copy /Y ..\deployment\win\start_garlic.bat deployment\packages\com.sagiadinos.garlic.x64\data

rem create install archive
%QT_PATH%\Tools\QtInstallerFramework\2.0\bin\binarycreator -c  deployment\config\config.xml -p  deployment\packages garlic-installer.exe

rem create 7zip archive (7Zip is needed to installed)
%SEVEN_ZIP_PATH% a garlic-player.7z  .\deployment\packages\com.sagiadinos.garlic.x64\data\*
cd ..

:END

