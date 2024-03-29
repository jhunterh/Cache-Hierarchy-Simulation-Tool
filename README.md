# Cache-Hierarchy-Simulation-Tool
This tool is a highly configurable cache hierarchy simulator for use in computer architecture research and study.

## To Build the Data Generator PIN Tool
In order to build the Data Generator PIN Tool, type "source tool_env.bash" to get the proper environment variables.
Next, type "make all" to build the tool.

To run the tool, type "./runDataGeneratorTool.bash <your_executable_path>"
NOTE: It might be necessary to update the permissions on "runDataGeneratorTool.bash" by typing "chmod +x runDataGeneratorTool.bash"

The Data Generator tool will produce a file called "Dataset.zip". This file contains a folder called "data" that contains all of the generated dataset files.

## Using a Dataset
To run the simulator with a dataset, unzip the dataset you want to use -> "unzip Dataset.zip"
The simulator will look for the "data" folder and automatically parse all of the data files.
If you would like to combine datasets into a single larger dataset, simply move all of the 
data files into a single "data" directory.