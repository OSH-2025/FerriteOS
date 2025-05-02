#!/bin/bash

if [ -e "$1/json_config.h.in" ]; then
    cp $1/json_config.h.in $1/json_config.h
fi

if [ -e "$1/config.h.in" ]; then
    cp $1/config.h.in $1/config.h
fi