#ifndef PITCH_H_INCLUDED
#define PITCH_H_INCLUDED

#include <iostream>
#include <vector>
#include <functional>
#include "Vec2.h"
#include "Platform.h"
#include "Agent.h"

using namespace std;

class Pitch {
	int x, y;
	Platform*** pitch;
	Platform*** pitchClone;
	vector<Agent*> agents;

	Platform*** clone();
	void link();

public:
	Pitch(const int &x, const int &y);
	~Pitch();

	vector<Agent*> initialize();

	void learn(function<void()> callAfterStep);
	void resetPitch();
	bool isStartOrFinish(const Platform* platform);

	
	Platform** operator[](const int &x);

	Vec2 getSize() const;

	ostream& draw(ostream &os) const;
};

ostream & operator<<(ostream & os, const Pitch &pitch);

#endif // PITCH_H_INCLUDED
