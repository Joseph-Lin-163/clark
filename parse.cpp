#include <iostream>
#include <string>

using namespace std;

int main() {

	string s;
	int count = 0;

	while (cin >> s) {
		if (s == "AA" || s == "AT" || s == "AC" || s == "AG" || s == "TA" || s == "TT" || s == "TC" || s == "TG" || 
				s == "CA" || s == "CT" || s == "CC" || s == "CG" || s == "GA" || s == "GT" || s == "GC" || s == "GG" ||
					s == "NN") {
			
			string temp;
			bool NNappear = false;
			for (int i = 0; i < 86; i++) {
				if (s == "NN")
					NNappear = true;
				temp += s + " ";
				cin >> s;
			}
			if (s == "NN" || NNappear) {
				continue;
			}
			else {
				temp += s;
				cout << temp << endl;	
			}			
		}
		//cout << "not done" << endl;
	}

	return 0;
}