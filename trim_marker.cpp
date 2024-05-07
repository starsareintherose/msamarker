/*
 * Author: Guoyi Zhang
 * Date: 07 May 2024
 * License: GPLv2
 * Function: Trim the multi-sequence-
 * 	alignment based on the marker
 * 	which `m` indicates selected
 * 	position.
 * */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Function: check the arg
void check_argument_count(int argc, char* argv[]) {
	if (argc != 4) {
		std::cerr
		    << "Usage: " << argv[0]
		    << " <marker_fasta_file> <input_fasta_file> <output_file>"
		    << std::endl;
		exit(1);
	}
}

// Function: check the file
void check_file_exists(const std::string& file) {
	std::ifstream infile(file);
	if (!infile.good()) {
		std::cerr << "Error: file " << file << " doesn't exsit"
			  << std::endl;
		exit(1);
	}
}

// Function: get the positions in marker file
std::vector<int> read_positions(const std::string& file) {
	std::ifstream infile(file);
	std::string line;
	std::vector<int> positions;
	int index = 1;
	while (std::getline(infile, line)) {
		if (line[0] != '>') {
			for (char c : line) {
				if (c == 'm' || c == 'M') {
					positions.push_back(index);
				}
				index++;
			}
		}
	}
	return positions;
}

// Function: trimming the sequences in input_fasta_file
void extract_characters(const std::string& line, const std::string& file,
			const std::vector<int> positions) {
	std::ofstream outfile(file, std::ios::app);
	std::string new_line;
	for (unsigned long pos : positions) {
		if (pos <= line.length()) {
			new_line += line[pos - 1];  // start from 0
		}
	}
	for (size_t i = 0; i < new_line.length(); i += 60) {
		int num_chars =
		    std::min(static_cast<int>(new_line.length() - i), 60);
		outfile << new_line.substr(i, num_chars) << std::endl;
	}
	outfile.close();
}

// Function: parse the fasta multi-lines format
void process_fasta_file(const std::string& fasta_file,
			const std::string& output_file,
			const std::vector<int> positions) {
	std::ifstream infile(fasta_file);
	std::string line;
	std::string prev_line = "";
	bool prev_line_is_header = false;
	while (std::getline(infile, line)) {
		if (line[0] == '>') {
			if (!prev_line_is_header) {
				extract_characters(prev_line, output_file,
						   positions);
			}
			std::ofstream outfile(output_file, std::ios::app);
			outfile << line << std::endl;
			outfile.close();
			prev_line_is_header = true;
		} else {
			if (prev_line_is_header) {
				prev_line = "";
				prev_line_is_header = false;
			}
			prev_line += line;
		}
	}
	extract_characters(prev_line, output_file, positions);
}

int main(int argc, char* argv[]) {
	check_argument_count(argc, argv);

	for (int i = 1; i <= 2; i++) {
		check_file_exists(argv[i]);
	}

	std::vector<int> positions = read_positions(argv[1]);

	process_fasta_file(argv[2], argv[3], positions);

	return 0;
}
