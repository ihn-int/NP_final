#include "util.hpp"

// The constructor also initialize the game window
SceneManager::SceneManager(
	std::string game_title,
    uint32_t width,
    uint32_t height
) : scenes(std::vector<Scene*>(0)),
	// canva(Canva()),
    current_scene(nullptr),
    game_title(game_title),
    width(width),
    height(height),
    window(sf::RenderWindow(
            sf::VideoMode(width, height),
            game_title,
            sf::Style::Titlebar | sf::Style::Close
    ))
    {
        window.setVerticalSyncEnabled(false);
        window.setFramerateLimit(60);
        glEnable(GL_TEXTURE_2D);
    }

void SceneManager::addScene(Scene* scene) {
    // This operation doesn't check redeclaration
    scenes.emplace_back(scene);
}

void SceneManager::runScene() {
	if (current_scene == nullptr) {
		// Error: trying to run a null scene
		ErrorHandler::printError(RUNNING_NULL_SCENE);
	}
    // check window event first
    while (window.isOpen()) {
        sf::Event event;
        if (window.pollEvent(event)) {
            // check if is close
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }
        std::string next_scene;
	    
        next_scene = current_scene->update();
        current_scene->blit(&window);

        if (next_scene != "") {
            changeScene(next_scene);
        }
    }
}

void SceneManager::changeScene(std::string name) {
    std::printf("Change to scene %s.\n", name.c_str());
    for (auto scene_p : scenes) {
        if (scene_p->getName() == name) {
            current_scene = scene_p;
            return;
        }
    }
    // If no name matched, report an Error
    ErrorHandler::printError(CHANGE_TO_NULL_SCENE);
}