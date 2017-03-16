#
#Set correct paths to your Qt runtime
#
export QT_PATH=/home/niko/Anwendungen/Qt
export QT_PATH_RUNTIME=$QT_PATH/5.8/gcc_64
if [ -z "$QT_PATH" ]; then
	echo Error: Set the correct paths in QT_PATH 
	exit 1;
fi

echo build zlib...

mkdir -p build_zlib
cd build_zlib
$QT_PATH_RUNTIME/bin/qmake ../zlib/zlib.pro
make -j 12 --silent
cd ..

echo build quazip...
mkdir -p build_quazip
cd build_quazip
$QT_PATH_RUNTIME/bin/qmake ../quazip/quazip.pro
make -j 12 --silent

echo static files found in ../../lib
exit 0;
