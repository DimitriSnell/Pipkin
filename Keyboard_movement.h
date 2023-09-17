#pragma once
#include"config.h"
#include"object.h"
#include"pp_window.hpp"
struct KeyMappings {
	int moveLeft = GLFW_KEY_A;
	int moveRight = GLFW_KEY_D;
	int moveUp = GLFW_KEY_X;
	int moveDown = GLFW_KEY_Z;
	int moveForward = GLFW_KEY_W;
	int moveBackward = GLFW_KEY_S;
	int lookLeft = GLFW_KEY_LEFT;
	int lookRight = GLFW_KEY_RIGHT;
	int lookup = GLFW_KEY_UP;
	int lookDown = GLFW_KEY_DOWN;
};
namespace ppkin {
	class KeyboardMovementController {
	public:

		void moveInPlaneXZ(GLFWwindow* window, float dt, Object& gameObject );

	private:
		KeyMappings keys{};
		float moveSpeed{ 3.f };
		float lookSpeed{ 1.5f };
	};
	

	
}