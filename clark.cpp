#include <iostream>
#include <vector>
#include <string> 

using namespace std;

/*
	TODO:
	Goal: Resolve with as few haplotypes as possible

	Problem formulation:
	Geno: 010100202011 | 010100101011
	Hapl: 010100101011 (Round 1)
	Hapl: 010100000011 (Round 2)
	i.e. A haplotype cannot possibly resolve a genotype if the 0's and 1's do not match
	But, if a haplotype's 0's and 1's match the genotype, then we can do the following:
		1. Look for another haplotype whose 0's and 1's match and 2's position is 
		   opposite of the first haplotype

	Idea: Keep a separate list of possibly resolvable haplotypes?

	Attempt: Clark's Algorithm
	1. Resolve unambiguous haplotypes
	2. Resolve the rest
		a. If two known haplotypes can resolve, do nothing
		b. Else, if one known haplotype + one new haplotype can resolve genotype
			i. Add new haplotype to the list
		c. No known haplotypes can resolve current genotype - skip for later round

	Greedy approach:
	1. Consider all haplotypes and see how many genotypes can be explained by 1 haplotype
		a. Add highest haplotype to the list
	2. Take list of haplotypes, increase K by 1, see now how many genotypes can be explained
	3. Continue until all genotypes explained

	(Slightly better than greedy approach)
	1. Do two haplotypes at a time instead of one

	Better than Greedy 1 & 2
	1. Remove resolvable genotypes from the list
	2. Eliminate repeats



	Case failures:
		1. No known unambiguous haplotypes. i.e. Round 1 produces 0 known haplotypes.
		2. 
*/


int main() {

	// start with round 1
	string s;
	vector<string> genotypes;
	vector<string> known;

	// get vector of genotypes
	while (getline(cin,s)) {
		genotypes.push_back(s);
	}

	// Round 1: Get known haplotypes
	for (int i = 0; i < genotypes.size(); i++) {
		bool has2 = false;
		for (int j = 0; j < genotypes[i].length(); j++) {
			if (genotypes[i][j] == '2') {
				has2 = true;
				break;
			}
		}
		if (has2 == true)
			continue;

		bool dup = false;
		for (int k = 0; k < known.size(); k++) {
			if (known[k] == genotypes[i]) {
				genotypes.erase(genotypes.begin()+i);
				i--;
				dup = true;
				break;
			}
		}

		if (dup == true)
			continue;
		else {
			known.push_back(genotypes[i]);
			genotypes.erase(genotypes.begin()+i);
			i--;
		}
	}

	// output unknowns
	/*
	for (int i = 0; i < genotypes.size(); i++) {
		cout << genotypes[i] << endl;
	}
	*/

	// if numKnown is the same size for 2 rounds in a row, we've come to a roadblock
	int prevKnown = 0;
	int currKnown = known.size();

	/* 
		If we found all haplotypes, genotypes should be empty
		Rules (applied in priority):
		1. If two known haplotypes can resolve ambiguous, erase and move on
		2. If one known haplotype can resolve the genotype, add haplotype, erase and move on
		3. If no known haplotype can resolve the genotype, skip and move on.
		4. If, for more than one round, we have the same number of number of known haplotypes, dead end
	*/

	while ((genotypes.size() != 0) && (prevKnown != currKnown)) {
		
		cout << "genotypes size = " << genotypes.size() << endl;
		cout << "known size = " << known.size() << endl;

		// Go through a round
		for (int i = 0; i < genotypes.size(); i++) {
			/* 
				The 0's and 1's of a known haplotype has to match the 0's and 1's of the selected genotype
				
				To satisfy (1), we need to have 2 known haplotypes whose 0's and 1's match
				In positions of 2's, we need to have those 2 known haplotypes differ

				To satisfy (2), we need to have 1 known haplotype whose 0's and 1's match
				In positions of 2's, invert the bit and construct a new haplotype, add to list

				To satisfy (3), we need to have 0 known haplotype whose 0's and 1's match
				Skip and move on

				Note on condition 1 & 2:
				If we have a match of 0's and 1's, that means we will either have (1) or (2).
				Why? Because we will go through all the rest of the known haplotypes looking
				a match so we could have (1), otherwise default to (2).

				Known problem with (2):
				Will stick with first haplotype where 0's and 1's match, and invert bits on 2's
			*/

			// Set condition to 1, 2, or 3
			int condition = -1;

			// set potentialHap
			string potentialHap;

			// Cycle through the known haplotypes
			for (int j = 0; j < currKnown; j++) {
				for (int k = 0; k < genotypes[i].size(); k++) {

					// If haplotype's 0's and 1's don't match genotype's 0's and 1's
					if ((known[j][k] != genotypes[i][k]) && (genotypes[i][k] != '2')) {
						break;
					}
					// potential to satisfy either (1) or (2), we have at least (2)
					else if (k == (genotypes[i].size() - 1)) {
						// Search for 2nd haplotype
						for (int h = j+1; h < currKnown; h++) {
							for (int g = 0; g < genotypes[i].size(); g++) {
								if (!((genotypes[i][g] == '2' && known[j][g] != known[h][g]) || (known[j][g] == known[h][g])))
									break;
								else if (g == (genotypes[i].size() -1)) {
									// Found a match for (1)!
									condition = 1;

									// Erase the genotype
									genotypes.erase(genotypes.begin()+i);
									i--;

									// Get out of loop
									h = currKnown;
									j = currKnown;
									break;
								}
							}
						}
						// Satisfy condition (2)!
						if (condition == -1) {
							condition = 2;
							string temp;

							// Construct a new haplotype
							for (int f = 0; f < genotypes[i].size(); f++) {
								if (genotypes[i][f] != '2') {
									temp += genotypes[i][f];
								}
								else if (known[j][f] == '1'){
									temp += '0';
								}
								else {
									temp += '1';
								}
							}
							for (int r = 0; r < known.size(); r++) {
								if (known[r] != temp && r == (known.size()-1))
									known.push_back(temp);
								else if (known[r] == temp)
									break;
							}

							// Delete genotype
							genotypes.erase(genotypes.begin()+i);

							// Break out of loop
							j = currKnown;
						}
					}
				}
			}

			if (condition == -1) {
				// We satisfy (3), no match
				// Action: Skip and move one
			}
 		}

		cout << "Previous Known = " << prevKnown << endl;
		cout << "Current Known = " << currKnown << endl;

 		prevKnown = currKnown;
 		currKnown = known.size();
	}

	if (genotypes.size() != 0) {
		cout << "Unable to resolve all genotypes." << endl;
		cout << "Previous Known = " << prevKnown << endl;
		cout << "Current Known = " << currKnown << endl;
	}

	if (genotypes.size() == 0) {
		cout << "Resolved all genotypes." << endl;

		for (int i = 0; i < known.size(); i++) {
			cout << known[i] << endl;
		}
	}


	return 0;
}
























