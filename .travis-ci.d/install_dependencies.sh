#!/bin/bash

ls -la
mkdir -p dependencies && cd dependencies

# install root

if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then export ROOT_BUILD="Linux-ubuntu18-x86_64-gcc7.5.tar.gz";
elif [[ "${TRAVIS_OS_NAME}" == "osx" ]]; then export ROOT_BUILD="macosx64-10.14-clang100.tar.gz";
elif [[ "${TRAVIS_OS_NAME}" == "windows" ]]; then export ROOT_BUILD="win32.vc16.zip";
fi
export ROOT_BIN="root_v${ROOT_VERSION}.${ROOT_BUILD}"
wget https://root.cern.ch/download/${ROOT_BIN}

if [[ "${TRAVIS_OS_NAME}" != "windows" ]]; then tar -xf ${ROOT_BIN} ;
else unzip ${ROOT_BIN} ; choco feature enable -n=allowGlobalConfirmation ; choco install openssl ;
fi

source root/bin/thisroot.sh
root-config --version

cd ..
