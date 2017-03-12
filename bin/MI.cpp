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

	//while (1) {
		int sizeX, sizeY;
		cin >> sizeX >> sizeY;
		double epoch, alpha, gamma;
		cin >> epoch >> alpha >> gamma;
		Pitch pitch(sizeX, sizeY);
		Agent* agent = pitch.initialize();

		cout << endl;
		cout << pitch << endl;

		for (int i = 0; i < epoch; i++) {
			agent->learn(alpha, gamma);
			cout << pitch << endl;
		}

		cout << "end";
	//}
}
