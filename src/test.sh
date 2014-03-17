#!/usr/bin/env bash

if [ ! -e "fig-command-info.h" ]; then
	echo "Not in src dir, will not run."
	exit 1
fi

rm -rf test-dir

./fig -d test-dir init

