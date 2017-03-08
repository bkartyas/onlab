#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED

#include "Platform.h"
#include "Vec2.h"

using namespace std;

enum Action{ stepLeft, stepUp, stepRight, stepDown };

class Platform;
class Agent;

class Knowledge{
	int x, y, action;
	double ***thoughts;
	friend Agent;
public:
	Knowledge(const int &x, const int &y, const int &action);

	~Knowledge();

	ostream& draw(ostream& os) const;
};

ostream& operator<<(ostream& os, const Knowledge &knowledge);

class Agent {
	Knowledge knowledge;
	Platform* platform;
	Platform* start;
	Platform* finish;

public:
	Agent(const int &x, const int &y, Platform *start);

    void learn();

    void setEnd(Platform *platform);

	double step(Platform *next);
	double step(const Direction &dir);
};

#endif // AGENT_H_INCLUDED
