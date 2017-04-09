#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED

#include "Platform.h"
#include "EndPlatform.h"
#include "Vec2.h"

using namespace std;

class Action {
public:
	int size = 4;
	enum type {
		stepLeft, stepUp, stepRight, stepDown,
		changeToWall, changeToPlatform
	};

};

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
	static Action action;
	int epoch;
	double alpha, gamma;
	Knowledge knowledge;
	Platform* platform;
	Platform* start;
	EndPlatform finish;

public:
	Agent(const string &id, const int &x, const int &y, Platform *start, const double &alpha, const double &gamma);

	void randomizeQ(const double max);
	bool learn();
	void learnStep();
	void stepNext();

    void setEnd(EndPlatform platform);
	void setEpoch(const int& epoch);
	bool isStartOrFinish(const Platform* platform);

	double changePlatform(const Direction &dir, const int& act);
	double step(Platform *next);
	double step(const Direction &dir);
	bool restart();

	ostream& draw(ostream &os, const int &i, const int &j) const;
	ostream& draw(ostream &os) const;
};

#endif // AGENT_H_INCLUDED
