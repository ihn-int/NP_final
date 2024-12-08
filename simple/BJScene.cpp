#include "UserScene.hpp"

BJScene::BJScene(sf::String name) :
    Scene(name), font(), inputBox(), outputBox(),
    inputText(), messages()
    {
        init();
    }

BJScene::init() {
    // Load font file
    if (!font.loadFromFile("WenQuanYiZenHeiMono-02.ttf")) {
        ErrorHandler::printError(FONT_NOT_FOUND);
    }
    // Set input box
    input_box.setSize(sf::Vector2f(180, 40));
    input_box.setPosition(610, 550);
    input_box.setFillColor(sf::Color::(200, 200, 200));

    // Set input text
    input_text.setFont(font);
    input_text.setFillColor(sf::Color::Black);
    input_text.setCharacterSize(20);
    input_text.setPosition(615, 555);

    // Set message box
    message_box.setSize(sf::Vector2f(180, 260));
    message_box.setPosition(610, 280);
    message_box.setFillColor(sf::Color::(220, 220, 220));

    // Set history message text
    message_text.setFont(font);
    message_text.setFillColor(sf::Color::Black);
    message_text.setCharacterSize(20);
}

sf::String BJScene::update(sf::RenderWindow* window) {
    // set delta_time;
    setDeltaTime();

    // get date from socket;

    sf::Event event;
}