#include <iostream>
#include <fstream>

#include "Pitch.h"

using namespace std;

int main()
{
	cout << "start" << endl;

	int sizeX, sizeY;
	cin >> sizeX >> sizeY;
	Pitch pitch(sizeX, sizeY);
	pitch.initialize();

	cout << endl;
	cout << pitch << endl;

	ofstream out("out.txt");
	pitch.learn([&out, &pitch]() { out << pitch << endl; /*double d; cin >> d; cout << d;*/ });

	cout << "end";
	out.close();
}
