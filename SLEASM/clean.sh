#!/bin/bash

find . -type f \( -name "*.lst" -o -name "*.v20raw" \) -exec rm -f {} \;
