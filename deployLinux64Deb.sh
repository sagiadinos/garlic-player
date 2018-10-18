#!/bin/bash

#Set correct paths to your Qt runtime
export QT_PATH=/home/niko/Anwendungen/Qt
export QT_PATH_RUNTIME=$QT_PATH/5.9.4/gcc_64
export DEV_JOBS=$(grep -c "^processor" /proc/cpuinfo)
if [ -z "$QT_PATH" ]; then
	echo Error: Set the correct paths in QT_PATH 
	exit 1;
fi

echo 
echo ========== create player ========== 
echo 
mkdir -p buildLinux64
cd buildLinux64

echo build garlic player...
$QT_PATH_RUNTIME/bin/qmake ../src/complete.pro
make -j $DEV_JOBS --silent

# check if binary was created 
if [ ! -f bin/garlic-player ]; then
	echo Binary was not created!
    exit 1;
fi

echo 
echo =========== create deb ============ 
echo 
echo collect files  ...

export GARLIC_DEPLOY_BASE_PATH=deb/garlic-player
export GARLIC_DEPLOY_PATH=$GARLIC_DEPLOY_BASE_PATH/opt

mkdir -p $GARLIC_DEPLOY_PATH $GARLIC_DEPLOY_BASE_PATH
cp -r ../deployment/deb/garlic-player/* $GARLIC_DEPLOY_BASE_PATH

mkdir -p $GARLIC_DEPLOY_PATH/garlic-player/bin/platforms
cp $QT_PATH_RUNTIME/plugins/platforms/libqeglfs.so $GARLIC_DEPLOY_PATH/garlic-player/bin/platforms/libqeglfs.so
cp $QT_PATH_RUNTIME/plugins/platforms/libqxcb.so $GARLIC_DEPLOY_PATH/garlic-player/bin/platforms/libqxcb.so

mkdir -p $GARLIC_DEPLOY_PATH/garlic-player/bin/plugins/iconengines
cp $QT_PATH_RUNTIME/plugins/iconengines/* $GARLIC_DEPLOY_PATH/garlic-player/bin/plugins/iconengines

mkdir -p $GARLIC_DEPLOY_PATH/garlic-player/bin/plugins/imageformats
cp $QT_PATH_RUNTIME/plugins/imageformats/* $GARLIC_DEPLOY_PATH/garlic-player/bin/plugins/imageformats

mkdir -p $GARLIC_DEPLOY_PATH/garlic-player/bin/plugins/xcbglintegrations
cp $QT_PATH_RUNTIME/plugins/xcbglintegrations/* $GARLIC_DEPLOY_PATH/garlic-player/bin/plugins/xcbglintegrations

mkdir -p $GARLIC_DEPLOY_PATH/garlic-player/bin/qtwebengine_locales
cp $QT_PATH_RUNTIME/translations/qtwebengine_locales/* $GARLIC_DEPLOY_PATH/garlic-player/bin/qtwebengine_locales

mkdir -p $GARLIC_DEPLOY_PATH/garlic-player/bin/resources
cp $QT_PATH_RUNTIME/resources/icudtl.dat $GARLIC_DEPLOY_PATH/garlic-player/bin/resources
cp $QT_PATH_RUNTIME/resources/qtwebengine_resources_100p.pak $GARLIC_DEPLOY_PATH/garlic-player/bin/resources
cp $QT_PATH_RUNTIME/resources/qtwebengine_resources_200p.pak $GARLIC_DEPLOY_PATH/garlic-player/bin/resources
cp $QT_PATH_RUNTIME/resources/qtwebengine_resources.pak $GARLIC_DEPLOY_PATH/garlic-player/bin/resources

cp $QT_PATH_RUNTIME/libexec/QtWebEngineProcess $GARLIC_DEPLOY_PATH/garlic-player/bin
printf "[Paths]\nPrefix=." > $GARLIC_DEPLOY_PATH/garlic-player/bin/qt.conf

mkdir -p $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libavcodec.so.57 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libavdevice.so.57 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libavfilter.so.6 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libavformat.so.57 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libavresample.so.3 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libavutil.so.55 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libicudata.so.56 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libicui18n.so.56 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libicuio.so.56 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libicuuc.so.56 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5Core.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5DBus.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5Gui.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5Network.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5OpenGL.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5Positioning.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5PrintSupport.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5Qml.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5Quick.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5QuickWidgets.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5Sql.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5Svg.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5WebChannel.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5WebChannel.so.5.9 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5WebEngine.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5WebEngineCore.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5WebEngineWidgets.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5WebSockets.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5Widgets.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5XcbQpa.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5Xml.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQt5XmlPatterns.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQtAV.so.1 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libQtAVWidgets.so.1 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libswresample.so.2 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L $QT_PATH_RUNTIME/lib/libswscale.so.4 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L /usr/lib/x86_64-linux-gnu/libstdc++.so.6 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L /usr/lib/x86_64-linux-gnu/libevent-2.0.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L /usr/lib/x86_64-linux-gnu/libminizip.so.1 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L /usr/lib/x86_64-linux-gnu/libopus.so.0 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L /usr/lib/x86_64-linux-gnu/libsnappy.so.1 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L /usr/lib/x86_64-linux-gnu/libwebp.so.5 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L /usr/lib/x86_64-linux-gnu/libwebpdemux.so.1 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L /usr/lib/x86_64-linux-gnu/libxcb-shm.so.0 $GARLIC_DEPLOY_PATH/garlic-player/lib
cp -L /usr/lib/x86_64-linux-gnu/libXss.so.1 $GARLIC_DEPLOY_PATH/garlic-player/lib


#
# copy created binary to bin path  ...
#
cp bin/garlic-player $GARLIC_DEPLOY_PATH/garlic-player/bin/
mv $GARLIC_DEPLOY_BASE_PATH/garlic.sh $GARLIC_DEPLOY_PATH/garlic-player

echo create deb  ...
dpkg -b $GARLIC_DEPLOY_BASE_PATH/ $DEPLOY_BASE_NAME.deb

echo create tar.xz
tar -cJf $DEPLOY_BASE_NAME.tar.xz -C deb/garlic-player/opt .
