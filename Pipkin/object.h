#pragma once
#include"model.h"
#include <glm/gtc/matrix_transform.hpp>
namespace ppkin {
	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f,1.f,1.f };
		glm::vec3 roatation{};
		glm::mat4 mat4() {
			auto transform = glm::translate(glm::mat4{ 1.f }, translation);
			transform = glm::scale(transform, scale);
			transform = glm::rotate(transform, roatation.y,{0,1.f,0});
			transform = glm::rotate(transform, roatation.x, { 1.f,0,0 });
			transform = glm::rotate(transform, roatation.z, { 0,0,1.f });
			return transform;
		}
	};

	class Object {
	public:
		using UniqueId = unsigned int;
		static Object createObject() {
			static UniqueId currentId = 0;
			return Object(currentId++);
		}
		
		 UniqueId getId() { return id; }
		/* void setObjectModel(model* ObjectModel) {
			 this->ObjectModel = ObjectModel;
		 }
		 model& getObjectModel() {
			 return *ObjectModel;
		 }*/
		 std::shared_ptr<model> ObjectModel;
		 TransformComponent transform;
	private:

		Object(UniqueId id) {
			this->id = id;
		}
		

		UniqueId id;
	};

}