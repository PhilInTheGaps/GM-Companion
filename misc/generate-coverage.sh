#!/bin/bash

dir=$1
filename="coverage.info"
filename_filtered="coverage_filtered.info"

if [ -z "$dir" ]; then echo "Please specify a directory!" && exit 1; fi

fastcov --include src/ --lcov -o $dir/$filename
lcov --remove $dir/$filename -o $dir/$filename_filtered '/usr/*' '*/**/tests/*' '*/**/thirdparty/*' '*/**/*_autogen/*'

genhtml -o $dir/coverage_html $dir/$filename_filtered
