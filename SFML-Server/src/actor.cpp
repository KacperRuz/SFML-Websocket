#include <iostream>

//Files and strings operating
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "actor.h"


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
}

void act::CreateActor(int x, int y, int id){
    Actor.x.push_back(x);
    Actor.y.push_back(y);
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

int act::SetX(int i, int x){
    Actor.x[i] = x;
}

int act::SetY(int i, int y){
    Actor.y[i] = y;
}

int act::GetActorsSize(){
    return Actor.x.size();
}

