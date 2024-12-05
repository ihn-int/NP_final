// User defined scene
#include "Scene.hpp"

#ifndef _ChatScene
#define _ChatScene
class ChatScene : public  Scene {
private:
    sf::Font font;
    sf::RectangleShape inputBox;
    sf::RectangleShape outputBox;
    sf::Text inputText;
    sf::Text messageText;
    std::wstring currentInput;
    std::deque<std::wstring> messages;

    const uint32_t maxLines = 10;

public:
    ChatScene(sf::String name);
    ~ChatScene() = default;
    void init() override;
    sf::String update(sf::RenderWindow* window) override;
    void blit(sf::RenderWindow* window) override;
    
    // class defined functions
    void handleInput(sf::Uint32 unicode);
};
#endif