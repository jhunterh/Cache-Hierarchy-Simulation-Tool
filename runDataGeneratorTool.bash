#!/bin/bash

source tool_env.bash
mkdir -p data
$PIN_HOME/pin -t DataGenerator/obj-intel64/DataGenerator.so -- $@
