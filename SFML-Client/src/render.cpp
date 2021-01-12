#include <iostream>

/*
@render.cpp and @render.h are handles rendering and window/keyboard events(coz im lazy)
*/



// SFML 2.5
#include <SFML/Graphics.hpp>

//Networking
#include <winsock2.h>

// Strings operating
#include<string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "render.h"
#include "network.h"
#include "actor.h"

sf::RenderWindow window(sf::VideoMode(800, 600), "Messenger");
sf::Event event;

sf::Font font;
sf::Text character_text;

sf::Texture texture;
sf::Sprite sprite;

bool rdr::init_sfml(){
    //fonts
    font.loadFromFile("font.ttf");
    character_text.setFont(font);
    character_text.setCharacterSize(16);

    //window parameters
    //window.setFramerateLimit(60);
    //window.setVerticalSyncEnabled(true);
    return true;
}

void rdr::keep_window(){
    if(window.isOpen()){
        window.clear();
        render_actors();
        render_debug();
        window.display();
    }
}

int rdr::window_event_handling(){
    while(window.pollEvent(event)){
        if (event.type == sf::Event::Closed){
        window.close();
        return 0;
        }
        keyboard_event_handling();
    }
    return 1;
}

int rdr::keyboard_event_handling(){
    if(event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::Enter){
            net::send_pos_packet();
        }
        if(event.key.code == sf::Keyboard::Up)act::MoveActor(0,UP);
        if(event.key.code == sf::Keyboard::Down)act::MoveActor(0,DOWN);
        if(event.key.code == sf::Keyboard::Left)act::MoveActor(0,LEFT);
        if(event.key.code == sf::Keyboard::Right)act::MoveActor(0,RIGHT);
    }
    return 1;
}

int rdr::render_debug(){
    if(act::GetIDSize() > 0){
    for(int i = 0; i < act::GetActorsSize(); i++){
        std::ostringstream st;
        st << "ID: " << act::GetID(i) << " X: " << act::GetX(i) << " Y: " << act::GetY(i);
        std::string str = st.str();
        character_text.setString(str);
        character_text.setPosition(0,i*24);
        window.draw(character_text);
    }
    }
}

int rdr::render_actors(){
    for(int i = 0; i < act::GetActorsSize(); i++){
        character_text.setColor(sf::Color::White);
        if(i > 0)
            character_text.setColor(sf::Color::Red);
        character_text.setString("@");
        character_text.setPosition(act::GetX(i)*20,act::GetY(i)*20);
        window.draw(character_text);
    }
    return 1;
}

int rdr::rendering_loop(){
    if(window_event_handling() == 0)return 0;
    keep_window();
    return 1;
}
