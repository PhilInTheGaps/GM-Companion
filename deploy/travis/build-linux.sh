#!/bin/bash

echo "Building gm-companion ..."

qmake CONFIG+=release PREFIX=/usr
make -j$(nproc)
