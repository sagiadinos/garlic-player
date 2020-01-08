#!/bin/bash
revisioncount=`git rev-list --all --count`
projectversion=`git describe --tags $(git rev-list --tags --max-count=1)`
cleanversion=${projectversion%%-*}
echo "#define version_from_git \"$cleanversion.$revisioncount\"" > src/garlic-lib/version.h
