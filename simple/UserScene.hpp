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

//==========================================================
// Start Scene
// 
#ifndef _StartScene
#define _StartScene
class StartScene : public  Scene {
private:
    // Input related
    sf::Font font;
    sf::Text select_text;
    sf::CircleShape selector;
    uint8_t select_result;

    // Background Related
    sf::Texture cover_texture;
    sf::Sprite cover;
    sf::Texture dies_texture;
    sf::Sprite dies;
    sf::Texture chess_texture;
    sf::Sprite chess;

public:
    StartScene(sf::String name);
    ~StartScene() = default;
    void init() override;
    sf::String update(sf::RenderWindow* window) override;
    void blit(sf::RenderWindow* window) override;
    
    // class defined functions
    sf::String handleInput();
};
#endif

//==========================================================
// Blackjack Scene
// 
/*
#ifndef _BJScene
#define _BJScene
class BJScene : public  Scene {
private:
    // Canva related
    sf::Font font;
    sf::RectangleShape input_box;
    sf::RectangleShape message_box;
    sf::Text input_text;
    sf::Text message_text;
    std::string current_input;
    std::deque<std::string> messages;

    const uint8_t max_lines = 10;
    const uint8_t max_chars = 17;

    std::vector<Player> 

public:
    BJScene(sf::String name);
    ~BJScene() = default;
    void init() override;
    sf::String update(sf::RenderWindow* window) override;
    void blit(sf::RenderWindow* window) override;
    
    // class defined functions
    void handleInput(sf::Uint32 unicode);

    // Player defined function

};
#endif

// Blackjack Scene objects
class Player {

}
*/