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

string gameLogic(string word, string guess){
	string out = "";

	for(int i = 0; i < 5; i++){
		
		if(word[i] == guess[i]) out += "g";
		else if(word.find(guess[i]) < 5) out += "y";
		else out += ".";

	}

	return out;



}

map<string, double> getProbs(vector<string> &words, string actual){
	
	map<string, double> map;
	for(int i  =0; i < words.size(); i++){
		
		if(words[i] != actual) map[gameLogic(actual, words[i])] = 0;

	}
	
	for(int i = 0; i < words.size(); i++){
		
		if(words[i] != actual) map[gameLogic(actual, words[i])] += 1.0/words.size();

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
	
	
	
	//we need to calculate the probability for all words, then we need to calculate p log(p) for all the words
	//whichever value of entropy is highest is the best word. whichever has the entropy at the 25%ile and 75%ile will be used to make the tree
	string actual;
	cout << "Enter an actual word : " ; 
	cin >> actual;
	map<string, double> map = getProbs(words, actual); 
		

	double sum = 0;	
	for(auto i : map){
		
		cout << i.first << "\t" << i.second << endl;
		sum += i.second* log2(1/i.second);
	}


	cout << endl << endl << sum << endl;
	
	
	return 0;	

}



