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

    // Background related
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
#ifndef _BJScene
#define _BJScene

const std::string card_files[] = {"BG",
    "SA", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9",
    "S10","SJ", "SQ", "SK", "HA", "H2", "H3", "H4", "H5",
    "H6", "H7", "H8", "H9", "H10","HJ", "HQ", "HK", "DA",
    "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "D10",
    "DJ", "DQ", "DK", "CA", "C2", "C3", "C4", "C5", "C6",
    "C7", "C8", "C9", "C10","CJ", "CQ", "CK"
};

class BJScene : public  Scene {
private:
    //============================================
    // Canva related
    sf::Font font;

    sf::RectangleShape input_box;
    sf::Text input_text;
    
    sf::RectangleShape message_box;
    sf::Text message_text;
    std::deque<std::string> messages;
    
    std::string current_input;

    const uint8_t max_lines = 10;
    const uint8_t max_chars = 17;

    // Game state
    int game_state;
    sf::Text game_state_text;
    int last_time;
    sf::Text last_time_text;
    
    // end of canva related
    //============================================

    //============================================
    // Player related
    // players' ids
    std::vector<std::string> user_ids;
    std::vector<sf::Text> user_id_texts;

    // players' ready info
    std::vector<bool> user_readys;

    // players' points
    std::vector<int> user_pts;
    std::vector<sf::Text> user_pt_texts;
    int current_point;
    sf::Text current_point_text;

    // players' cards
    std::vector<std::vector<std::string>> user_cards;
    std::vector<std::vector<sf::Sprite>> user_card_sprites;

    // poker cards' texture, load to user card sprite
    // load only at init()
    std::map<std::string, sf::Texture> card_textures;
    
    // player's available command
    std::vector<std::string> commands;
    std::vector<sf::Text> command_texts;


    // end of player related
    //============================================

    //============================================
    // Socket data related
    std::vector<std::string> op_from_servs;

    // end of socket data related
    //============================================

    //============================================
    // state machine
    bool back_to_lobby;
    //============================================

public:
    BJScene(sf::String name);
    ~BJScene() = default;
    void init() override;
    sf::String update(sf::RenderWindow* window) override;
    void blit(sf::RenderWindow* window) override;
    
    // class defined functions
    void handleInput(sf::Uint32 unicode);
    void handleMessage(std::string chatinfo);
    void parseOps(std::string op);
    void parseSendOps(std::string send_op);
    void cleanCard();
};
#endif

#ifndef __C4Scene
#define __C4Scene

class C4Scene : public Scene {
private:
    //============================================
    // Canva related
    sf::Font font;

    sf::RectangleShape input_box;
    sf::Text input_text;
    
    sf::RectangleShape message_box;
    sf::Text message_text;
    std::deque<std::string> messages;
    
    std::string current_input;

    const uint8_t max_lines = 10;
    const uint8_t max_chars = 17;

    // Game state
    int game_state; // an extra state -1 for matching
    sf::Text game_state_text;
    int last_time;
    sf::Text last_time_text;
    
    

    // end of canva related
    //============================================

    //============================================
    // player related
    // chess board
    sf::RectangleShape board;
    std::vector<std::vector<int>> chessboard;
    std::vector<std::vector<sf::CircleShape>> chess_circles;

    std::string player_a;
    bool player_a_ready;
    std::string player_b;
    bool player_b_ready;
    sf::Text player_avsb;

    std::vector<std::string> commands;
    std::vector<sf::Text> command_texts;

    std::vector<std::string> op_from_servs;
    bool back_to_lobby;

public:
    C4Scene(sf::String name);
    ~C4Scene() = default;
    void init() override;
    sf::String update(sf::RenderWindow* window) override;
    void blit(sf::RenderWindow* window) override;

    void handleInput(sf::Uint32 unicode);
    void parseOps(std::string op);
    void handleMessage(std::string chatinfo);
    void parseSendOps(std::string send_op);
};

#endif