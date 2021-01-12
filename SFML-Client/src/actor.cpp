#include <iostream>

//Files and strings operating
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "actor.h"
#include "network.h"


Actor Actor;

int act::MoveActor(int i, int dir){
    switch(dir){
case UP:
    Actor.y[i]--;
    break;
case DOWN:
    Actor.y[i]++;
    break;
case LEFT:
    Actor.x[i]--;
    break;
case RIGHT:
    Actor.x[i]++;
    break;
    }
    net::send_pos_packet();
}

void act::CreateActor(int x, int y){
    Actor.x.push_back(x);
    Actor.y.push_back(y);
}

int act::AddID(int id){
    Actor.id.push_back(id);
}

int act::GetX(int i){
    return Actor.x[i];
}

int act::GetY(int i){
    return Actor.y[i];
}
int act::GetID(int i){
    return Actor.id[i];
}
int act::GetIDSize(){
    return Actor.id.size();
}

int act::SetX(int i, int x){
    Actor.x[i] = x;
}

int act::SetY(int i, int y){
    Actor.y[i] = y;
}

int act::GetActorsSize(){
    return Actor.x.size();
}

