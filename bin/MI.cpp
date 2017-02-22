#include<iostream>
#include<string>

using namespace std;

int main()
{
	cout << "kortefa" << endl;
	int i = 1;
	while (1) {
		string input;
		cin >> input;

		if (input == "exit") {
			break;
		}
		else if (input == "n") {
			cout << "kortefa" << i << endl;
			cout << "kortefa" << i << endl;
			i++;
		}

	}
}