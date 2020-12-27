#!/bin/bash
set -e
rm -Rf .env || true
virtualenv -p python3.8 .env
. .env/bin/activate
pip install cpplint
