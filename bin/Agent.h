#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED

#include "Platform.h"
#include "EndPlatform.h"
#include "Vec2.h"

using namespace std;

struct Action {
	int size = 12;
	int groupSize = 4;
	enum type {
		stepLeft, stepUp, stepRight, stepDown,
		buildLeft, buildUp, buildRight, buildDown,
		destroyLeft, destroyUp, destroyRight, destroyDown,
	};
};

class Platform;
class Agent;

class Knowledge{
	static Action action;
	int x, y;
	double ***thoughts;
	friend Agent;
public:
	Knowledge(const int& x, const int& y);
	void randomize(const double max);

	bool makesSense(const int& action, const Platform* platform) const;
	int& maxIndex(const int& x, const int& y, const int& offset, const Platform* platform) const;
	int& bestAction(const int& x, const int& y, const Platform* platform, const int& walls) const;
	int random() const;
	Direction direction(const int& a) const;

	~Knowledge();


	ostream& draw(ostream& os, const int& x, const int& y, Platform* platform, const int& walls) const;
	ostream& draw(ostream& os) const;
};

ostream& operator<<(ostream& os, const Knowledge &knowledge);

class Agent {
	string id;
	int epoch;
	double alpha, gamma;
	int built = 0;
	int numberOfWalls = 3;
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
	void newStartOrEnd(Platform* from, Platform* to);
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
