#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main() {
	
	srand(time(NULL));

	int numSNPs;
	int numPeople;
	//cout << "Number of SNPs? ";
	cin >> numSNPs;
	//cout << "Number of people? ";
	cin >> numPeople;

	for (int i = 0; i < numPeople; i++) {

		bool out2 = rand() % 3;
		for (int j = 0; j < numSNPs; j++) {
			
			if (out2) {
				cout << rand() % 3 << " ";
			}
			else {
				bool outNot2 = false;
				while (outNot2 == false) {
					int temp = rand() % 3;
					if (temp == 2)
						continue;
					else {
						cout << temp << " ";
						break;
					}
				}
			}
			
		}
		cout << "\n";
	}

	return 0;
}