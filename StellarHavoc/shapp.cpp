#include "../hed/shapp.hpp"

namespace stellarhavoc
{
	void shapp::run()
	{
		while (!shwindow.shouldClose())
		{
			glfwPollEvents();
		}
	}
}