#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include "app.h"
#include <stdexcept>
#include<vulkan/vulkan.hpp>
int main() {
    
    ppkin::app firstApp;
    
    try {
        firstApp.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    
    
    return EXIT_SUCCESS;
}