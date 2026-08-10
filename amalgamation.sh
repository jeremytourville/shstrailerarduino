#!/bin/bash
set -euo pipefail

rm -f amalgamation

# install with: pip install quom
quom --source_directory src --include_directory include src/main.cpp amalgamation

sed -i '1i #define SERIAL_BAUD 115200' amalgamation

# amalgamation code file can be loaded into this project:
# https://app.cirkitdesigner.com/project/2e63a0aa-5d21-4710-8b59-05566160b19c
