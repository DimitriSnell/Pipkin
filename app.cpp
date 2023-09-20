#pragma once
#include "app.h"
#include"Keyboard_movement.h"
namespace ppkin {
	void app::run()
	{
		renderer->initVulkan();
		devices = &renderer->devices;
		make_assets();
		auto currentTime = std::chrono::high_resolution_clock::now();
		glm::vec3 eye = { 3.5f, 1.0f, 2.f };
		glm::vec3 center = { 0.0f, 0.0f,  0.0f };
		glm::vec3 up = { 0.0f,-1.0f,0.0f };
		camera.setViewDirection(eye, center, up);
		KeyboardMovementController cameraController{};
		auto viewerObject = Object::createObject();
		while (!ppwin.shouldClose()) {
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();

			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;
			cameraController.moveInPlaneXZ(ppwin.getWindow(), frameTime, viewerObject);
			std::cout << glm::to_string(viewerObject.transform.translation) << std::endl;
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.roatation);
			renderer->drawFrame(gameObjects);
		}
		vkDeviceWaitIdle(devices->getLogicalDevice());
		
	}
	app::app()
	{
		renderer = new Renderer();
		renderer->setCamera(&camera);
		renderer->ppwin = &ppwin;
		
	}
	app::~app()
	{
		for (auto& o : gameObjects) {
			o.ObjectModel.get()->destroyModel(devices->getLogicalDevice());
		}
		renderer->cleanup();
		delete renderer;
	}

	void app::make_assets()
	{
		model::Builder builder{};
		builder.vertices = {

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

		};
		//auto m = new model(&devices, verticies);
		//auto m = std::make_shared<model>(devices, builder);
		std::shared_ptr<model> m = model::createModelFromFile(*devices, "C:/Users/meach/source/repos/Pipkin/Pipkin/models/nier-automata-3d-print-figure/source/SkecthFab2/SkecthFab2.obj");
		Object o = Object::createObject();
		o.ObjectModel = m;

		//o.transform.translation = { .0f,.0f,.0f };
		o.transform.translation = { 0.0f,0.0f, 0.f };
		o.transform.scale = { .5f,.5f,.5f };
		gameObjects.push_back(std::move(o));

	}
	
}



