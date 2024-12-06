#include "UserScene.hpp"

//==========================================================
// User defined Start Scene
//
StartScene::StartScene(sf::String name) :
    Scene(name), font(), select_result(0),
    cover_texture(), select_text(),
    selector(sf::CircleShape(15, 3))
    {
        init();
    }

void StartScene::init() {
    // Set cover picture
    if (!cover_texture.loadFromFile("gamecover.png")) {
        ErrorHandler::printError(IMAGE_NOT_FOUND);
    }
    cover.setTexture(cover_texture);
    cover.setPosition(sf::Vector2f(100, 50));
    
    // Set font and selection text
    if (!font.loadFromFile("WenQuanYiZenHeiMono-02.ttf")) {
        ErrorHandler::printError(FONT_NOT_FOUND);
    }
    select_text.setFont(font);
    select_text.setFillColor(sf::Color::Black);
    select_text.setCharacterSize(40);
    select_text.setPosition(350, 400);
    select_text.setString("BlackJack\nConnect 4");

    // Set selector
    selector.setRotation(90);
    selector.setPosition(300, 410);
    selector.setFillColor(sf::Color::Black);
}

sf::String StartScene::update(sf::RenderWindow* window) {
    setDeltaTime();
    sf::Event event;
    if (window->pollEvent(event)) {
        // Check if window closed
        if (event.type == sf::Event::Closed) {
            return "Exit";
        }
    }
    // Check keyboard input
    sf::String next_scene;
    next_scene = handleInput();
    return next_scene;
}
void StartScene::blit(sf::RenderWindow* window) {
    window->clear(sf::Color::White);

    // Draw cover image
    window->draw(cover);
    window->draw(select_text);
    window->draw(selector);
    // Display
    window->display();
}

sf::String StartScene::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
        select_result == 1) {
        // Move Up
        select_result = 0;
        selector.setPosition(300, 410);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
        select_result == 0) {
        // Move Down
        select_result = 1;
        selector.setPosition(300, 465);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
        return select_result ? "Connect4" : "Blackjack";
    }
    else {
        return "";
    }
}