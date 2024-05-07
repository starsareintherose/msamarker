/*
 * Author: Guoyi Zhang
 * Date: 07 May 2024
 * License: GPLv2
 * Function: Generate fasta file which `m`
 * 	means selected position by Gblocks
 * 	from Gblocks html report file.
 * */

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

bool checkArguments(int argc, char* argv[]) {
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0]
			  << " input_file output_file\n";
		return false;
	}
	return true;
}

bool openFiles(std::ifstream& input, std::ofstream& output,
	       const std::string& inputFilename,
	       const std::string& outputFilename) {
	input.open(inputFilename);
	if (!input.is_open()) {
		std::cerr << "Unable to open input file: " << inputFilename
			  << std::endl;
		return false;
	}

	output.open(outputFilename);
	if (!output.is_open()) {
		std::cerr << "Unable to open output file: " << outputFilename
			  << std::endl;
		input.close();
		return false;
	}

	return true;
}

void closeFiles(std::ifstream& input, std::ofstream& output) {
	input.close();
	output.close();
}

void processFile(const std::string& inputFilename,
		 const std::string& outputFilename) {
	std::ifstream input;
	std::ofstream output;

	if (!openFiles(input, output, inputFilename, outputFilename)) {
		return;
	}

	output << ">marker" << std::endl;

	std::regex pattern("class=BL|^\\s{70}");
	std::string line;
	while (std::getline(input, line)) {
		if (std::regex_search(line, pattern)) {
			std::string processed_line = std::regex_replace(
			    line,
			    std::regex("</span>|<span class=BL>|^\\s{17}"), "");
			std::replace(processed_line.begin(),
				     processed_line.end(), ' ', '-');
			std::replace(processed_line.begin(),
				     processed_line.end(), '#', 'm');
			output << processed_line << std::endl;
		}
	}

	closeFiles(input, output);
}

int main(int argc, char* argv[]) {
	if (!checkArguments(argc, argv)) {
		return 1;
	}

	processFile(argv[1], argv[2]);

	return 0;
}

