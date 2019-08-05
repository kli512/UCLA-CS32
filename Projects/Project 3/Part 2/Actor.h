#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

/*
Actor Hierarchy:

GraphObject	
	Actor
		Wall
		Interactable
			Exit
			Pit
			Landmine
			TimedInteractable
				Flame
				Vomit
			Goodie
				VaccineGoodie
				GasCanGoodie
				LandmineGoodie
		LivingActor
			Human
				Penelope
				Citizen
			Zombie
				DumbZombie
				SmartZombie
*/

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, int startDirection, int depth) :
		GraphObject(imageID, startX * SPRITE_WIDTH, startY * SPRITE_HEIGHT, startDirection, depth),
		m_world(world), alive(true) {};

	virtual void doSomething() = 0;
	StudentWorld* getWorld() { return m_world; };
	virtual bool blocksMovement() = 0;
	bool isAlive() { return alive; };

	virtual bool killable() { return false; };
	virtual bool lethal() { return false; };
	virtual bool landmineActivator() { return false; };
	virtual bool canExit() { return false; };
	virtual bool infectious() { return false; };
	virtual bool canBeInfected() { return false; };
	virtual bool scary() { return false; };
	virtual bool blocksFlames() { return false; };

	virtual void remove() { alive = false; };
protected:
	virtual void die() { alive = false; };

private:
	bool alive;
	StudentWorld* m_world;
};

class Wall : public Actor {
public:
	Wall(StudentWorld* world, double level_x, double level_y) :
		Actor(world, IID_WALL, level_x, level_y, right, 0) {};

	virtual void doSomething() { return; };
	virtual bool blocksMovement() { return true; };
	virtual bool blocksFlames() { return true; };
};

class Interactable : public Actor {
public:
	Interactable(StudentWorld* world, int imageID, double level_x, double level_y, int depth, int startDirection = right) :
		Actor(world, imageID, level_x, level_y, startDirection, depth) {};

	virtual bool blocksMovement() { return false; };
};

class Exit : public Interactable {
public:
	Exit(StudentWorld* world, double level_x, double level_y) :
		Interactable(world, IID_EXIT, level_x, level_y, 1) {};

	virtual void doSomething();
};

class Pit : public Interactable {
public:
	Pit(StudentWorld* world, double level_x, double level_y) :
		Interactable(world, IID_PIT, level_x, level_y, 0) {};

	virtual void doSomething() {};
	virtual bool lethal() { return true; };
};

class Landmine : public Interactable {
public:
	Landmine(StudentWorld* world, double level_x, double level_y) :
		Interactable(world, IID_LANDMINE, level_x, level_y, 1),
		safetyTicks(30), active(false) {};

	virtual void doSomething();

private:
	void explode();

	int safetyTicks;
	bool active;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class TimedInteractable : public Interactable {
public:
	TimedInteractable(StudentWorld* world, int imageID, double level_x, double level_y, int startDirection) :
		Interactable(world, imageID, level_x, level_y, 0, startDirection),
		health(2) {};

	virtual void doSomething();

private:
	int health;
};

class Flame : public TimedInteractable {
public:
	Flame(StudentWorld* world, double level_x, double level_y, int startDirection) :
		TimedInteractable(world, IID_FLAME, level_x, level_y, startDirection) {};

	virtual bool lethal() { return true; };
	virtual bool landmineActivator() { return true; };
};

class Vomit : public TimedInteractable {
public:
	Vomit(StudentWorld* world, double level_x, double level_y, int startDirection) :
		TimedInteractable(world, IID_VOMIT, level_x, level_y, startDirection) {};

	virtual bool infectious() { return true; };
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Goodie : public Interactable {
public:
	Goodie(StudentWorld* world, int imageID, double level_x, double level_y) :
		Interactable(world, imageID, level_x, level_y, 1) {};

	virtual void doSomething();

private:
	virtual void giveGoodie() = 0;
};

class VaccineGoodie : public Goodie {
public:
	VaccineGoodie(StudentWorld* world, double level_x, double level_y) :
		Goodie(world, IID_VACCINE_GOODIE, level_x, level_y) {};

private:
	virtual void giveGoodie();
};

class GasCanGoodie : public Goodie {
public:
	GasCanGoodie(StudentWorld* world, double level_x, double level_y) :
		Goodie(world, IID_GAS_CAN_GOODIE, level_x, level_y) {};

private:
	virtual void giveGoodie();
};

class LandmineGoodie : public Goodie {
public:
	LandmineGoodie(StudentWorld* world, double level_x, double level_y) :
		Goodie(world, IID_LANDMINE_GOODIE, level_x, level_y) {};

private:
	virtual void giveGoodie();
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class LivingActor : public Actor {
public:
	LivingActor(StudentWorld* world, int imageID, double startX, double startY) : // maybe need non default start direction?
		Actor(world, imageID, startX, startY, right, 0),
		paralyzed(false) {};

	virtual bool blocksMovement() { return true; };
	virtual bool killable() { return true; };
	virtual bool landmineActivator() { return true; };

protected:
	bool canMove(Direction dir);
	bool move(Direction dir);
	void newLoc(Direction dir, double& xTo, double& yTo, double dist);
	void newLoc(Direction dir, double& xTo, double& yTo);

	bool doLivingActorStuff();

	bool getParalysis() { return paralyzed; };
	void paralyze() { paralyzed = true; };
	void unparalyze() { paralyzed = false; };

private:
	virtual int myMoveDist() = 0;
	bool paralyzed;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Human : public LivingActor {
public:
	Human(StudentWorld* world, int imageID, double startX, double startY) :
		LivingActor(world, imageID, startX, startY),
		infected(false), infectionCount(0) {};

	void setInfected(bool i) { infected = i; };

	bool getInfection() { return infected; };
	void incInfectionCount() { infectionCount++; };
	int getInfectionCount() { return infectionCount; };

	virtual bool canExit() { return true; };
	virtual bool canBeInfected() { return true; };

protected:
	bool touchingVomit();

private:
	bool infected;
	int infectionCount;
};

class Penelope : public Human {
public:
	Penelope(StudentWorld* world, double level_x, double level_y) :
		Human(world, IID_PLAYER, level_x, level_y),
		landmines(0), flames(0), vaccines(0) {};

	virtual void doSomething();

	int getVaccines() { return vaccines; };
	int getFlames() { return flames; };
	int getMines() { return landmines; };

	void giveVaccines(int n) { vaccines += n; };
	void giveGasCan(int n) { flames += n; };
	void giveMines(int n) { landmines += n; };

private:
	virtual void die();

	void shootFlames();
	void addLandmine();
	void useVaccine();

	int myMoveDist() { return 4; };

	int landmines, flames, vaccines;
};

class Citizen : public Human {
public:
	Citizen(StudentWorld* world, double level_x, double level_y) :
		Human(world, IID_CITIZEN, level_x, level_y) {};

	virtual void doSomething();

private:
	virtual void die();
	int myMoveDist() { return 2; };
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Zombie : public LivingActor {
public:
	Zombie(StudentWorld* world, double startX, double startY) :
		LivingActor(world, IID_ZOMBIE, startX, startY),
		movementPlanDist(0) {};

	virtual bool scary() { return true; };

protected:
	void doZombieStuff();
	virtual void die();

	int getMovementPlanDist() { return movementPlanDist; };
	void setMovementPlanDist(int mpd) { movementPlanDist = mpd; };
	void decMovementPlanDist() { movementPlanDist--; };

private:
	int myMoveDist() { return 1; };
	int movementPlanDist;

};

class DumbZombie : public Zombie {
public:
	DumbZombie(StudentWorld* world, double startX, double startY) :
		Zombie(world, startX, startY) {};

	virtual void doSomething();

private:
	virtual void die();

};

class SmartZombie : public Zombie {
public:
	SmartZombie(StudentWorld* world, double startX, double startY) :
		Zombie(world, startX, startY) {};

	virtual void doSomething();
};

#endif // ACTOR_H_
