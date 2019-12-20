#!/bin/bash

ls -la
mkdir -p dependencies && cd dependencies

# install root

if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then export ROOT_BUILD="Linux-ubuntu18-x86_64-gcc7.4";
elif [[ "${TRAVIS_OS_NAME}" == "osx" ]]; then export ROOT_BUILD="macosx64-";
    if [[ "${TRAVIS_OSX_IMAGE}" == "xcode10.13" ]]; then export ROOT_BUILD=$ROOT_BUILD"10.13-clang100";
    elif [[ "${TRAVIS_OSX_IMAGE}" == "xcode10.14" ]]; then export ROOT_BUILD=$ROOT_BUILD"10.14-clang100";
    fi;
fi
export ROOT_BIN="root_v${ROOT_VERSION}.${ROOT_BUILD}.tar.gz"
wget https://root.cern.ch/download/${ROOT_BIN}
tar -xf ${ROOT_BIN}
source root/bin/thisroot.sh
root-config --version

cd ..
