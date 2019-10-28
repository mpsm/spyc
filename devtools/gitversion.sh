#!/bin/sh

# check arguments
if [ $# -ne 1 ]; then
    exit 1
fi
outfile="$1"

topdir="$(realpath $(dirname $0)/..))"
if [ ! -d "${topdir}/.git" ]; then
    echo "namespace spyc{const char *gitversion = nullptr;}" > "$outfile"
else
    gitrev="$(git rev-parse --short HEAD)"
    dirty="$(git diff --quiet || echo '-dirty')"
    echo "namespace spyc{const char *gitversion = \"${gitrev}${dirty}\";}" \
        > "${outfile}"
fi
