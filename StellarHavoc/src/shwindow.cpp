#include "../hed/shwindow.hpp"

namespace stellarhavoc
{
	shwindow::shwindow(int w, int h, std::string name) : width(w), height(h), windowname(name)
	{
		initWindow();
	}

	shwindow::~shwindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void shwindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


		window = glfwCreateWindow(width, height, windowname.c_str(), nullptr, nullptr);
	}

	void shwindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface");
		}
	}
}