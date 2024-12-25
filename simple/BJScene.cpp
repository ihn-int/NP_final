#include "UserScene.hpp"

BJScene::BJScene(sf::String name) :
    Scene(name), font(), input_box(), message_box(),
    input_text(), messages()
    {
        init();
    }

void BJScene::init() {
    // Load font file
    if (!font.loadFromFile("WenQuanYiZenHeiMono-02.ttf")) {
        ErrorHandler::printError(FONT_NOT_FOUND);
    }
    // Set input box place and size
    input_box.setSize(sf::Vector2f(180, 40));
    input_box.setPosition(610, 550);
    input_box.setFillColor(sf::Color(200, 200, 200));

    // Set input text place and size
    input_text.setFont(font);
    input_text.setFillColor(sf::Color::Black);
    input_text.setCharacterSize(20);
    input_text.setPosition(615, 555);

    // Set message box place and size
    message_box.setSize(sf::Vector2f(180, 260));
    message_box.setPosition(610, 280);
    message_box.setFillColor(sf::Color(220, 220, 220));

    // Set history message text place and size
    message_text.setFont(font);
    message_text.setFillColor(sf::Color::Black);
    message_text.setCharacterSize(20);

    // Set up player related informations
    // Set up cards textures
    for (int i = 0; i < 53; i++) {
        std::string card = card_files[i];
        card_textures[card] = sf::Texture();
        if (!card_textures[card].loadFromFile(
            "poker/" + card + ".png" )){
            ErrorHandler::printError(IMAGE_NOT_FOUND);
        }
    }
    // Set user id place and size
    user_readys = std::vector<bool>(4, false);
    user_ids = std::vector<std::string>(4, "");
    user_id_texts = std::vector<sf::Text>(4, sf::Text());
    for (int i = 0; i < 4; i++) {
        user_id_texts[i].setFont(font);
        user_id_texts[i].setFillColor(sf::Color::Black);
        user_id_texts[i].setCharacterSize(30);
        user_id_texts[i].setPosition(10, 10 + i * 110);
    }
    // Set user point place and size
    user_pts = std::vector<int>(4, 0);
    user_pt_texts = std::vector<sf::Text>(4, sf::Text());
    for (int i = 0; i < 4; i++) {
        user_pt_texts[i].setFont(font);
        user_pt_texts[i].setFillColor(sf::Color::Black);
        user_pt_texts[i].setCharacterSize(30);
        user_pt_texts[i].setPosition(10, 50 + i * 110);
    }

    // Set user card place and size
    user_cards = std::vector<std::vector<std::string>>(
        4, std::vector<std::string>(5, "")
    );
    user_card_sprites = std::vector<std::vector<sf::Sprite>>(
        4, std::vector<sf::Sprite>(5, sf::Sprite())
    );
    for (int i = 0; i < 4; i++) {
        int y = 10 + i * 110;
        for (int j = 0; j < 5; j++) {
            user_card_sprites[i][j].setPosition(170 + 80 * j, y);
        }
    }
    // Set time zone place and size
    game_state = 0;
    game_state_text = sf::Text();
    game_state_text.setFont(font);
    game_state_text.setCharacterSize(30);
    game_state_text.setFillColor(sf::Color::Black);
    game_state_text.setPosition(615, 10);

    // Set last time place and size
    last_time = 0;
    last_time_text = sf::Text();
    last_time_text.setFont(font);
    last_time_text.setCharacterSize(30);
    last_time_text.setFillColor(sf::Color::Black);
    last_time_text.setPosition(615, 50);

    // Set player's current point place and size
    current_point = 0;
    current_point_text = sf::Text();
    current_point_text.setFont(font);
    current_point_text.setCharacterSize(30);
    current_point_text.setFillColor(sf::Color::Black);
    current_point_text.setPosition(615, 90);

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

    // Set to wait state
    game_state = 0;

}

sf::String BJScene::update(sf::RenderWindow* window) {
    // Set delta_time;
    setDeltaTime();

    // De-assert send flag and state flag
    is_send = false;
    back_to_lobby = false;

    // Recieve socket data if needed
    if (is_recv) {  // the recieve flag is asserted
        parseOps(std::string(recvline));
    }


    // Check sfml event
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

void BJScene::blit(sf::RenderWindow* window) {
    window->clear(sf::Color::White);

    // Blit user ids
    for (int i = 0; i < 4; i++) {
        if (user_ids[i] != "") {
            // player i exists
            user_id_texts[i].setString(
                (user_readys[i] ? "(R)" : "   ") + 
                user_ids[i]
            );
            window->draw(user_id_texts[i]);
        }
    }

    // Blit user points
    for (int i = 0; i < 4; i++) {
        if (user_ids[i] != "") {
            // player i exists
            user_pt_texts[i].setString(
                "   " + std::to_string(user_pts[i])
            );
            window->draw(user_pt_texts[i]);
        }
    }

    // Blit user cards on at game state
    for (int i = 0; i < 4; i ++) {
        if (user_ids[i] != "") {
            // player i exists
            for (int j = 0; j < 5; j++) {
                std::string card = user_cards[i][j];
                if (card != "") {
                    // card j is holded
                    user_card_sprites[i][j].setTexture(card_textures[card]);
                    window->draw(user_card_sprites[i][j]);
                }
            }
        }
    }

    // Blit game state
    switch(game_state) {
        case 0: // waiting state
            game_state_text.setString("Waiting");
            break;
        case 1: // betting state
            game_state_text.setString("Betting");
            break;
        case 2: // asking state
            game_state_text.setString("Asking");
            break;
        case 3: // opening state
            game_state_text.setString("Opening");
            break;
        default:// ???
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
        message_text.setPosition(615, yoffset);
        window->draw(message_text);
        yoffset += 25;
    }

    // draw input message
    window->draw(input_box);
    window->draw(input_text);
    

    window->display();
}

void BJScene::parseOps(std::string op) {
    std::istringstream opstream(op);
    std::string less;
    int opcode;
    int index;
    std::string token;
    char abbr;
    int number;
    int number2;
    std::string chatinfo = "";
    opstream >> opcode;
    switch(opcode) {
        case 3: // player name
            opstream >> index >> token;
            if (token.size() > 8){
                // Upperbound is 8
                token = token.substr(0, 8);
            }
            user_ids[index] = token;
            break;
        case 4: // ready
            opstream >> index;
            user_readys[index] = true;
            break;
        case 5: // player quit
            opstream >> index;
            user_ids[index] = "";
            user_readys[index] = false;
            break;
        case 17: // player's current points
            opstream >> current_point;
            break;
        case 102: // legal command
            opstream >> token;
            // check ready
            commands[4] = (token[0] == '1') ? "4 ready" : "";
            commands[5] = (token[1] == '1') ? "5 quit" : "";
            commands[0] = (token[2] == '1') ? "0 bet" : "";
            commands[1] = (token[3] == '1') ? "1 hiy" : "";
            commands[2] = (token[4] == '1') ? "2 stand" : "";
            break;
        case 101: // chat
            while(opstream >> token){
                chatinfo += token + " ";
            }
            chatinfo.pop_back();
            handleMessage(chatinfo);
            break;
        case 550: // waiting zone
            opstream >> abbr;
            switch (abbr) {
                case 'W' :
                    game_state = 0; // 0 for wait
                    // clear card
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 5; j++) {
                            user_cards[i][j] = "";
                        }
                        user_pts[i] = 0;
                    }
                    break;
                case 'B':
                    game_state = 1; // 1 for bet
                    break;
                case 'A':
                    game_state = 2; // 2 for ask
                    break;
                case 'O':
                    game_state = 3; // 3 for open
                    break;
                default:    // default to wait
                    game_state = 0;
                    // clear card
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 5; j++) {
                            user_cards[i][j] = "";
                        }
                        user_pts[i] = 0;
                    }
                    break;
            }
            break;
        case 555: // last time
            opstream >> last_time;
            break;
        case -2: // someone time out
            opstream >> index;
            if (index == 0) { // self time out, return to lobby
                back_to_lobby = true;
                break;
            }
            else {
                user_ids[index] = "";
                user_readys[index] = false;
                break;
            }
        case 10:   // player bets points
            opstream >> index >> number;
            user_pts[index] = number;
            break;
        case 11:   // player gets card
            opstream >> index >> number >> number2;
            // linear search for the first empty card slot
            user_cards[index][number] = card_files[number2];
            break;
        default:
            std::printf("[INFO] Unknown instruction: %s.\n", op.c_str());
            //ErrorHandler::printError(UNKNOWN_OPCODE); 
    }
}

void BJScene::handleInput(sf::Uint32 unicode) {
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

void BJScene::handleMessage(std::string chatinfo) {
    // We need to split the chat info in unit of 17
    int start = 0;
    int length = 0;
    while(start < chatinfo.size()) {
        length = (17 < chatinfo.size()-start) ? 17 : chatinfo.size()-start;
        messages.push_back(chatinfo.substr(start, length));
        if (messages.size() > max_lines) {
            messages.pop_front();
        }
        start += length;
    }
}

void BJScene::parseSendOps(std::string send_op) {
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
            case 0: // bet
                ops >> number;
                sprintf(sendline, "6 %i\n", number);
                break;
            case 1: // ask
                sprintf(sendline, "7\n");
                break;
            case 2: // stop
                sprintf(sendline, "8\n");
                break;
            case 3: // nop
                is_send = false;
                break;
                // send nothing
            case 4: // ready
                sprintf(sendline, "2\n");
                break;
            case 5: // quit
                // assert back to lobby here
                back_to_lobby = true;
                sprintf(sendline, "-1\n");
                break;
            default: // unknown op, send nothing
                is_send = false;
                break;
        }
    }
    else {  // chat message
        sprintf(sendline, "100 %s\n", send_op.c_str());
    }
}