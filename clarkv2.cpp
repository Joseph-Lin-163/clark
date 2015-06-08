#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

	/*
		Problem: Minimum Parsimony Formulation

		Input: List of genotypes
		Output: Fewest haplotypes that resolves genotypes

		How:
		Djikstra's algorithm provides an easy way to get the minimum number of haplotypes

		Nodes are states in the different runs of clark's algorithm
		Edges are the number of additional haplotypes added to each state in clark's algorithm

		Note: 
		It's important to recognize that, even though we do not know how the graph
		of clark's algorithm runs, a graph does exist. It may be the case that there is no
		solution that phases all genotypes (at which point, we give up because guessing is
		stupid). But, there may also be a case where all genotypes are phased and that means
		that there exists a path from start to finish.

		We have to understand that there may be "dead ends". These are the cases in clark's
		algorithm in which no known haplotypes can possibly resolve the rest of the ambiguous
		genotypes. At this point, we need to mark this node as a "dead end".

		At each run of the algorithm, we need to create a new node. Each new node will have 
		some number of known haplotypes. The "weight" of the edge from node a to node b will
		be node b's number of known haplotype minus node a's number of known haplotypes.

		Djikstra's will dictate that we select the path which has the least number of known 
		haplotypes and follow that. It may mean that we run the function that takes us from
		one node to the next quite often, but it guarantees that we are able to get the minimum
		number of known haplotyopes.

		Complexity:
		Suppose there are n number of ambiguous genotypes. That means that there are n! number
		of total orderings to begin with. This removes any factors relating to the order of the
		genotypes, which means that any genotype list will work for this algorithm. In addition,
		there's one more point of contention - the known haplotypes.

		Say, for example, we are unable to find two haplotypes to phase a genotype. However, 
		there are some m > 1 haplotypes that can phase the genotype. These haplotypes will 
		obviously produce two different results when we are constructing a new haplotype from
		the single known haplotype (otherwise, they're identical). This can affect the results
		because one haplotype may be able to combine with another known haplotype to resolve
		further genotypes while the other may not. To remedy this situation, we need to take
		all the known haplotypes that are compatible with the genotype in question and create
		m different nodes. This will successfully explore all the different possible 
		combinations that may possible arise in this problem.

		In essence, this is a brute force way to find the minimum number haplotypes necessary 
		to phase all genotypes. 

		Overall complexity in the worst case: O(n!m!)
		Accuracy: 100%

		Functions needed:
		1. Bring one state to the next
			a. Check if finish
		2. Create new nodes (initialization) ------------------------------------ Done 
		3. Create new nodes (creating new haplotype using known haplotypes) ----- Done
		4. Main should handle the Djikstra's part of the algorithm

		Lists
		1. Genotypes
		2. Known haplotypes
	*/

struct node {
	vector<string> genotypes;
	vector<string> haplotypes;
	bool dead;

	// genotypes.size() gets the number of genotypes left
	// haplotypes.size() gets the number of haplotypes found
	// Goal: Make genotypes 0 and haplotypes minimal
};

int factorial(int n) {
	if (n == 1)
		return n;
	else
		return n*factorial(n-1);
}

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

	int initialSolver1 = -1;
	for (int h = 0; h < vec[num].genotypes.size(); h++) {
		for (int j = 0; j < vec[num].haplotypes.size(); j++) {
			for (int k = 0; k < vec[num].genotypes[h].size(); k++) {
				if ((vec[num].genotypes[h][k] == '0') && (vec[num].haplotypes[j][k] == '0') && (k != vec[num].genotypes[h].size()-1))
					continue;

				if ((vec[num].genotypes[h][k] == '1') && (vec[num].haplotypes[j][k] == '1') && (k != vec[num].genotypes[h].size()-1))
					continue;

				if ((vec[num].genotypes[h][k] == '2') && (k != vec[num].genotypes[h].size()-1))
					continue;

				/*
				if (vec[num].genotypes[h][k] == 2)
					cout << "really???" << endl;

				cout << vec[num].genotypes[h][k] << endl;
				cout << vec[num].genotypes[h][k+1] << endl;
				cout << vec[num].genotypes[h][k+2] << endl;
				cout << vec[num].genotypes[h][k+3] << endl;
				cout << vec[num].genotypes[h][k+4] << endl;
				cout << vec[num].genotypes[h][k+5] << endl;
				
				if (k != vec[num].genotypes[h].size()-1)
					cout << "seriously???" << endl;
				cout << "k is: " << k << endl;
				cout << "limit is: " <<  vec[num].genotypes[h].size()-1 << endl;
				*/

				if (k != vec[num].genotypes[h].size()-1) {
					/*
					cout << "broke at these geno and haplo at position " << k << ":" << endl;
					cout << "vec[num].genotypes[h].size()-1 is " << vec[num].genotypes[h].size()-1 << endl;
					cout << vec[num].genotypes[h] << endl;
					cout << "specifically: " << vec[num].genotypes[h][k] << endl;
					cout << vec[num].haplotypes[j] << endl;
					cout << "specifically: " << vec[num].haplotypes[j][k] << endl;
					*/
					break;
				}
				else {
					/*
					cout << "potential!" << endl;
					cout << vec[num].genotypes[h] << endl;
					cout << vec[num].haplotypes[j] << endl;
					*/
					initialSolver1 = j;
					j = vec[num].haplotypes.size();
					h = vec[num].genotypes.size();
					break;
				}
			}
		}
	}

	if (initialSolver1 != -1) {

		for (int h = 0; h < vec[num].genotypes.size(); h++) {
			for (int j = 0; j < vec[num].haplotypes.size(); j++) {
				if (j == initialSolver1)
					continue;
				for (int k = 0; k < vec[num].genotypes[h].size(); k++) {
					if ((vec[num].genotypes[h][k] == '0') && (vec[num].haplotypes[j][k] == '0') && (k != vec[num].genotypes[h].size()-1))
						continue;

					if ((vec[num].genotypes[h][k] == '1') && (vec[num].haplotypes[j][k] == '1') && (k != vec[num].genotypes[h].size()-1))
						continue;

					if ((vec[num].genotypes[h][k] == '2') && (k != vec[num].genotypes[h].size()-1)) {
						if (vec[num].haplotypes[j][k] != vec[num].haplotypes[initialSolver1][k])
							continue;
						else
							break;
					}

					if (k != vec[num].genotypes[h].size()-1)
						break;
					else {
						//cout << "erasing" << endl;
						vec[num].genotypes.erase(vec[num].genotypes.begin() + h);
						return;
					}
				}
			}
		}
	}

	// go through genotypes one at a time
	for (i = 0; i < n.genotypes.size(); i++) {
		// go through haplotypes one at a time
		for (int j = 0; j < n.haplotypes.size(); j++) {
			// see if haplotypes can potentially match genotype
			for (int k = 0; k < n.haplotypes[j].length(); k++) {
				if ((n.genotypes[i][k] == '2') && !(k == n.haplotypes[j].length() - 1))
					continue;

				if ((n.haplotypes[j][k] != n.genotypes[i][k]) && !(k == n.haplotypes[j].length() - 1))
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
		
		vec[num].dead = true;
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
		// cout << s << endl;
		// cout << "\n";
		vec[num].haplotypes.push_back(s);
		vec[num].genotypes.erase(vec[num].genotypes.begin()+i);
		//cout << s << endl;
		
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
		if (n.genotypes[i] == possiblegen[j]) {
			
			return;
		}
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

int min(vector<node> vec, bool &finish) {
	// vec size must be > 0
	if (vec.size() < 1)
		return -1;

	//cout << "vec size: " << vec.size() << endl;

	int minindex = 0;
	int min = vec[0].haplotypes.size();

	if (vec[0].genotypes.size() == 0)
		finish = true;

	for (int i = 1; i < vec.size(); i++) {
		if (min > vec[i].haplotypes.size()) {
			min = vec[i].haplotypes.size();
			minindex = i;
		}
		if (vec[i].genotypes.size() == 0) {
			//cout << "finished" << endl;
			finish = true;
			break;
		}
	}

	return minindex;
}

int main() {

	string s;
	vector<string> genotypes;
	vector<string> known;
	vector<node> nodes;

	// get vector of genotypes
	while (getline(cin,s)) {
		genotypes.push_back(s);
	}

	// Get known haplotypes
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

	// Initialize the nodes
	for (int i = 0; i < factorial(genotypes.size()); i++) {
		node tmp;
		tmp.genotypes = genotypes;
		tmp.haplotypes = known;
		tmp.dead = false;
		nodes.push_back(tmp);
		std::next_permutation(genotypes.begin(), genotypes.end());
	}

	/*
		I have a list of all the different permutations of ambiguous genotypes
		I have a list of all the unambiguous genotypes converted to haplotypes

		I need to write a function that takes the problem from one step to the next
		Status: Done
	*/

	/*
		Handle Djikstra's Algorithm:

		Need to:
		Prune dead ends
			Check after running advance
		Check for genotypes.size() == 0	------------------------------------ Done
			Can be check while running genotypes
		Find minimum haplotype and run (if multiple, run first occurence) -- Done
			Find minimum every run		
	*/

	/*
	for (int i = 0; i < nodes.size(); i++) {
		cout << "for genotypes node " << i << ": " << endl;
		for (int j = 0; j < nodes[i].genotypes.size(); j++) {
			cout << nodes[i].genotypes[j] << endl;
		}

		cout << "for haplotypes node " << i << ": " << endl;
		for (int j = 0; j < nodes[i].haplotypes.size(); j++) {
			cout << nodes[i].haplotypes[j] << endl;
		}
	}
	*/

	// Djikstra's Algorithm is right here.
	bool finish = false;
	while (!finish) {
		int torun = min(nodes,finish);
		if (finish)
			break;

		advance(torun, nodes);
		if (nodes[torun].dead)
			nodes.erase(nodes.begin()+torun);				
		if (nodes.size() == 0)
			break;
	}

	if (nodes.size() == 0)
		cout << "Unable to phase all genotypes. Unwilling to guess." << endl;

	/*
		for (int i = 0; i < nodes.size(); i++) {
			if (nodes[i].dead)
				cout << "node " << i << " is dead" << endl;
			
			cout << "for genotypes node " << i << ": " << endl;
			for (int j = 0; j < nodes[i].genotypes.size(); j++) {
				cout << nodes[i].genotypes[j] << endl;
			}

			cout << "for haplotypes node " << i << ": " << endl;
			for (int j = 0; j < nodes[i].haplotypes.size(); j++) {
				cout << nodes[i].haplotypes[j] << endl;
			}
		}
		*/

	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].genotypes.size() == 0) {
			// Output Haplotypes
			//cout << "conclusion" << endl;
			for (int j = 0; j < nodes[i].haplotypes.size(); j++) {
				cout << nodes[i].haplotypes[j] << endl;
			}
			break;
		}
	}

	return 0;
}