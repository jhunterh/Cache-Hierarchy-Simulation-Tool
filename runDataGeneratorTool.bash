#!/bin/bash
echo "Creating data folder..."
rm -rf data
mkdir -p data
echo "Running Generator..."
$PIN_HOME/pin -t $CHS_HOME/DataGenerator/obj-intel64/DataGenerator.so $@
