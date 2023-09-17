#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
namespace ppkin {
	class ppWindow {
	public:
		ppWindow(int w, int h, std::string name);
		~ppWindow();

		ppWindow(const ppWindow&) = delete;
		ppWindow& operator = (const ppWindow&) = delete;

		GLFWwindow* getWindow();
		bool shouldClose() { return glfwWindowShouldClose(window); }
	private:			
		void initWindow();
		const int width;
		const int height;
		std::string windowName;
		GLFWwindow* window;

	};



}