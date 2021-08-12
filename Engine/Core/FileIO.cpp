#include "FileIO.h"

namespace nc {
	int FileIO::LoadHighScore(const std::string& filename)
	{
		int highscore = -1;
		std::fstream my_file;
		my_file.open(filename, std::ios::in);
		
		if (my_file) {
			char ch;
			std::string s = "";

			while (1 == 1) {
				my_file >> ch;
				if (my_file.eof()) break;
				s += ch;
			}

			try {highscore = std::stoi(s);}
			catch (...) {std::cerr << "ERROR!\n";}
		}
		else {
			std::cout << "File not found" << std::endl;
		}
		my_file.close();
		return highscore;
	}

	void FileIO::WriteHighScore(const std::string& filename, int newHighscore) {
		std::fstream my_file;
		my_file.open(filename, std::ios::out);
		if (my_file) {
			my_file << newHighscore;
		}
		my_file.close();
	}
}