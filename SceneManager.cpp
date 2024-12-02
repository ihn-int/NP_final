#include "util.hpp"

// The constructor also initialize the game window
SceneManager::SceneManager(
	std::string game_title;
    uint32_t width, height;
    std::vector<Scene> scenes;
    Scene* current_scene;
) : game_title
	scenes(std::vector<Scene>(0)),
	current_scene(nullptr) {

	}

void SceneManager::Run() {
	if (current_scene == nullptr) {
		// Error: trying to run a null scene
		ErrorHandler(RUNNING_NULL_SCENE);
	}

	current_scene->Update();
}