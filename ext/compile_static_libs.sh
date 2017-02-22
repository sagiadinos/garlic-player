check_qmake()
{
  [ -n "$QMAKEPATH" ] || {
    echo "Input your absolute path of qmake"
    echo "example: $HOME/Qt5.7.1/5.7/gcc_64/bin/"
    read QMAKEPATH
  }
	if [ -f "$QMAKEPATH" ]
	then
		echo "$QMAKEPATH found."
	else
		echo "$QMAKEPATH not found."
		exit 1
	fi
}


check_qmake


cd zlib-1.2.11 
./configure --prefix="../" --static
make -j 12
cp libz.a ../libz.a
make distclean

cd ../quazip
$QMAKEPATH
make -j 12

exit 0

