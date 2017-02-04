#!/bin/bash
#
# installs dependencies and runs default build

git submodule update --init

if ! type "doit" > /dev/null; then
    sudo pip2 install "doit==0.27"
fi

doit