#include <iostream>
#include <string>

#include "Vec2.h"
#include "Platform.h"
#include "Pitch.h"
#include "Agent.h"

using namespace std;

int main()
{
	cout << "start" << endl;

	int sizeX, sizeY;
	cin >> sizeX >> sizeY;
	Pitch pitch(sizeX, sizeY);
	Agent* agent = pitch.initialize();

	cout << endl;
	cout << pitch << endl;

	agent->learn([&pitch]() { cout << pitch << endl; });

	cout << "end";

	/*double d;
	cin >> d;
	cout << d;*/
}
