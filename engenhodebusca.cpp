#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>

using namespace std;

typedef struct server{
	uint32_t capacity = 0;
	string historic = "";
} server;

uint32_t hash_1(uint32_t checksum, uint32_t t){
	uint32_t resul = (7919 * checksum) % t;
	return resul;
}

uint32_t hash_2(uint32_t checksum, uint32_t index, uint32_t t){
	uint32_t resul = (7919 * checksum + index * (104729 * checksum + 123)) % t;
	return resul;
}

void addHistoric(string word, server* currentServer){
	if (currentServer -> historic == ""){
		currentServer -> historic = word;
	}else{
		currentServer -> historic = currentServer -> historic + ", " + word;
	}
}

int main(int argc, char* argv[]){

    // Opening input file and output file
	ifstream input;
	input.open(argv[1]);
	ofstream output;
	output.open(argv[2]);

	uint32_t capacity, num_servers;

	input >> num_servers >> capacity;

	server* servers = new server[num_servers];

	uint32_t searches;
	input >> searches;

	uint32_t num_words;
	string word = "", new_word ="";
	uint32_t checksum = 0;
	uint32_t first_hash;
	uint32_t double_hash;
	uint32_t tamanho = 0;
	string first, last;

	for (uint32_t  i=0; i<searches;i++){
		input >> num_words;

		for (uint32_t  j=0; j<num_words;j++){
			input >> word; 
			tamanho = word.length();
			for (uint32_t  k=0; k<tamanho;k++){
				checksum = checksum ^ word[k];
			}
			if (new_word == ""){
				new_word = word;
			}else{
				new_word = new_word + " " + word;
			}
		}

		first_hash = hash_1(checksum, num_servers);
		first = "S" + to_string(first_hash);
		double_hash = first_hash;
		
		if (servers[first_hash].capacity >= capacity){
			for (uint32_t  l=1; servers[double_hash].capacity >= capacity; l++){
 				double_hash = hash_2(checksum, l, num_servers);
			}
			last = "S" + to_string(double_hash);
			output << first << "->" << last << "\n";
		}
		
		servers[double_hash].capacity += 1;
		addHistoric(new_word, &servers[double_hash]);
		output << "[S" <<  to_string(double_hash) << "] " << servers[double_hash].historic << "\n";
		
		new_word = "";
		checksum = 0;
	}

	//Closing input file and output file
	input.close();
	output.close();

	return 0;
}