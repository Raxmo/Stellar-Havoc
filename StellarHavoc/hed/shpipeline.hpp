#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cassert>

#include "shdevice.hpp"

namespace stellarhavoc
{
	struct pipelineconfiginfo
	{
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttatchment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencileInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class shpipeline
	{
	public:
		shpipeline(
			shdevice &device, 
			const std::string vertPath, 
			const std::string fragPath, 
			const pipelineconfiginfo &cfginfo);
		~shpipeline();

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

		shdevice& device;
		VkPipeline graphicsPipeline;
		VkShaderModule vertmodule;
		VkShaderModule fragmodule;

	};
}