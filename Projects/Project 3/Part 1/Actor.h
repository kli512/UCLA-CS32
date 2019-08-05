#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
	// could maybe default startDirection = right, depth = 0
	Actor(StudentWorld* world, int imageID, double startX, double startY, int startDirection, int depth) :
		GraphObject(imageID, startX, startY, startDirection, depth),
		m_world(world) {};

	virtual void doSomething() = 0;
	StudentWorld* getWorld() { return m_world; };
	virtual bool blocksMovement() = 0;

private:
	StudentWorld* m_world;
};


class LivingActor : public Actor {
public:
	LivingActor(StudentWorld* world, int imageID, double startX, double startY, int startDirection) :
		Actor(world, imageID, startX, startY, startDirection, 0),
		alive(true) {};

	virtual void die() { alive = false; return; };
	bool isAlive() { return alive; };
private:
	bool alive;
};

class Penelope : public LivingActor {
public:
	Penelope(StudentWorld* world, int level_x, int level_y) :
		LivingActor(world, IID_PLAYER, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y, right),
		infected(false),
		landmines(0), flames(0), vaccines(0) {};

	virtual void doSomething();
	virtual bool blocksMovement() { return true; };
private:
	void goTo(double x, double y);
	bool infected;
	int landmines, flames, vaccines, infectionCount;
};

class Wall : public Actor {
public:
	Wall(StudentWorld* world, double level_x, double level_y) :
		Actor(world, IID_WALL, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y, right, 0) {};

	virtual void doSomething() { return; };
	virtual bool blocksMovement() { return true; };
private:
};
#endif // ACTOR_H_
