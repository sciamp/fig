#!/usr/bin/env bash

if [ ! -e "fig-command-info.h" ]; then
    echo "Not in src dir, will not run."
    exit 1
fi

TEST_DIR=test-dir

if [ -d "${TEST_DIR}" ]; then
    rm -rf "${TEST_DIR}"
fi

./fig -d "${TEST_DIR}" init
./fig -d "${TEST_DIR}" add-target --library foobar-1.0
./fig -d "${TEST_DIR}" add-target --program baz

