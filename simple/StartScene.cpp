#include "UserScene.hpp"

//==========================================================
// User defined Start Scene
//
StartScene::StartScene(sf::String name) :
    Scene(name), font(), select_result(0),
    cover_texture(), dies_texture(), chess_texture(),
    select_text(), dies(), chess(),
    selector(sf::CircleShape(15, 3))
    {
        init();
    }

void StartScene::init() {
    // Set cover picture
    if (!cover_texture.loadFromFile("background/gamecover.png")) {
        ErrorHandler::printError(IMAGE_NOT_FOUND);
    }
    cover.setTexture(cover_texture);
    cover.setPosition(sf::Vector2f(100, 50));
    
    // Set dies and chess decorations
    /**/
    if (!dies_texture.loadFromFile("background/dies.png")) {
        ErrorHandler::printError(IMAGE_NOT_FOUND);
    }
    dies.setTexture(dies_texture);
    dies.setPosition(sf::Vector2f(700, 0));
    if (!chess_texture.loadFromFile("background/chess.png")) {
        ErrorHandler::printError(IMAGE_NOT_FOUND);
    }
    chess.setTexture(chess_texture);
    chess.setPosition(sf::Vector2f(-80, 90));
    /**/

    // Set font and selection text
    if (!font.loadFromFile("WenQuanYiZenHeiMono-02.ttf")) {
        ErrorHandler::printError(FONT_NOT_FOUND);
    }

    select_result = 0;

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
    // Recieve socket data if needed
    // No data should be recieve in StartScene
    // De-assert send flag
    is_send = false;

    // Check sfml event
    sf::Event event;
    sf::String next_scene = "";

    if (window->pollEvent(event)) {
        // Check if window closed
        if (event.type == sf::Event::Closed) {
            //return "Exit";
            next_scene = "Exit";
        }
        else if (event.type == sf::Event::KeyPressed) {
            next_scene = handleInput(event.key.code);
        }

    }
    return next_scene;
}
void StartScene::blit(sf::RenderWindow* window) {
    window->clear(sf::Color::White);

    // Draw cover image
    window->draw(chess);
    window->draw(dies);
    window->draw(cover);
    window->draw(select_text);
    window->draw(selector);
    // Display
    window->display();
}

sf::String StartScene::handleInput(sf::Keyboard::Key code) {
    if (code == sf::Keyboard::Up && select_result == 1) {
        // Move Up
        select_result = 0;
        selector.setPosition(300, 410);
    }
    if (code == sf::Keyboard::Down && select_result == 0) {
        // Move Down
        select_result = 1;
        selector.setPosition(300, 465);
    }
    //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
    if (code == sf::Keyboard::Enter) {
        // Handle socket when pressing the enter key

        if (select_result == 0) { // send blackjack
            sprintf(sendline, "1 B\n");
            is_send = true;
        }
        else if (select_result == 1) { // send connect 4
            sprintf(sendline, "1 F\n");
            is_send = true;
        }
        return select_result ? "Connect4" : "Blackjack";
    }
    else {
        return "";
    }
}