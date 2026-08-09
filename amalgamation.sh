#!/bin/bash
set -euo pipefail

rm -f amalgamation

# install with: pip install quom
quom --source_directory src --include_directory include src/main.cpp amalgamation

sed -i '1i #define SERIAL_BAUD 115200' amalgamation
