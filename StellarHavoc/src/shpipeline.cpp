#include "../hed/shpipeline.hpp"

namespace stellarhavoc
{
	shpipeline::shpipeline(const std::string vertPath, const std::string fragPath)
	{
		createPipeline(vertPath, fragPath);
	}
	std::vector<char> shpipeline::readfile(const std::string& path)
	{
		std::ifstream file(path, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("Failed to open file" + path);
		}

		size_t filesize = static_cast<size_t>(file.tellg());

		std::vector<char> buffer(filesize);

		file.seekg(0);
		file.read(buffer.data(), filesize);
		
		file.close();
		return buffer;
	}

	void shpipeline::createPipeline(const std::string vertpath, const std::string fragpath)
	{
		auto vertcode = readfile(vertpath);
		auto fragcode = readfile(fragpath);

		std::cout << "vertex cote size: " << vertcode.size() << "\n";
		std::cout << "Fragment code size: " << fragcode.size() << "\n";
	}
}