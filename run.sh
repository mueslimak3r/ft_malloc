#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
	export DYLD_LIBRARY_PATH=`pwd`
	export DYLD_INSERT_LIBRARIES=libft_malloc.so
	export DYLD_FORCE_FLAT_NAMESPACE=1

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
	export LD_LIBRARY_PATH=`pwd`
	export LD_PRELOAD=`pwd`/libft_malloc.so
fi
$@
