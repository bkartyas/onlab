#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED

#include <functional>
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
	void randomize(const double max);

	~Knowledge();

	ostream& draw(ostream& os) const;
};

ostream& operator<<(ostream& os, const Knowledge &knowledge);

class Agent {
	int epoch;
	double alpha, gamma;
	Knowledge knowledge;
	Platform* platform;
	Platform* start;
	Platform* finish;

public:
	Agent(const int &x, const int &y, Platform *start, const int &epoch, const double &alpha, const double &gamma);

	void randomizeQ(const double max);
	void learn(function<void()> callAfterStep);
	void learnStep();

    void setEnd(Platform *platform);

	double step(Platform *next);
	double step(const Direction &dir);

	ostream& draw(ostream &os, const int &i, const int &j) const;
};

#endif // AGENT_H_INCLUDED
