@echo off

set ACTUAL_DIR=%CD%
rem Set correct path to your Qt runtime
set QT_PATH=C:\Qt
set QT_PATH_RUNTIME=%QT_PATH%\5.8\msvc2015_64

if not exist build_zlib mkdir build_zlib
cd build_zlib

%QT_PATH_RUNTIME%\bin\qmake %ACTUAL_DIR%\zlib\zlib.pro
%QT_PATH%\Tools\QtCreator\bin\jom.exe

cd ..

if not exist build_quazip mkdir build_quazip
cd build_quazip

%QT_PATH_RUNTIME%\bin\qmake %ACTUAL_DIR%\quazip\quazip.pro
%QT_PATH%\Tools\QtCreator\bin\jom.exe

cd ..
