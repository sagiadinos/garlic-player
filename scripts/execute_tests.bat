@ECHO off

REM =============================================== 
REM execute this script in a VS Native CMD
REM =============================================== 
if "%VSINSTALLDIR%" =="" (
	CALL "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
)

REM =============================================== 
REM config your paths
REM ===============================================
SET QT_VERSION=
SET QT_API_VERSION=
SET QT_BASE_PATH=

REM Attention! Spaces after SETs produse error paths
SET QT_PATH=%QT_BASE_PATH%\%QT_VERSION%\%QT_API_VERSION%
if "%QT_PATH%" == "" (
	@ECHO on
	ECHO Error: Set the correct paths for QT_PATH 
	@ECHO off
	goto END
)

if not exist execute_tests mkdir execute_tests
cd execute_tests

REM =============================================== 
REM create garlic lib first
REM ===============================================
%QT_PATH%\bin\qmake.exe ..\src\libs_only.pro "CONFIG+=debug"
%QT_BASE_PATH%\Tools\QtCreator\bin\jom.exe

REM =============================================== 
REM execute tests
REM ===============================================
SET PATH=%PATH%;C:\Qt\5.9.3\msvc2015_64\bin;lib;
%QT_PATH%\bin\qmake.exe ../tests/testsuite.pro -r -spec win32-msvc
%QT_BASE_PATH%\Tools\QtCreator\bin\jom.exe

:END
cd ..