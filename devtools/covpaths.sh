#!/bin/sh

if [ $# -ne 1 ]; then
    exit 1
fi

buildpath=$(realpath $1)
if [ ! -d ${buildpath} ]; then
    exit 2
fi

outpath="${buildpath}-coverage"
if [ -d ${outpath} ]; then
    rm -rf ${outpath}
fi

mkdir -p ${outpath}
cd ${outpath} \
    && find ${buildpath} -name "*.gcno" -o -name "*.gcda" \
    | sed 's,.*\/\(.*\),\1 &,;s,\.cc,,' \
    | awk '{print "cp "$2" "$1}' \
    | xargs -I{} sh -c "{}"