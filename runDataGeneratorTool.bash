#!/bin/bash
echo "Creating data folder..."
mkdir -p data
echo "Running Generator..."
$PIN_HOME/pin -t DataGenerator/obj-intel64/DataGenerator.so $@
