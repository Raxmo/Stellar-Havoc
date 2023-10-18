#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

namespace stellarhavoc
{
	class shpipeline
	{
	public:
		shpipeline(const std::string vertPath, const std::string fragPath);

	private:
		static std::vector<char> readfile(const std::string& path);

		void createPipeline(const std::string vertpath, const std::string fragpath);
	};
}