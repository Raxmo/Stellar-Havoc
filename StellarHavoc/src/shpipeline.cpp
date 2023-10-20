#include "../hed/shpipeline.hpp"

namespace stellarhavoc
{
	shpipeline::shpipeline(
		shDevice& device,
		const std::string vertPath,
		const std::string fragPath,
		const pipelineconfiginfo& cfginfo)
		: device(device)
	{
		createPipeline(vertPath, fragPath, cfginfo);
	}
	pipelineconfiginfo shpipeline::createdefaultconfiginfo(uint32_t width, uint32_t height)
	{
		return pipelineconfiginfo();
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

	void shpipeline::createPipeline(
		const std::string vertpath,
		const std::string fragpath,
		const pipelineconfiginfo cfginfo)
	{
		auto vertcode = readfile(vertpath);
		auto fragcode = readfile(fragpath);

		std::cout << "vertex cote size: " << vertcode.size() << "\n";
		std::cout << "Fragment code size: " << fragcode.size() << "\n";
	}
	void shpipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shadermodule)
	{
		VkShaderModuleCreateInfo sminfo;
		sminfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		sminfo.codeSize = code.size();
		sminfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(device.device(), &sminfo, nullptr, shadermodule) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module");
		}
	}
}