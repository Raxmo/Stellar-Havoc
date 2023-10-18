#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>

namespace stellarhavoc
{

	class shwindow
	{
	public:
		shwindow(int w, int h, std::string name);
		~shwindow();

		shwindow(const shwindow&) = delete;
		shwindow& operator=(const shwindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); };

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowname;

		GLFWwindow *window;

	};
}