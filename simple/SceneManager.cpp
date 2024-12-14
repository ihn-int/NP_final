#include "Scene.hpp"
// #include "uitl.hpp"

SceneManager::SceneManager(
    sf::String game_title,
    uint32_t width,
    uint32_t height) : game_title(game_title),
                       width(width),
                       height(height)
{
    init();
}

void SceneManager::init()
{
    // create window
    glEnable(GL_TEXTURE_2D);
    sf::ContextSettings settings(24, 8, 4, 4, 3, 0, false);
    window.create(
        sf::VideoMode(width, height),
        game_title,
        sf::Style::Titlebar | sf::Style::Close,
        settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    // Scene container
    scenes = std::vector<Scene *>(0);
}

void SceneManager::addScene(Scene *scene)
{
    scenes.emplace_back(scene);
    scene->init();
}

void SceneManager::changeScene(sf::String name)
{
    std::printf("INFO: Jump to %s scene.\n",
                name.toAnsiString().c_str());
    for (auto scene : scenes)
    {
        if (scene->getName() == name)
        {
            current_scene = scene;
            return;
        }
    }
    ErrorHandler::printError(CHANGE_TO_NULL_SCENE);
}

void SceneManager::runScene()
{
    // check buffer
    // skip
    if (current_scene == nullptr)
    {
        ErrorHandler::printError(RUNNING_NULL_SCENE);
    }

    // check if wondow open
    while (window.isOpen())
    {
        sf::String next_scene = current_scene->update(&window);
        current_scene->blit(&window);
        if (next_scene != "")
        {
            if (next_scene == "Exit")
            {
                // User quit
                window.close();
            }
            else
            {
                changeScene(next_scene);
            }
        }
    }
}