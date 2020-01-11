#!/bin/bash

#
# detect version information from github
#
COMMIT_NUMBER=`git rev-list --all --count`
VERSION_NAME=`git describe --tags $(git rev-list --tags --max-count=1)`

#
# write a header file
#
echo "#define version_from_git \"${VERSION_NAME%%-*}.$COMMIT_NUMBER\"" > src/garlic-lib/version.h

#
# write version information to Androidmanifest
#
xmlstarlet edit --inplace --update "/manifest/@android:versionName" --value $VERSION_NAME src/player-c2qml/AndroidManifest.xml
xmlstarlet edit --inplace --update "/manifest/@android:versionCode" --value $COMMIT_NUMBER src/player-c2qml/AndroidManifest.xml

echo ${VERSION_NAME}.$COMMIT_NUMBER
