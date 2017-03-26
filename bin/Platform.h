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
	Vec2 position;
    double reward;
	Platform *left = nullptr;
	Platform *up = nullptr;
	Platform *right = nullptr;
	Platform *down = nullptr;

public:

	Platform(const Vec2 &pos, const double &reward);

    virtual double getReward();
	void setReward(const double& reward);
	Vec2 getPosition();

    Platform* inDirection(const Direction &dir);
	virtual bool step(Agent *agent) = 0;

	virtual void draw(ostream &os) const = 0;
};


class NormalPlatform : public Platform {
    Agent *agent = nullptr;

public:
    NormalPlatform(const Vec2 &position, const double &reward);

	double getReward();
	bool step(Agent *agent);

	virtual void draw(ostream &os) const;
};


class WallPlatform : public Platform {
public:
    WallPlatform(const Vec2 &position, const double &reward);

	bool step(Agent *agent);

    virtual void draw(ostream &os) const;
};


class PlatformFactory{
public:
    Platform* create(const Vec2 &pos, char type, double reward);
};

#endif // PLATFORM_H_INCLUDED
