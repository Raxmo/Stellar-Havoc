#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace stellarhavoc
{

	class shwindow
	{
	public:
		shwindow(int w, int h, std::string name);
		~shwindow();

		shwindow(cinst shwindow&) = delete;
		shwindow& operator=(const shwindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); };
	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowname;

		GLFWwindow *window;

	};
}