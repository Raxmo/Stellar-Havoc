#include "../hed/shpipeline.hpp"

namespace stellarhavoc
{
	shpipeline::shpipeline(
		shdevice& device,
		const std::string vertPath,
		const std::string fragPath,
		const pipelineconfiginfo& cfginfo)
		: device(device)
	{
		createPipeline(vertPath, fragPath, cfginfo);
	}

	shpipeline::~shpipeline()
	{
		vkDestroyShaderModule(device.device(), vertmodule, nullptr);
		vkDestroyShaderModule(device.device(), fragmodule, nullptr);
		vkDestroyPipeline(device.device(), graphicsPipeline, nullptr);
	}

	pipelineconfiginfo shpipeline::createdefaultconfiginfo(uint32_t width, uint32_t height)
	{
		pipelineconfiginfo configinfo{};

		configinfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configinfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configinfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		configinfo.viewport.x = 0.0f;
		configinfo.viewport.y = 0.0f;
		configinfo.viewport.width = static_cast<float>(width);
		configinfo.viewport.height = static_cast<float>(height);
		configinfo.viewport.minDepth = 0.0f;
		configinfo.viewport.maxDepth = 1.0f;

		configinfo.scissor.offset = { 0, 0 };
		configinfo.scissor.extent = { width, height };

		configinfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configinfo.viewportInfo.viewportCount = 1;
		configinfo.viewportInfo.pViewports = &configinfo.viewport;
		configinfo.viewportInfo.scissorCount = 1;
		configinfo.viewportInfo.pScissors = &configinfo.scissor;

		configinfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configinfo.rasterizationInfo.depthClampEnable = VK_FALSE;
		configinfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configinfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configinfo.rasterizationInfo.lineWidth = 1.0f;
		configinfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		configinfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		configinfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
		configinfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
		configinfo.rasterizationInfo.depthBiasClamp = 0.0f;
		configinfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

		configinfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configinfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
		configinfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configinfo.multisampleInfo.minSampleShading = 1.0f;
		configinfo.multisampleInfo.pSampleMask = nullptr;
		configinfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		configinfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

		configinfo.colorBlendAttatchment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		configinfo.colorBlendAttatchment.blendEnable = VK_FALSE;
		configinfo.colorBlendAttatchment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		configinfo.colorBlendAttatchment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		configinfo.colorBlendAttatchment.alphaBlendOp = VK_BLEND_OP_ADD;

		configinfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configinfo.colorBlendInfo.logicOpEnable = VK_FALSE;
		configinfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		configinfo.colorBlendInfo.attachmentCount = 1;
		configinfo.colorBlendInfo.pAttachments = &configinfo.colorBlendAttatchment;
		configinfo.colorBlendInfo.blendConstants[0] = 0.0f;
		configinfo.colorBlendInfo.blendConstants[1] = 0.0f;
		configinfo.colorBlendInfo.blendConstants[2] = 0.0f;
		configinfo.colorBlendInfo.blendConstants[3] = 0.0f;

		configinfo.depthStencileInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configinfo.depthStencileInfo.depthTestEnable = VK_TRUE;
		configinfo.depthStencileInfo.depthWriteEnable = VK_TRUE;
		configinfo.depthStencileInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configinfo.depthStencileInfo.depthBoundsTestEnable = VK_FALSE;
		configinfo.depthStencileInfo.minDepthBounds = 0.0f;
		configinfo.depthStencileInfo.maxDepthBounds = 1.0f;
		configinfo.depthStencileInfo.stencilTestEnable = VK_FALSE;
		configinfo.depthStencileInfo.front = {};
		configinfo.depthStencileInfo.back = {};

		return configinfo;
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
		assert(cfginfo.pipelineLayout != VK_NULL_HANDLE && 
			"Cannot create graphics pipeline, no pipelineLayout provided in cfginfo!");
		assert(cfginfo.renderPass != VK_NULL_HANDLE &&
			"Cannot create graphics pipeline, no renderPass provided in cfginfo!");

		auto vertcode = readfile(vertpath);
		auto fragcode = readfile(fragpath);

		createShaderModule(vertcode, &vertmodule);
		createShaderModule(fragcode, &fragmodule);

		VkPipelineShaderStageCreateInfo shaderStages[2];

		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = vertmodule;
		shaderStages[0].pName = "main";
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = fragmodule;
		shaderStages[1].pName = "main";
		shaderStages[1].flags = 0;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo;
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &cfginfo.inputAssemblyInfo;
		pipelineInfo.pViewportState = &cfginfo.viewportInfo;
		pipelineInfo.pRasterizationState = &cfginfo.rasterizationInfo;
		pipelineInfo.pMultisampleState = &cfginfo.multisampleInfo;
		pipelineInfo.pDepthStencilState = &cfginfo.depthStencileInfo;
		pipelineInfo.pDynamicState = nullptr;

		pipelineInfo.layout = cfginfo.pipelineLayout;
		pipelineInfo.renderPass = cfginfo.renderPass;
		pipelineInfo.subpass = cfginfo.subpass;

		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphics pipeline.");
		}

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