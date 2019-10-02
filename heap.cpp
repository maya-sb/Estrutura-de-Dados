#include <iostream>
#include <string>
#include <fstream>

using namespace std;

typedef struct heapNode{
	string cod = "";
	int hits = 0;
}heapNode;

int inNumbers(int num, int* numbers){
	for (int i=0; i<10;i++){
		if (num == numbers[i]){
			return 1;
		}
	}
	return 0;
}

void swap(heapNode* players, int current, int pos){
	string auxCod = players[pos].cod;
	int auxHits = players[pos].hits;

	players[pos].cod = players[current].cod;
	players[pos].hits = players[current].hits;
	players[current].cod = auxCod;
	players[current].hits = auxHits;
}


void heapify (heapNode* players, int length, int pos){
	int current = pos;
	int left = (2*pos) + 1;
	int right = (2*pos) + 2;

	if(left < length && players[left].hits>players[current].hits){
		current = left;
	}
	if(right < length && players[right].hits>players[current].hits){
		current = right;
	}
	if(current != pos){
		swap(players, current, pos);
		heapify(players, length, current);
	}
}

void minHeapify (heapNode* players, int length, int pos){
	int current = pos;
	int left = 2*pos + 1;
	int right = 2*pos + 2;

	if(left < length && players[left].hits<players[current].hits){
		current = left;
	}
	if(right < length && players[right].hits<players[current].hits){
		current = right;
	}
	if(current != pos){
		swap(players, current, pos);
		minHeapify(players, length, current);
	}
}

string* returnHeapMax(heapNode* players, int length, int* numWinners){
	int hitMax = players[0].hits;
	string* winners = new string[length];
	int j = 0;

	int head = hitMax;

	while(head==hitMax){
		*numWinners+=1;
		winners[j++] = players[0].cod;
		players[0] = players[length-1];
		length--;
		heapify(players, length, 0);
		head = players[0].hits;
	}
	return winners;
}

string* returnHeapMin(heapNode* players, int length, int* numWinners){
	int hitMin = players[0].hits;
	string *winners = new string[length];
	int j = 0;

	int head = hitMin;

	while(head==hitMin){
		*numWinners+=1;
		winners[j++] = players[0].cod;
		players[0] = players[length-1];
		length--;
		minHeapify(players, length, 0);
		head = players[0].hits;
	}

	return winners;
}


int main(int argc, char* argv[]){

    // Opening input file and output file
	ifstream input;
	input.open(argv[1]);
	ofstream output;
	output.open(argv[2]);

	int prize, num_bets;
	int numbers[10];
	string cod;

	input >> prize;
	input >> num_bets;

	for(int i=0; i<10;i++){
		input >> numbers[i];
	}

	heapNode* heap = new heapNode[num_bets];
	heapNode* heapMin = new heapNode[num_bets];

	int num;

	for(int i=0; i<num_bets;i++){
		input >> cod;
		heap[i].cod = cod;
		heapMin[i].cod = cod;
		for (int j=0; j<15; j++){
			input >> num;
			if (inNumbers(num, numbers) == 1){
				heap[i].hits += 1;
				heapMin[i].hits +=1;
			}
		}
	}

	//Fazendo o heap max
	for(int i = (num_bets-2)/2; i>=0; i--){
		heapify(heap, num_bets, i);
	}

	int numWinners = 0;
	string* winners;
	int maior = heap[0].hits;
	winners = returnHeapMax(heap, num_bets, &numWinners);
	output << "[" << numWinners << ":" << maior << ":" << (prize/2)/numWinners << "]\n";

	for (int i=0; i<numWinners; i++){
		output << winners[i] << "\n";
	}

	// Fazendo o heap min
	for(int i = (num_bets-2)/2; i>=0; i--){
		minHeapify(heapMin, num_bets, i);
	}

	numWinners = 0;
	string* minWinners;
	int menor = heapMin[0].hits;
	minWinners = returnHeapMin(heapMin, num_bets, &numWinners);

	output << "[" << numWinners << ":" << menor << ":" << (prize/2)/numWinners << "]\n";

	for (int i=0; i<numWinners-1; i++){
		output << minWinners[i] << "\n";
	}
	output << minWinners[numWinners-1];

	//Closing input file and output file
	input.close();
	output.close();

	return 0;
}







