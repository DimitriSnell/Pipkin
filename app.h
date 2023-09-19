#pragma once
#include"renderer.hpp"
#include"Camera.h"
#include"pp_window.hpp"
#include"logical_device.hpp"
#include"object.h"
#include<chrono>
#include"Keyboard_movement.h"
#include"glm/gtx/string_cast.hpp"
#include"glm/gtc/constants.hpp"
namespace ppkin {
	class app {
	public:
		void run();
		app();
		~app();
		static constexpr int WIDTH = 1500;
		static constexpr int HEIGHT = 1200;
	private:
		void make_assets();
		Renderer* renderer;
		Camera camera{};
		ppWindow ppwin{ WIDTH, HEIGHT, "pipkin" };
		logicDevice* devices;
		std::vector<Object> gameObjects;
	};
}