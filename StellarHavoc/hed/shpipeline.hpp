#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "shdevice.hpp"

namespace stellarhavoc
{
	struct pipelineconfiginfo
	{

	};

	class shpipeline
	{
	public:
		shpipeline(
			shDevice &device, 
			const std::string vertPath, 
			const std::string fragPath, 
			const pipelineconfiginfo &cfginfo);
		~shpipeline() {}

		shpipeline(const shpipeline&) = delete;
		void operator=(const shpipeline&) = delete;

		static pipelineconfiginfo createdefaultconfiginfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> readfile(const std::string& path);

		void createPipeline(
			const std::string vertpath, 
			const std::string fragpath,
			const pipelineconfiginfo cfginfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shadermodule);

		shDevice& device;
		VkPipeline graphicsPipeline;
		VkShaderModule vertmodule;
		VkShaderModule fragmodule;

	};
}