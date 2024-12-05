#include "UserScene.hpp"

//==========================================================
// User defined Scene
ChatScene::ChatScene(sf::String name) :
    Scene(name), font(), inputBox(), outputBox(), 
    inputText(), messages()
    {
        init();
    }

void ChatScene::init() {
    // Load font file
    if (!font.loadFromFile("WenQuanYiZenHeiMono-02.ttf")) {
        ErrorHandler::printError(FONT_NOT_FOUND);
    }

    inputBox.setSize(sf::Vector2f(180, 40));
    inputBox.setPosition(610, 550);
    inputBox.setFillColor(sf::Color(200, 200, 200));

    outputBox.setSize(sf::Vector2f(180, 540));
    outputBox.setPosition(610, 10);
    outputBox.setFillColor(sf::Color(220, 220, 220));

    inputText.setFont(font);
    inputText.setFillColor(sf::Color::Black);
    inputText.setCharacterSize(20);
    inputText.setPosition(615, 555);

    messageText.setFont(font);
    messageText.setFillColor(sf::Color::Black);
    messageText.setCharacterSize(20);
}

sf::String ChatScene::update(sf::RenderWindow* window) {
    setDeltaTime();
    sf::Event event;
    if (window->pollEvent(event)) {
        // some event happend
        if (event.type == sf::Event::Closed) {
            return "Exit";
        }
        if (event.type == sf::Event::TextEntered) {
            handleInput(event.text.unicode);
        }
    }
    return "";
}

void ChatScene::blit(sf::RenderWindow* window) {
    window->clear(sf::Color::White);
    window->draw(outputBox);
    float yoffset = 15.0;
    for(auto& message : messages) {
        messageText.setString(message);
        messageText.setPosition(615, yoffset);
        window->draw(messageText);
        yoffset += 25;
    }
    window->draw(inputBox);
    window->draw(inputText);
    window->display();
}

void ChatScene::handleInput(sf::Uint32 unicode) {
    if (unicode == '\r') {
        if (!currentInput.empty()) {
                messages.push_back(currentInput);
                if (messages.size() > maxLines) {
                    messages.pop_front();
                }
                currentInput.clear();
                inputText.setString(L"");
            }
        } else if (unicode == '\b') { // Backspace 鍵
            if (!currentInput.empty()) {
                currentInput.pop_back();
                inputText.setString(currentInput);
            }
        } else if (unicode >= 0x4E00 && unicode <= 0x9FFF) { // 中文字符範圍
        currentInput += static_cast<wchar_t>(unicode);
        inputText.setString(currentInput);
    } else if (unicode >= 32 && unicode < 127) { // 英文等基本字符
        currentInput += static_cast<wchar_t>(unicode);
        inputText.setString(currentInput);
    }
}
