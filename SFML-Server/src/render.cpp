#include <iostream>

// SFML 2.5
#include <SFML/Graphics.hpp>

//CMD cursor positioning
#include <windows.h>

#include "render.h"
#include "actor.h"

int n = 0;
int m = 0;

void gotoxy(int x, int y)
{
  COORD pos = {x, y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

bool init_sfml(){// Might be used later.
    sf::RenderWindow window(sf::VideoMode(800, 600), "Messenger");
    return true;
}

int interface_render(){
    /*n++;
    gotoxy(50,0);
    std::cout << n;
    if(n == 59){
        m++;
        n = 0;
    }
    gotoxy(55,0);
    std::cout << m;*/
    gotoxy(0,4);
    std::cout << "                                             ";
    gotoxy(0,4);
    if(act::GetActorsSize() > 0)
    std::cout << "X: " << act::GetX(0) << "Y: " << act::GetY(0);
    gotoxy(0,6);
    std::cout << "                                             ";
    gotoxy(0,6);
    if(act::GetActorsSize() > 0)
    std::cout << "X: " << act::GetX(1) << "Y: " << act::GetY(1);
     gotoxy(0,8);
    //std::cout << "                                             ";
    gotoxy(0,8);
    if(act::GetActorsSize() > 0)
    std::cout << "S:" << act::GetActorsSize();
}
