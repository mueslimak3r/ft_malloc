#!/bin/bash

dir="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
parentdir="$(dirname "$dir")"

if [ "$(uname)" == "Darwin" ]; then
	export DYLD_LIBRARY_PATH="$parentdir"
	export DYLD_INSERT_LIBRARIES="$parentdir/libft_malloc.so"
	export DYLD_FORCE_FLAT_NAMESPACE=1

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
	export LD_LIBRARY_PATH="$parentdir"
	export LD_PRELOAD="$parentdir/libft_malloc.so"
fi
$@
