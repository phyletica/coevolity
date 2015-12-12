#!/bin/sh

usage () {
    echo ""
    echo "usage: $0 [-h|--help] [-p|--prefix <INSTALL-PREFIX>] [-s|--static] [-i|--install] [-t|--test]"
    echo "  -h|--help       show help message and exit."
    echo "  -s|--static     build statically linked binaries."
    echo "  -t|--test       run the test suite after building."
    echo "  -i|--install    install the executables."
    echo "  -p|--prefix     install path. Default: '/usr/local'."
    echo ""
}

build_coevolity () {
    cmake_args=$@
    if [ -n "$COEVOLITY_BASE_DIR" ]
    then
        base_dir="$COEVOLITY_BASE_DIR"
    else
        echo "ERROR: COEVOLITY_BASE_DIR was not defined prior to calling "
        echo "build_coevolity."
        exit 1
    fi
    if [ -n "$COEVOLITY_BUILD_DIR" ]
    then
        build_dir="$COEVOLITY_BUILD_DIR"
    else
        build_dir="${base_dir}/build"
    fi
    if [ -d "$build_dir" ]
    then
        echo "ERROR: build directory '$build_dir' already exists."
        echo "To reconfigure, please remove this directory and re-run this script."
        exit 1
    else
        mkdir -p "$build_dir"
    fi
    
    # configure make files and build
    cd "$build_dir"
    echo "Configuring make files..."
    echo "${base_dir}/ $cmake_args" | xargs cmake || exit 1
    echo "Building..."
    make || exit 1
    if [ -n "$COEVOLITY_RUN_TESTS" ]
    then
        echo "Building and running test suite..."
        make check || exit 1
    fi
    if [ -n "$COEVOLITY_RUN_INSTALL" ]
    then
        echo "Installing..."
        make install || exit 1
    fi
    cd "$base_dir"
    echo "Done!"
}


# get location of script
COEVOLITY_BASE_DIR=""
this_dir=`dirname "$0"`
if [ "$this_dir" = "." ]
then
    export COEVOLITY_BASE_DIR="$(pwd)"
else
    cd "$this_dir"
    export COEVOLITY_BASE_DIR="$(pwd)"
fi
export COEVOLITY_BUILD_DIR="${COEVOLITY_BASE_DIR}/build"

# make sure submodules are here and up to date
# git submodule init
# git submodule update

# process args
extra_args=""
static=""
install_prefix=""
COEVOLITY_RUN_TESTS=""
COEVOLITY_RUN_INSTALL=""
universal_mac_build=""
linux_dist_build=""
mbit=""
while [ "$1" != "" ]
do
    case $1 in
        -h| --help)
            usage
            exit
            ;;
        -p| --prefix)
            shift
            install_prefix=$1
            ;;
        -s| --static)
            static=1
            ;;
        -t| --test)
            COEVOLITY_RUN_TESTS=1
            export COEVOLITY_RUN_TESTS
            ;;
        -i| --install)
            COEVOLITY_RUN_INSTALL=1
            export COEVOLITY_RUN_INSTALL
            ;;
        -m)
            shift
            mbit=$1
            ;;
        --universal-mac-build)
            universal_mac_build=1
            static=1
            ;;
        --linux-dist-build)
            linux_dist_build=1
            static=1
            ;;
        * )
            extra_args="$extra_args $1"
    esac
    shift
done
args=""
if [ -n "$install_prefix" ]
then
    args="${args} -DCMAKE_INSTALL_PREFIX=${install_prefix}"
fi
if [ -n "$static" ]
then
    args="${args} -DSTATIC_LINKING=YES"
fi
if [ -n "$extra_args" ]
then
    args="${args} ${extra_args}"
fi
if [ -n "$linux_dist_build" ] && [ -n "$mbit" ]
then
    args="${args} -DCMAKE_CXX_FLAGS=-m${mbit} -DCMAKE_LD_FLAGS=-m${mbit}"
fi
if [ -n "$universal_mac_build" ]
then
    args="${args} -DCMAKE_CXX_FLAGS=\"-arch ppc -arch i386 -arch x86_64\""
fi

# check for build directory
build_coevolity $args
