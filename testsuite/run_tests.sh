#!/bin/bash

###########################################################################################################
#
# Build and run testsuite
#
###########################################################################################################

mkdir -p build-tests			# create the test dir if not exists
rm -rf build-tests/*			# delete all entries in test dir
cd build-tests					# enter test dir

# create the tests and run
cmake ..
make
ctest # -V

###########################################################################################################
#
# Coverage
#
###########################################################################################################


SCRIPT_DIR=$(dirname $0)

SRC_DIR="$SCRIPT_DIR/../../src/build-garlic-lib-Qt_5_15_13_Desktop_Qt-Debug" # the directory containing the .gcno files (%{buildDir} in Qt Creator)
COV_DIR="$SCRIPT_DIR/./coverage"				# stores the coverage results
HTML_RESULTS="${COV_DIR}""/html"   				# stores the HTML files


mkdir -p ${HTML_RESULTS}						# Create the coverage and html folder if they does not exists

# Generate our initial info
lcov --directory "${SRC_DIR}" --capture --output-file "${COV_DIR}/coverage.info" --rc lcov_branch_coverage=1

# Remove paths/files not required for code coverage (e.g. 3rd party libs)
lcov --ignore-errors unused,unused \
 --remove "${COV_DIR}/coverage.info" "*Qt*.framework*" "*.h" "*/tests/*" "*Xcode.app*" "*.moc" "*moc_*.cpp" "*/test/*" "*/build*/*" "/usr/include/*" "ext/*" \
 --output-file "${COV_DIR}/coverage-filtered.info"

genhtml -o "${HTML_RESULTS}" "${COV_DIR}/coverage-filtered.info"	# Generate the HTML files
lcov --directory "${COV_DIR}" --zerocounters    					# Reset our counts
firefox "${HTML_RESULTS}/index.html" 								# Open the index.html


exit 0

