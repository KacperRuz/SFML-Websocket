#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED

#include<vector>

#define PLAYER 0

enum{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

namespace act{
    void CreateActor(int x, int y, int id);
    //Getters
    int GetX(int i);
    int GetY(int i);
    int GetID(int i);
    //Setters
    int SetX(int i, int x);
    int SetY(int i, int y);

    int GetActorsSize();
    int MoveActor(int i, int dir);
}


class Actor{
    public:
        std::vector<int>id;
        std::vector<int>x;
        std::vector<int>y;
};




#endif // ACTOR_H_INCLUDED
