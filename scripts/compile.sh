#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

command_exists () {
    type "$1" &> /dev/null ;
}

NUM_THREADS=8
BUILD_MODE=$1
RELEASE_OR_DEBUG=$2

if [ -z ${BUILD_MODE} ] 
then
	BUILD_MODE="EDITOR"
	echo "Build mode not specified. Picking EDITOR mode (default)."
	echo "If you want to change it, specify as first argument: EDITOR or GAME"
	echo 
fi
sleep 1
if [ -z ${RELEASE_OR_DEBUG} ] 
then
	RELEASE_OR_DEBUG="RELEASE_MODE"
	echo "Release or Debug not specified. Picking RELEASE_MODE mode (default)."
	echo "If you want to change it, specify as second argument: RELEASE_MODE or DEBUG_MODE"
	echo 
fi
sleep 1

# Make and run Bang
echo "BUILD_MODE on compile.sh:       '${BUILD_MODE}'"
echo "RELEASE_OR_DEBUG on compile.sh: '${RELEASE_OR_DEBUG}'"
qmake "BUILD_MODE=${BUILD_MODE}" "RELEASE_OR_DEBUG=${RELEASE_OR_DEBUG}"
if [ $(command_exists colormake) ] && [ "${BUILD_MODE}" != "GAME" ]
then
	colormake -j${NUM_THREADS}
else
	make -j${NUM_THREADS}
fi
