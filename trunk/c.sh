#!/usr/bin/env bash

phpize && ./configure && make && sudo make install  && phpize  --clean
