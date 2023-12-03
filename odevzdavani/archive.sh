#!/bin/bash

cd "$(dirname "$0")/../"
zip -r odevzdavani/xhobza03.zip *.c *.h Makefile tests documentation ifjcompiler README.md
