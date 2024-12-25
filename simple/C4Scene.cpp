#include "UserScene.hpp"

C4Scene::C4Scene(sf::String name) :
    Scene(name) {
        init();
    }

void C4Scene::init() {
    // Load font file
    font = sf::Font();
    if (!font.loadFromFile("WenQuanYiZenHeiMono-02.ttf")) {
        ErrorHandler::printError(FONT_NOT_FOUND);
    }
    
    // Set input box place and size
    input_box = sf::RectangleShape();
    input_box.setSize(sf::Vector2f(210, 40));
    input_box.setPosition(580, 550);
    input_box.setFillColor(sf::Color(200, 200, 200));

    // Set input text place and size
    input_text = sf::Text();
    input_text.setFont(font);
    input_text.setFillColor(sf::Color::Black);
    input_text.setCharacterSize(20);
    input_text.setPosition(585, 555);

    // Clear history message
    messages.clear();

    // Set message box place and size
    message_box = sf::RectangleShape();
    message_box.setSize(sf::Vector2f(210, 260));
    message_box.setPosition(580, 280);
    message_box.setFillColor(sf::Color(220, 220, 220));

    // Set history message text place and size
    message_text = sf::Text();
    message_text.setFont(font);
    message_text.setFillColor(sf::Color::Black);
    message_text.setCharacterSize(20);

    // Set board
    board = sf::RectangleShape();
    board.setFillColor(sf::Color::Blue);
    board.setSize(sf::Vector2f(430, 370));
    board.setPosition(90, 90);

    // Set chess board
    chessboard = std::vector<std::vector<int>>(
        6, std::vector<int>(7, -1) // -1 for empty slot
    );
    chess_circles = std::vector<std::vector<sf::CircleShape>>(
        6, std::vector<sf::CircleShape>(7, sf::CircleShape(25))
    );
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            chess_circles[i][j].setPosition(100+60*j, 100+60*i);
        }
    }

    // Set time zone place and size
    game_state = -1;
    game_state_text = sf::Text();
    game_state_text.setFont(font);
    game_state_text.setCharacterSize(30);
    game_state_text.setFillColor(sf::Color::Black);
    game_state_text.setPosition(585, 10);

    // Set last time place and size
    last_time = 0;
    last_time_text = sf::Text();
    last_time_text.setFont(font);
    last_time_text.setCharacterSize(30);
    last_time_text.setFillColor(sf::Color::Black);
    last_time_text.setPosition(585, 50);

    // Set player's current point place and size
    current_point = 0;
    current_point_text = sf::Text();
    current_point_text.setFont(font);
    current_point_text.setCharacterSize(30);
    current_point_text.setFillColor(sf::Color::Black);
    current_point_text.setPosition(585, 90);

    // Set player info
    player_a = "";
    player_a_ready = false;
    player_b = "";
    player_b_ready = false;
    player_avsb = sf::Text();
    player_avsb.setFont(font);
    player_avsb.setCharacterSize(30);
    player_avsb.setFillColor(sf::Color::Black);
    player_avsb.setPosition(585, 130);

    // Set commands place and size
    commands = std::vector<std::string>(8, "");
    command_texts = std::vector<sf::Text>(8, sf::Text());
    for (int i = 0; i < 4; i++) {
        command_texts[i].setFont(font);
        command_texts[i].setCharacterSize(30);
        command_texts[i].setFillColor(sf::Color::Black);
        command_texts[i].setPosition(10 + i * 130, 500);
    }
    for (int i = 4; i < 8; i++) {
        command_texts[i].setFont(font);
        command_texts[i].setCharacterSize(30);
        command_texts[i].setFillColor(sf::Color::Black);
        command_texts[i].setPosition(10 + (i-4) * 130, 550);
    }

    game_state = -1; // -1 for matching
}

sf::String C4Scene::update(sf::RenderWindow* window) {
    // Set delta time
    setDeltaTime();

    // De-assert send flag and state flag
    is_send = false;
    back_to_lobby = false;

    // Recieve socket data if needed
    if (is_recv) {
        parseOps(std::string(recvline));
    }
    
    sf::Event event;
    if (window->pollEvent(event)) {
        // Check if window closed
        if (event.type == sf::Event::Closed) {
            return "Exit";
        }
        // Check keyboard input
        if (event.type == sf::Event::TextEntered) {
            handleInput(event.text.unicode);
        }
    }
    return (back_to_lobby) ? "Start" : "";
}

void C4Scene::blit(sf::RenderWindow* window) {
    window->clear(sf::Color::White);

    // Blit chess board
    window->draw(board);

    // Blit chesses
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            if (chessboard[i][j] == -1) {
                // empty, set to white
                chess_circles[i][j].setFillColor(sf::Color::White);
            }
            else if (chessboard[i][j] == 0) {
                // self, set to red
                chess_circles[i][j].setFillColor(sf::Color::Red);
            }
            else if (chessboard[i][j] == 1) {
                // opposite, set to yellow 
                chess_circles[i][j].setFillColor(sf::Color::Yellow);
            }
            window->draw(chess_circles[i][j]);
        }
    }
    // Blit game state
    switch(game_state) {
        case -1: // matching state
            game_state_text.setString("Matching");
            break;
        case 0:  // waiting state
            game_state_text.setString("Waiting");
            break;
        case 1:  // y state
            game_state_text.setString("Self");
            break;
        case 2:  // c state
            game_state_text.setString("Opposite");
            break;
        case 3:  // finish state
            game_state_text.setString("Finish");
            break;
        default: 
            game_state_text.setString("Unknown");
            break;
    }
    window->draw(game_state_text);

    // Blit last time
    last_time_text.setString("Last: " + std::to_string(last_time));
    window->draw(last_time_text);

    // Blit player current point
    current_point_text.setString("Pts: " + std::to_string(current_point));
    window->draw(current_point_text);

    // Blit player a vs. player b
    std::string tmp_a = (player_a_ready) ? "(R)" : "   ";
    std::string tmp_b = (player_b_ready) ? "(R)" : "   ";
    tmp_a += (player_a.size() > 8) ? player_a.substr(0, 8) : player_a;
    tmp_b += (player_b.size() > 8) ? player_b.substr(0, 8) : player_b;
    player_avsb.setString(tmp_a + "\n   vs.\n" + tmp_b);
    window->draw(player_avsb);
    // Blit legal commands
    for (int i = 0; i < 8; i++) {
        if (commands[i] != "") {
            command_texts[i].setString(commands[i]);
            window->draw(command_texts[i]);
        }
    }

    // draw history message
    window->draw(message_box);
    float yoffset = 285;
    for (auto& message : messages) {
        message_text.setString(message);
        message_text.setPosition(585, yoffset);
        window->draw(message_text);
        yoffset += 25;
    }

    // draw input message
    window->draw(input_box);
    window->draw(input_text);

    window->display();
}

void C4Scene::parseOps(std::string op) {
    std::istringstream opstream(op);
    std::string less;
    int opcode;
    int index;

    std::string token;
    char abbr;
    int number;
    int r = 0, c = 0;

    std::string chatinfo = "";
    opstream >> opcode;
    switch (opcode) {
        case 3:  // players' name
            opstream >> index;
            if (index == 0) { // player a name
                opstream >> player_a;
            }
            else {  // player b name
                opstream >> player_b;
            }
            break;
        case 4:  // player ready
            opstream >> index;
            if (index == 0) { // player a ready
                player_a_ready = true;
            }
            else {  // player b ready
                player_b_ready = true;
            }
            break;
        case 17: // player's current point
            opstream >> current_point;
            break;
        case 15: // down
            opstream >> index >> token;
            r = token[0] - '0';
            c = token[1] - '0';
            chessboard[r][c] = index;
            break;
        case 101: // chat
            while (opstream >> token) {
                chatinfo += token + " ";
            }
            chatinfo.pop_back();
            handleMessage(chatinfo);
            break;
        case 102: // legal command
            opstream >> token;
            commands[4] = (token[0] == '1') ? "4 ready" : "";
            commands[5] = (token[1] == '1') ? "5 quit" : "";
            commands[0] = (token[2] == '1') ? "0 down" : "";
            commands[1] = (token[3] == '1') ? "1 surrend" : "";
            break;
        case 550: // game state
            opstream >> abbr;
            switch (abbr) {
                case 'W': // waiting state
                    game_state = 0;
                    
                    break;
                case 'Y': // self
                    game_state = 1;
                    break;
                case 'C': // opposite
                    game_state = 2;
                    break;
                case 'F': // finish
                    game_state = 3;
                    break;
                case 'M': // match
                    game_state = -1;// clean up chess board
                    for (int i = 0; i < 6; i++) {
                        for (int j = 0; j < 7; j++) {
                            chessboard[i][j] = -1;
                        }
                    }
                    player_a = "";
                    player_b = "";
                    last_time = 0;
                    player_a_ready = false;
                    player_b_ready = false;
                    break;
                default:  // default to match
                    game_state = -1;// clean up chess board
                    for (int i = 0; i < 6; i++) {
                        for (int j = 0; j < 7; j++) {
                            chessboard[i][j] = -1;
                        }
                    }
                    player_a = "";
                    player_b = "";
                    last_time = 0;
                    player_a_ready = false;
                    player_b_ready = false;
                    break;
            }
            break;
        case 555: // timer
            opstream >> last_time;
            break;
        case (-2):// someone time out
            opstream >> index;
            if (index == 0) { // self time out, return to lobby
                back_to_lobby = true;
            }
            break;
        default:
            std::printf("[INFO] Unknown instruction: %s\n", op.c_str());
    }
}

void C4Scene::handleInput(sf::Uint32 unicode) {
    if (unicode == '\r') {  // enter key
        if (!current_input.empty()) {
            // Send message via socket
            parseSendOps(current_input);
            current_input.clear();
            input_text.setString("");
        }
    }
    else if (unicode == '\b') { // backspace
        if (!current_input.empty()) {
            current_input.pop_back();
            input_text.setString(current_input);
        }
    }
    else if (unicode >= 32 && unicode < 127) {
        // Other visible ascii characters
        current_input += static_cast<char>(unicode);
        input_text.setString(current_input);
    }
}

void C4Scene::handleMessage(std::string chatinfo) {
    // We need to split the chat info in unit of 20
    int start = 0;
    int length = 0;
    while(start < chatinfo.size()) {
        length = (20 < chatinfo.size()-start) ? 20 : chatinfo.size()-start;
        messages.push_back(chatinfo.substr(start, length));
        if (messages.size() > max_lines) {
            messages.pop_front();
        }
        start += length;
    }
}

void C4Scene::parseSendOps(std::string send_op) {
    // Assert the send flag
    is_send = true;

    if (send_op[0] == '$') {
        // Some operation
        send_op = send_op.substr(1, send_op.size()-1);
        std::istringstream ops(send_op);
        int opcode;
        int number;

        ops >> opcode;
        switch(opcode) {
            case 0: // down
                ops >> number;
                sprintf(sendline, "13 %i\n", number);
                break;
            case 1: // surrend
                sprintf(sendline, "14\n");
                break;
            case 4: // ready
                sprintf(sendline, "2\n");
                break;
            case 5: // quit
                // back_to_lobby = true;
                sprintf(sendline, "-1\n");
                break;
            default:
                is_send = false;
                break;
        }
    }
    else{   // chat message
        sprintf(sendline, "100 %s\n", send_op.c_str());
    }
}