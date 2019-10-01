#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// User structure
typedef struct user {
	string name;

	user* pred = 0;
	user* succ = 0;

} user;

// User list structure
typedef struct listUsers {
	
	user* head = 0;
	user* tail = 0;

} listUsers;

// Function to add user
void add (listUsers* list, string name){

	user* newUser = new user;
	newUser -> name = name;

	// Checking if list is empty
	if (list -> head == 0){
		list -> head = newUser;
		list -> tail = newUser;
		newUser -> pred = newUser;
		newUser -> succ = newUser;

	}else{
		// Changing neighbors of the tail and head
		list -> tail -> succ = newUser;
		list -> head -> pred = newUser;
		// Adding new user neighbors
		newUser -> pred = list -> tail;
		newUser -> succ = list -> head;
		// Setting the new tail
		list -> tail = newUser;
	}
	
}

void remove(listUsers* list, user* removedUser){

	// Checking if list contains only one element
	if (list -> head == list -> tail){
		list -> head = 0;
		list -> tail = 0;

	} else { 
		// Joining neighbors of the removed user
		removedUser -> succ -> pred = removedUser -> pred;
		removedUser -> pred -> succ = removedUser -> succ;

		// Checking if removed user was the head
		if (removedUser == list -> head){
            list -> head = removedUser -> succ;

		// Checking if removed user was the tail
		}else if(removedUser == list -> tail){
			list -> tail = removedUser -> pred;
		}
	}
	delete removedUser;
}

user* returnsPointer(listUsers* list, string name){
	user* current = list -> head;

	// Looking for user's pointer
	if (list -> head != 0){

		do{
			if (name == current -> name){
				// Returning found user
				return current;
			}
			current = current -> succ;
		} while (current != list -> head);

		// User not found
		return 0;

	}else{
		// Empty list
		return 0;
	}
}

int main(int argc, char* argv[]){

	// Creating the list of users
	listUsers list; 

	// Opening input file and output file
	ifstream input;
	input.open(argv[1]);
	ofstream output;
	output.open(argv[2]);

	// Variables for the file lines
	string cod;
	string name;
	string line;

	while (getline(input, line)){

		// Dividing operation code and username
		int pos = line.find(" ");
		cod = line.substr(0,pos);
		name = line.substr(pos+1);

		// Search pointer
		user* resul = returnsPointer(&list, name);

		// Identifying operations
		if (cod == "ADD"){

	    	if (resul == 0){
	    		output << "[ OK  ] ADD " << name << "\n";
	    		add(&list, name);
  
	    	}else{
	    		output << "[ERROR] ADD " << name << "\n";
	    	}

		}else if (cod == "REMOVE"){

			if (resul != 0){
				output << "[ OK  ] REMOVE "<< name << "\n";
				remove(&list, resul);

			}else{
				output << "[ERROR] REMOVE " << name << "\n";
			}

		}else if (cod == "SHOW"){

			if (resul != 0){
				output << "[ OK  ] "<< resul -> pred -> name << "<-" << resul -> name << "->" << resul -> succ -> name << "\n";

			}else{
				output << "[ERROR] ?<-" << name << "->?" << "\n";
			}
		}
	}

	//Closing input file and output file
	input.close();
	output.close();

	return 0;
}