#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
using namespace std;


void getWords(vector<string> &v, string path){
	
	ifstream file(path);

	string sa;
	if(file.is_open()){
		while(getline(file, sa)){
			v.push_back(sa);
			
		}
	}

	file.close();



}

bool checkForRepeats(string word, string guess, char letter, string currentoutput){
	
	

	int count = 0;
	for(int i = 0; i < 5; i++){
		
		if(word[i] == letter){
			count++;
		}

	}


	for(int i = 0; i < 5; i++){
		if((currentoutput[i] == 'g' || currentoutput[i] == 'y')&& guess[i] == letter) {
			count--;
		}

	}

	return count > 0;
}


string gameLogic(string word, string guess){
	string out = "";
	for(int i = 0; i < 5; i++){
		
		if(word[i] == guess[i]) out += "g";
		else out += ".";

	}

	for(int i = 0; i < 5; i++){
		if(out[i] != 'g'&& checkForRepeats(word, guess, guess[i], out)) out[i] = 'y';
	}

	return out;
}
map<string, vector<string>*> getProbs(vector<string> &words, string actual){
	
	map<string, vector<string>*> map;
	for(int i  =0; i < words.size(); i++){
		
		if(words[i] != actual) map[gameLogic(actual, words[i])] = new vector<string>();

	}
	
	for(int i = 0; i < words.size(); i++){
		
		if(words[i] != actual) map[gameLogic(actual, words[i])]->push_back(words[i]);

	}

	return map;

}
/*map<string, double> getSortedMap(map<string, double> probs){
	
	map<string, double> out;
	vector<double> temp;
	for(auto i: probs){
		
		temp.push_back(i.second);

	}
	


	//mergesort(temp) based on the probability
	
	//do something here to make the keys correspond to the values;
	

	return out;
}
*/
int main(){


	vector<string> words;
	getWords(words, "words.txt");


	for(int i = 0; i < 10; i++){
		// cout << words[i] << endl;
	}
	
	
	
	//we need to calculate the probability for all words, then we need to calculate p log(1/p) for all the words
	//whichever value of entropy is highest is the best word. whichever has the entropy at the 25%ile and 75%ile will be used to make the tree
	// 1. for each word in words, sum entropy(word, other word). whichever word has the highest entropy is the word we select
	// 2. guess selected word, observe the outcome, suppose it is x = '..gy.'
	// 3. then in the map, for each word that had an outcome of x, guess a word in that set that had the highest entropy
	// 	also save words that are in the 5%ile and 95%ile of guesses to make the tree
	// 4. repeat 2-> 4 until word is found 
	string actual;
	cout << "Enter an actual word : " ; 
	cin >> actual;
	//need to sort this based on length of vector<string>*
	map<string, vector<string>*> map = getProbs(words, actual); 
		

	double sum = 0, p;	
	for(auto i : map){
		
		//cout << i.first << "\t" << i.second << endl;
		p = i.second->size() / static_cast<double>(words.size());
		sum += p* log2(1/p);
		
		cout << i.first << "\t";	
		for(const auto& element: *i.second) cout << element << " ";
		cout << endl;	
	}


	cout << endl << endl << sum << endl;
	//cout << gameLogic("tests", actual) << endl;	
	
	return 0;	

}



