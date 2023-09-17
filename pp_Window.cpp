#pragma once
#include "pp_window.hpp"

namespace ppkin {
	ppWindow::ppWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
		initWindow();
	}
	ppWindow::~ppWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	void ppWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	
	}
	GLFWwindow* ppWindow::getWindow() {
		return window;
	}
}