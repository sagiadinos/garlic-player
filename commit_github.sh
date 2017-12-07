#!/bin/bash
export WORK_DIR="/home/niko/source/garlic-player"
export GIT_DIR="/home/niko/source/github/garlic-player"
export PARAMS="-ah --exclude=*.user --exclude=build* --progress"

echo
echo ===================
echo rsync deployment
echo ===================
echo

rsync $PARAMS $WORK_DIR/deployment  $GIT_DIR

echo
echo ===================
echo rsync src
echo ===================
echo

rsync $WORK_DIR/src/* $GIT_DIR/src  # without directorys
rsync $PARAMS $WORK_DIR/src/ext  $GIT_DIR/src
rsync $PARAMS $WORK_DIR/src/parser  $GIT_DIR/src
rsync $PARAMS $WORK_DIR/src/player-qml $GIT_DIR/src
rsync $PARAMS $WORK_DIR/src/player-widget $GIT_DIR/src

echo
echo ===================
echo rsync tests
echo ===================
echo

rsync $PARAMS $WORK_DIR/tests  $GIT_DIR/tests

cd /home/niko/source/github/garlic-player/

git add *

exit 0;
