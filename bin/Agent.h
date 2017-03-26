#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED

#include "Platform.h"
#include "EndPlatform.h"
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
	void randomize(const double max);

	~Knowledge();

	ostream& draw(ostream& os) const;
};

ostream& operator<<(ostream& os, const Knowledge &knowledge);

class Agent {
	string id;
	int epoch;
	double alpha, gamma;
	Knowledge knowledge;
	Platform* platform;
	Platform* start;
	EndPlatform finish;

public:
	Agent(const string &id, const int &x, const int &y, Platform *start, const int &epoch, const double &alpha, const double &gamma);

	void randomizeQ(const double max);
	bool learn();
	void learnStep();
	void stepNext();

    void setEnd(EndPlatform platform);

	double step(Platform *next);
	double step(const Direction &dir);

	ostream& draw(ostream &os, const int &i, const int &j) const;
	ostream& draw(ostream &os) const;
};

#endif // AGENT_H_INCLUDED
