#!/usr/bin/env bash
 
# get location of script
ECOEVOLITY_BASE_DIR=""
this_dir=`dirname "$0"`
if [ "$this_dir" = "." ]
then
    ECOEVOLITY_BASE_DIR="$(pwd)"
else
    cd "$this_dir"
    ECOEVOLITY_BASE_DIR="$(pwd)"
fi

TEST_BUILD_DIR="${ECOEVOLITY_BASE_DIR}/test-suite-build"

if [ -d "$TEST_BUILD_DIR" ]
then 
    echo "ERROR: build directory '$TEST_BUILD_DIR' already exists."
    echo "To recompile, please remove this directory and re-run this script."
    exit 1
else
    mkdir -p "${TEST_BUILD_DIR}"
fi

# number of cpus to use during compile
COMPILETHREADS=4
 
cd "${TEST_BUILD_DIR}"
cmake -DCMAKE_BUILD_TYPE=Debug ../
make clean
make -j $COMPILETHREADS check

cd test
./test_ecoevolity

cd "$ECOEVOLITY_BASE_DIR"
