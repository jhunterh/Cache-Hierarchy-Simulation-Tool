#!/bin/bash

usage() {
    echo "Usage: $0 [-o output_file]"
    echo "  -o : Specify the name of the zipped dataset"
    echo ""
    echo "Example: ./zipDataset.bash -o MyDataset"
    exit 1
}

SET_NAME="Dataset"

while getopts o: flag
do
    case "${flag}" in
        o) SET_NAME=${OPTARG};;
        *) usage;;
    esac
done

if [ -d "data" ]; then
    echo "Zipping Dataset: ${SET_NAME}.zip"
    zip -r ${SET_NAME}.zip data
else
    echo "Unable to locate data directory! Cancelling..."
    exit 1
fi