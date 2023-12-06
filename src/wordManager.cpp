
#include "wordle.h"
void getWords(std::vector<std::string> &v, std::string path) {
    std::ifstream file(path);
    std::string sa;
    if (file.is_open()) {
        while (getline(file, sa)) {
            std::transform(sa.begin(), sa.end(), sa.begin(), ::toupper);
            v.push_back(sa);
            //std::cout << sa << std::endl;
        }
    }
    file.close();
}

bool checkForRepeats(std::string word, std::string guess, char letter, std::string currentOutput) {
	int count = 0;
	for(int i = 0; i < 5; i++) {
		if(word[i] == letter) {
			count++;
		}
	}
	for(int i = 0; i < 5; i++) {
		if((currentOutput[i] == 'g' || currentOutput[i] == 'y') && guess[i] == letter) {
			count--;
		}
	}
	return count > 0;
}

std::string gameLogic(std::string word, std::string guess) {
	std::string out = "";
	for (int i = 0; i < 5; i++) {
		if (word[i] == guess[i]) out += "g";
		else out += ".";
	}
	for (int i = 0; i < 5; i++) {
		if (out[i] != 'g' && checkForRepeats(word, guess, guess[i], out)) out[i] = 'y';
	}
	return out;
}

std::map<std::string, std::vector<std::string>*> getProbs(std::string actual) {
	std::map<std::string, std::vector<std::string>*> wordMap;
	for (const auto & word : *words) {
		if(word != actual) wordMap[gameLogic(actual, word)] = new std::vector<std::string>();
	}
	for (const auto & word : *words) {
		if(word != actual) wordMap[gameLogic(actual, word)]->push_back(word);
	}
	return wordMap;
}

//we need to calculate the probability for all words, then we need to calculate p log(1/p) for all the words
// whichever value of entropy is highest is the best word. whichever has the entropy at the 25%ile and 75%ile will be used to make the tree
// 1. for each word in words, sum entropy(word, other word). whichever word has the highest entropy is the word we select
// 2. guess selected word, observe the outcome, suppose it is x = '..gy.'
// 3. then in the map, for each word that had an outcome of x, guess a word in that set that had the highest entropy
// 	also save words that are in the 5%ile and 95%ile of guesses to make the tree
// 4. repeat 2-> 4 until word is found
double calculateExpectedEntropyOfAWord(std::string word){
	std::map<std::string, std::vector<std::string>*> wordMap = getProbs(word);
	double sum = 0, p;
	double actualinfo;
	for (const auto& i : wordMap) {
		p = i.second->size() / static_cast<double>(words->size());
		sum += p* log2(1/p);
		if (gameLogic(word, answer) == i.first){
			actualinfo = log2(1/p);

			std::cout << i.first << "\t";
			std::cout<<std::endl;	

			for (int j = 0; j < i.second->size(); j++) std::cout << i.second->at(j) << " ";

			words = i.second;
		}
	}
	std::cout << word << std::endl;
	std::cout << "Expected Information to gain: " << sum << std::endl; 
	std::cout << "Actual Information gained: " << actualinfo << std::endl;
	return sum;
}

//need to call this method before each of the user's guesses in order to get the best word to guess (the best word is the one with the highest entropy) 
std::map<std::string, double> getEntropyMap(){
	std::map<std::string, double> out;
	for(int i = 0; i < words->size(); i++){

		out[words->at(i)] = calculateExpectedEntropyOfAWord(words->at(i));

	}
	//..std::cout<<out << std::endl;
	return out;
}
