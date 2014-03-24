#!/usr/bin/env bash

if [ ! -e "fig-command-info.h" ]; then
    echo "Not in src dir, will not run."
    exit 1
fi

FIG=${PWD}/fig
TEST_DIR=test-dir

if [ -d "${TEST_DIR}" ]; then
    rm -rf "${TEST_DIR}"
fi

mkdir ${TEST_DIR}
cd ${TEST_DIR}

git init

$FIG  init --name foobar --license agpl-3.0 --version 2.0.0
git add .
git commit -a -m 'initial commit'

$FIG add-target --library foobar-1.0 --directory foobar
$FIG add-target --program baz --directory baz

$FIG update-authors
git add .
git commit -a -m 'update authors'

./autogen.sh
make

cd -
