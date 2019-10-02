#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using std::stringstream;
using namespace std;

struct queue;

typedef struct agencie{
	string name = "";
	int numClerk = 0;
	int totalPeople = 0;

	agencie* pred = nullptr;
	agencie* succ = nullptr;
	queue* priorityPeople;
	queue* regularPeople;
}agencie;

typedef struct listAgencies{
	agencie* head = nullptr;
	agencie* tail = nullptr;
	int numAgencies = 0;

	void add (agencie* newAgencie){
		// Checking if list is empty
		if (head == nullptr){
			head = newAgencie;
			tail = newAgencie;
			newAgencie->pred = newAgencie;
			newAgencie->succ = newAgencie;
		}else{
			// Changing neighbors of the tail and head
			tail->succ = newAgencie;
			head->pred = newAgencie;
			// Adding new user neighbors
			newAgencie->pred = tail;
			newAgencie->succ = head;
			// Setting the new tail
			tail = newAgencie;
		}
		numAgencies+=1;
	}

	agencie* search (string name){
		agencie* current = tail;

		do {
			current = current->succ;
			if (current->name == name){
				return current;
			}
		}while (current->succ != head);

		return nullptr;
	}

	void remove (agencie* oldAgencie){
	// Checking if list contains only one element
		if (head == tail){
			head = nullptr;
			tail = nullptr;

		} else { 
			// Joining neighbors of the removed agencie
			oldAgencie->succ->pred = oldAgencie->pred;
			oldAgencie->pred->succ = oldAgencie->succ;

			// Checking if removed agencie was the head
			if (oldAgencie == head){
	            head = oldAgencie->succ;

			// Checking if removed agencie was the tail
			}else if(oldAgencie == tail){
				tail = oldAgencie->pred;
			}
		}
		numAgencies-=1;
	}

}listAgencies;

typedef struct person{
	string name;

	person* pred = nullptr;
	person* succ = nullptr;
}person;

typedef struct queue{
	person* head = nullptr;
	person* tail = nullptr;
	int numPeople = 0;

	void add(person* newPerson){
		// Checking if list is empty
		if (head == nullptr){
			head = newPerson;
			tail = newPerson;
			newPerson->pred = newPerson;
			newPerson->succ = newPerson;
		}else{
			tail->succ = newPerson;
			head->pred = newPerson;
			// Adding new person neighbors
			newPerson->pred = tail;
			newPerson->succ = head;
			// Setting the new tail
			tail = newPerson;
		}

		numPeople+=1;
	}

	void remove(){
		if (head == tail){
			head = nullptr;
			tail = nullptr;
		}else{
			head->pred->succ = head->succ;
			head->succ->pred = head->pred;
			head = head->succ;
		} 
		numPeople-=1;
	}

}queue;

int main(int argc, char* argv[]){

    // Opening input file and output file
	ifstream input;
	input.open(argv[1]);
	ofstream output;
	output.open(argv[2]);

	listAgencies agencies;

	string line;
	int numAgencies, pos;

	getline(input, line);
	numAgencies = stoi(line);

	string name;
	int clerks, numPeople;

	for (int i=0; i<numAgencies; i++){
		agencie* newAgencie = new agencie;

		getline(input, line);

		pos = line.find(" ");
		name = line.substr(0,pos);
		clerks = stoi(line.substr(pos+1));

		newAgencie->name = name;
		newAgencie->numClerk = clerks;
		newAgencie->priorityPeople = new queue;
		newAgencie->regularPeople = new queue;
		agencies.add(newAgencie);
	}

	getline(input, line);
	numPeople = stoi(line);

	string agencieName, namePerson;
	int age;
	string age1;
	agencie* auxAgencie = new agencie;


	for(int i=0; i<numPeople; i++){
		getline(input,line);
		person* newPerson = new person;

		pos = line.find("|");
		agencieName = line.substr(0,pos);
		line = line.substr(pos+1);
		pos = line.find("|");
		namePerson = line.substr(0,pos);
		age1 = line.substr(pos+1);
		age = stoi(age1);
		newPerson->name = namePerson;
		auxAgencie = agencies.search(agencieName);

		if (age>=60){
			auxAgencie->priorityPeople->add(newPerson);
		}else{
			auxAgencie->regularPeople->add(newPerson);
		}
	}
	
	string namePrint;
	auxAgencie = agencies.head;
	int pessoasFila;

	while(agencies.head != nullptr){

		output << auxAgencie->name << ":";

		pessoasFila = auxAgencie->priorityPeople->numPeople + auxAgencie->regularPeople->numPeople;

		for (int j=0; j<auxAgencie->numClerk;j++){

			if (auxAgencie->priorityPeople->numPeople>0){
				namePrint = auxAgencie->priorityPeople->head->name;
				output << namePrint;
				if ((j+1)!=auxAgencie->numClerk && (j+1)<pessoasFila){
					output << ",";
				}

				auxAgencie->priorityPeople->remove();
			}else if (auxAgencie->regularPeople->numPeople>0){
				namePrint = auxAgencie->regularPeople->head->name;
				output << namePrint;
				if ((j+1)!=auxAgencie->numClerk && (j+1)<pessoasFila){
					output << ",";
				}
				auxAgencie->regularPeople->remove();
			}

			pessoasFila--;
			if (pessoasFila == 0){
				auxAgencie = auxAgencie->pred;
				agencies.remove(auxAgencie->succ);
				break;
			}
		
		}
		output << endl;
		if (auxAgencie != nullptr){
			auxAgencie = auxAgencie->succ;
		}
	}
	//Closing input file and output file
	input.close();
	output.close();

	return 0;
}
