#pragma once
#include <iostream>
#include <fstream>
#include <Vector>
#include <String>


namespace nc {
	class FileIO 
	{
	public:
		int static LoadHighScore(const std::string& filename);
		void static WriteHighScore(const std::string& filename, int newHighscore);
	};
}
