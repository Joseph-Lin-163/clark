#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct node {
	vector<string> genotypes;
	vector<string> haplotypes;
	bool dead;

	// genotypes.size() gets the number of genotypes left
	// haplotypes.size() gets the number of haplotypes found
	// Goal: Make genotypes 0 and haplotypes minimal
};

void advance(int num, vector<node> &vec) {
	/*
		Take first genotype
		Look for haplotypes to match (two haplotypes that phase it)
		If can't find two haplotypes to phase the genotype
			Collect all haplotypes that match (suppose m haplotypes match)
			Create m new nodes
				Use a different haplotype to create a new haplotype to match the 
				genotype at each node
	*/

	node n = vec[num];

	if (n.dead == true)
		return;

	vector<string> potential;
	vector<string> possiblegen;

	int i; // Genotype index that we should phase

	// go through genotypes one at a time
	for (i = 0; i < n.genotypes.size(); i++) {
		// go through haplotypes one at a time
		for (int j = 0; j < n.haplotypes.size(); j++) {
			// see if haplotypes can potentially match genotype
			for (int k = 0; k < n.haplotypes[j].length(); k++) {
				if (n.genotypes[i][k] == '2') 
					continue;

				if (n.haplotypes[j][k] != n.genotypes[i][k])
					break;

				if (k == n.haplotypes[j].length() - 1) {
					potential.push_back(n.haplotypes[j]);
				}
			}
		}
		if (potential.size() != 0)
			break;
	}

	if (potential.size() == 0) {
		n.dead = true;
		return;
	}

	// Just create the haplotype and move on
	if (potential.size() == 1) {
		string s;
		for (int j = 0; j < n.genotypes[i].size(); j++) {
			if (n.genotypes[i][j] == '2' && potential[0][j] == '1') {
				s += '0';
			}
			else if (n.genotypes[i][j] == '2' && potential[0][j] == '0') {
				s += '1';				
			}

			if (n.genotypes[i][j] != '2') {
				s += n.genotypes[i][j];				
			}
		}
		cout << s << endl;
		cout << "\n";
		n.haplotypes.push_back(s);
		n.genotypes.erase(n.genotypes.begin()+i);
		return;
	}

	// Populate possiblegen
	int iter = 0;
	while (possiblegen.size() < ((potential.size() * (potential.size()-1))/2) ) {
		for (int j = iter + 1; j < potential.size(); j++) {
			string s;
			for (int k = 0; k < potential[iter].length(); k++) {
				if (potential[iter][k] == potential[j][k])
					s += potential[iter][k];
				else
					s += '2';
			}
			possiblegen.push_back(s);
		}
		if (iter < potential.size() - 1)
			iter++;

		//cout << possiblegen.size() << endl;
	}

	// cout << "Possible haplotypes" << endl;
	// for (int j = 0; j < potential.size(); j++) {
	// 	cout << potential[j] << endl;
	// }

	// cout << "Possible genotypes" << endl;
	// for (int j = 0; j < possiblegen.size(); j++) {
	// 	cout << possiblegen[j] << endl;
	// }

	// Check to see if two haplotypes can phase it
	for (int j = 0; j < possiblegen.size(); j++) {
		if (n.genotypes[i] == possiblegen[j])
			return;
			//cout << "phased" << endl;
	}

	// Create m new nodes, m = potential.size()
	for (int k = 0; k < potential.size(); k++) {
		string s;
		// cout << n.genotypes[i] << " n genotype i " << endl;
		for (int j = 0; j < n.genotypes[i].size(); j++) {
			if (n.genotypes[i][j] == '2' && potential[k][j] == '1') {
				s += '0';
			}
			else if (n.genotypes[i][j] == '2' && potential[k][j] == '0') {
				s += '1';				
			}

			if (n.genotypes[i][j] != '2') {
				s += n.genotypes[i][j];				
			}
			// cout << "S :" << s << endl;
		}
		//cout << s << endl;
		//cout << "\n";
		// cout << "what" << endl;
		// cout << s << endl;
		node tmp;
		tmp.haplotypes = n.haplotypes;
		tmp.haplotypes.push_back(s);
		for (int h = 0; h < n.genotypes.size(); h++) {
			if (h == i)
				continue;
			string stmp = n.genotypes[h];
			tmp.genotypes.push_back(stmp);
		}
		// cout << "\n\n";
		// for (int h = 0; h < n.genotypes.size(); h++) {
		// 	cout << n.genotypes[h] << endl;
		// }
		// cout << "\n\n";

		//tmp.genotypes = n.genotypes;
		//tmp.genotypes.erase(tmp.genotypes.begin()+i);
		vec.push_back(tmp);

		// cout << n.genotypes[i] << "sanity?" << endl;
		// Could it be that doing a push_back changes the location of the vector on the stack
		// Therefore, you can't use n[0] and vec as parameters?
	}

	vec.erase(vec.begin()+i);
	return;

}

int main() {

	node sandbox;
	sandbox.genotypes.push_back("2020");
	sandbox.genotypes.push_back("1020");
	sandbox.genotypes.push_back("1220");
	sandbox.haplotypes.push_back("1010");
	sandbox.haplotypes.push_back("1000");
	vector<node> n;
	n.push_back(sandbox);
	/*
		sandbox.haplotypes.push_back("1000");
		sandbox.haplotypes.push_back("0000");
		sandbox.haplotypes.push_back("1111");
		sandbox.haplotypes.push_back("0010");
	*/
	int paramOne;
	paramOne = 0;
	advance(0,n);

	cout << "conclusion" << endl;

	for (int i = 0; i < n.size(); i++) {
		cout << "for genotypes node " << i << ": " << endl;
		for (int j = 0; j < n[i].genotypes.size(); j++) {
			cout << n[i].genotypes[j] << endl;
		}

		cout << "for haplotypes node " << i << ": " << endl;
		for (int j = 0; j < n[i].haplotypes.size(); j++) {
			cout << n[i].haplotypes[j] << endl;
		}
	}


	return 0;
}