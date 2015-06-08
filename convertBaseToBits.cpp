#include <iostream>

using namespace std;

int main() {
	
	// AA = 0
	// TT = 1
	// CC = 0
	// GG = 1
	// all other heterozygote = 2

	string s;
	while (cin >> s) {
		for (int i = 0; i < 86; i++) {
			if (s == "AA" || s == "CC") {
				cout << "0" << " ";
			}
			else if (s == "TT" || s == "GG") {
				cout << "1" << " ";
			}
			else {
				cout << "2" << " ";
			}

			cin >> s;
		}
		if (s == "AA" || s == "CC") {
				cout << "0" << endl;
		}
		else if (s == "TT" || s == "GG") {
			cout << "1" << endl;
		}
		else {
			cout << "2" << endl;
		}		
	}

	return 0;
}