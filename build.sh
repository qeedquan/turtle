#!/bin/sh

cd src
bison parse.y --report-file=output -o y.tab.c --defines=y.tab.h
cd ..
make
