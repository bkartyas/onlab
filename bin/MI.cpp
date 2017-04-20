#include <iostream>
#include <fstream>
#include <windows.h>

#include "Pitch.h"

using namespace std;

int main()
{
	cout << "start" << endl;

	SYSTEMTIME st;
	GetSystemTime(&st);
	
	int sizeX, sizeY;
	cin >> sizeX >> sizeY;
	Pitch pitch(sizeX, sizeY);
	pitch.initialize();
	cout << endl;

	char buffer[256];
	sprintf_s(buffer,
		"%d.%02d.%02d-%02dh%02d'%02d.%03d",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour + 2,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds);

	string outName = "OUT_";
	outName += buffer;
	outName += ".txt";
	ofstream out(outName);
	pitch.learn([&out, &pitch]() { out << pitch << endl; /*double d; cin >> d; cout << d;*/ });

	cout << "end";
	out.close();
}
