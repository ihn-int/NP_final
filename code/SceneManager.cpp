#include "util.hpp"

// The constructor also initialize the game window
SceneManager::SceneManager(
	std::string game_title,
    uint32_t width,
    uint32_t height
) : scenes(std::vector<Scene>(0)),
	// canva(Canva()),
    current_scene(nullptr),
    game_title(game_title),
    width(width),
    height(height)
    {
        window = sf::RenderWindow(
            sf::VideoMode(width, height),
            game_title,
            sf::Style::Titlebar | sf::Style::Close
        );
	}

void SceneManager::run() {
	if (current_scene == nullptr) {
		// Error: trying to run a null scene
		ErrorHandler::PrintError(RUNNING_NULL_SCENE);
	}
	current_scene->Update();
}

void SceneManager::changeScene(std::string tag) {
    for (auto scene_p : scenes) {
        if (scene_p->getTag() == tag) {
            current_scene = scene_p;
        }
    }
    // If no tag matched, report an Error
    ErrorHandler::PrintError(CHANGE_TO_NULL_SCENE);
}