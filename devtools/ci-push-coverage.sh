#!/bin/sh

if [ -z "${CODECOV_TOKEN}" ]; then
    echo "Setup Codecov token"
    exit 1
fi

if [ -z "${CIRCLE_BUILD-NUM}" ]; then
    echo "Can't determine build version - run outside CircleCI?"
    exit 2
fi

set -x
gcovr --exclude test --exclude-throw-branches --exclude-unreachable-branches \
    -x -o coverage.xml
curl -s https://codecov.io/bash \
    | bash -s -- -f coverage.xml -t "${CODECOV_TOKEN}" \
    -n "${CIRCLE_BUILD_NUM}" -Z \
    || echo \"Codecov upload failed\"
