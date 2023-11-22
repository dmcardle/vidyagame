#!/bin/sh
set -ex

sudo apt install -y bear build-essential clang-format gnuboy-sdl
rm -rf gbdk*
wget 'https://github.com/gbdk-2020/gbdk-2020/releases/download/4.1.1/gbdk-linux64.tar.gz'
tar xzvf gbdk-linux64.tar.gz
