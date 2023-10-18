#pragma once

#include "shwindow.hpp"

namespace stellarhavoc
{
	class shapp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	private:
		shwindow shwindow{WIDTH, HEIGHT, "Hello Vulkan!"};
	};
}