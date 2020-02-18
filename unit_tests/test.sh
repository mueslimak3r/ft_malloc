#!/bin/bash

red='\033[1;31m'
NC='\033[0m'
green='\033[01;32m'
yellow='\033[0;33m'

dir="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
parentdir="$(dirname "$dir")"

gcc "${dir}/test0.c" -o "${dir}/test0" &>/dev/null
gcc "${dir}/test1.c" -o "${dir}/test1" &>/dev/null
gcc "${dir}/test2.c" -o "${dir}/test2" &>/dev/null
gcc "${dir}/test3.c" -o "${dir}/test3" &>/dev/null
gcc "${dir}/test3b.c" -o "${dir}/test3b" &>/dev/null
gcc -L"${parentdir}" -lft_malloc "${dir}/test5.c" -o "${dir}/test5" &>/dev/null


###################################################################
##Functions export
echo -e "${red}Functions export:${NC}"

nm "${parentdir}/libft_malloc.so" | grep -- '_free\|_malloc\|_realloc\|_show_alloc_mem'
echo


###################################################################
##Feature's testing
echo -e "${red}Feature's testing:${green}"
BASELINE=`/usr/bin/time -l "${dir}/test0" 2>&1 | grep "page reclaims" | rev | cut -c 15- | rev | awk '{$1=$1};1'`
echo -e "${green}baseline:${NC}${BASELINE}"

W_MALLOC_T0=`"${dir}/lib_preload.sh" /usr/bin/time -l "${dir}/test1" 2>&1 | grep "page reclaims" | rev | cut -c 15- | rev | awk '{$1=$1};1'`
WO_MALLOC_T0=`"${dir}/lib_preload.sh" /usr/bin/time -l "${dir}/test0" 2>&1 | grep "page reclaims" | rev | cut -c 15- | rev | awk '{$1=$1};1'`
echo -e "${green}test0 pages:${NC}${WO_MALLOC_T0}"
echo -e "${green}test1 pages:${NC}${W_MALLOC_T0}"
diff=$(expr $W_MALLOC_T0 - $WO_MALLOC_T0)

color='\033[0m'
if (($diff < 255 )) || (($diff > 1022 )) ; then
	color=$red
elif (($diff > 272 )) && (($diff < 1023 )) ; then
	color=$yellow
elif (($diff > 254 )) && (($diff < 273 )) ; then
	color=$green
fi
echo -e "diff:${color}${diff}${NC}"
echo


###################################################################
##Tests of free
echo -e "${red}Tests of free:${NC}"

FREE_T0=`"${dir}/lib_preload.sh" /usr/bin/time -l "${dir}/test2" 2>&1 | grep "page reclaims" | rev | cut -c 15- | rev | awk '{$1=$1};1'`
diff_w=$(expr $W_MALLOC_T0 - $FREE_T0)
diff_wo=$(expr $FREE_T0 - $WO_MALLOC_T0)

color='\033[0m'
if (($diff_w >= W_MALLOC_T0 )) ; then
	color=$red
elif (($diff_wo > 3 )) ; then
	color=$yellow
elif (($diff_wo < 4 )) ; then
	color=$green
fi
echo -e "diff between no allocs and after freeing:${color}${diff_wo}${NC}"
echo -e "diff between allocs and after freeing:${color}${diff_w}${NC}"
echo


###################################################################
##Tests of Realloc
echo -e "${red}Tests of Realloc:${NC}"
REALLOC_T0=`"${dir}/lib_preload.sh" "${dir}/test3"`
REALLOC_T1=`"${dir}/lib_preload.sh" "${dir}/test3b"`

color='\033[0m'
if [[ $REALLOC_T0 == $'Bonjour\nBonjour' ]] ; then
	color=$green
else
	color=$red
fi
echo -e "${color}test0${NC}:${color}${REALLOC_T0}${NC}"

color='\033[0m'
if [[ $REALLOC_T1 == $'Bonjour' ]] ; then
	color=$green
else
	color=$red
fi
echo -e "${color}test1${NC}:${color}${REALLOC_T1}${NC}"
echo


###################################################################
##Error Tests
echo -e "${red}Error Tests:${NC}"
ERROR_T0=`"${dir}/lib_preload.sh" "${dir}/test3b"`

color='\033[0m'
if [[ $ERROR_T0 == $'Bonjour' ]] ; then
	color=$green
else
	color=$red
fi
echo -e "${color}test0${NC}:${color}${ERROR_T0}${NC}"
echo


###################################################################
##Tests of show_alloc_mem
echo -e "${red}Tests of show_alloc_mem:${NC}"
"${dir}/lib_preload.sh" "${dir}/test5" > "${dir}/test5_log"

echo -e "${NC}check ${green}test5_log${NC} for output"


###################################################################
rm "${dir}/test0" "${dir}/test1" "${dir}/test2" "${dir}/test3" "${dir}/test3b" "${dir}/test5"
