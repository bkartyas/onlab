#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED

#include <iostream>
#include <utility>
#include "Direction.h"
#include "Vec2.h"
#include "Agent.h"

using namespace std;

class Agent;

class Platform {
protected:
	friend class Pitch;
	Pitch *pitch; 
	Vec2 position;
    double reward;
	Platform *left = nullptr;
	Platform *up = nullptr;
	Platform *right = nullptr;
	Platform *down = nullptr;

public:

	Platform(Pitch *pitch, const Vec2 &pos, const double &reward);

    virtual double getReward();
	void setReward(const double& reward);
	Vec2 getPosition();

	bool changePlatform(const char &type);
	virtual bool isChangeable() = 0;

    Platform* inDirection(const Direction &dir);
	virtual bool step(Agent *agent) = 0;

	virtual ostream& draw(ostream &os) const = 0;
};


class NormalPlatform : public Platform {
    Agent *agent = nullptr;

public:
    NormalPlatform(Pitch *pitch, const Vec2 &position, const double &reward);

	bool isChangeable();

	double getReward();
	bool step(Agent *agent);

	virtual ostream& draw(ostream &os) const;
};


class WallPlatform : public Platform {
public:
    WallPlatform(Pitch *pitch, const Vec2 &position, const double &reward);
	
	bool isChangeable();

	double getReward();
	bool step(Agent *agent);

    virtual ostream& draw(ostream &os) const;
};


class PlatformFactory{
public:
    Platform* create(Pitch *pitch, const Vec2 &pos, char type, double reward);
};

#endif // PLATFORM_H_INCLUDED
