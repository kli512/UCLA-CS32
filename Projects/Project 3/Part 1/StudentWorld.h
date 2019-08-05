#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	~StudentWorld();
	StudentWorld();
	bool isValidPos(double x, double y);

private:
	Penelope* m_penelope;
	vector<Actor*> actors;
};

#endif // STUDENTWORLD_H_
