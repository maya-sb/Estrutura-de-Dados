#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Printer structure
typedef struct printer {
	string name;
	unsigned int pags;

	string historic = "";
	string last = "";

	printer* succ = 0;
	printer* pred = 0;

} printer;

// Printer list structure
typedef struct listPrinters {
	
	printer* begin = 0; 
	printer* end = 0;

} listPrinters;

// Document structure
typedef struct document {

	string name;
	unsigned int pags;

	document* succ = 0;

} document;

// Document list structure
typedef struct stackDocuments {
	
	document* top = 0; 

} stackDocuments;

// Function to add document to stack
void addStack (stackDocuments* stack, string historic){

	document* newDocument = new document;
	newDocument -> name = historic;
	newDocument -> succ = stack -> top;
	stack -> top = newDocument;

}

// Function to add printer
void addPrinter(listPrinters* list, string name){

	printer* newPrinter = new printer;
	newPrinter -> name = name;

	// Checking if list is empty
	if (list -> begin == 0){
		list -> begin = newPrinter;
		list -> end = newPrinter;
	}else{
		list -> end -> succ = newPrinter;
		newPrinter -> pred = list -> end;
		list -> end = newPrinter;
	}
}

void removePrinter(listPrinters* list, printer* removedPrinter){

	// Checking if list contains only one element
	if (list -> begin == list -> end){
		list -> begin = 0;
		list -> end = 0;
	}else {

		// Checking if removed printer was the begin
		if (list -> begin == removedPrinter){
			list -> begin = list -> begin -> succ;
			removedPrinter -> succ -> pred = 0;
		// Checking if removed printer was the end
		} else if (list -> end == removedPrinter){
			removedPrinter -> pred -> succ = 0;
			list -> end = removedPrinter -> pred;
		} else {
			removedPrinter -> succ -> pred = removedPrinter -> pred;
			removedPrinter -> pred -> succ = removedPrinter -> succ;
		}
	}
}

//Function to add document to printer
void addDocument(printer* printer, string name, unsigned int pags){

	printer -> pags += pags;
	string newHistoric = name + "-" + to_string(pags) + "p";
	
	if (printer -> historic == ""){
		printer -> historic = newHistoric;
		printer -> last = newHistoric;
	}else{
		printer -> last = newHistoric;
		printer -> historic = newHistoric + ", " + printer -> historic;
	}

}

int main(int argc, char* argv[]){

    // Opening input file and output file
	ifstream input;
	input.open(argv[1]);
	ofstream output;
	output.open(argv[2]);

	// Creating the list of printers and stack of documents
	listPrinters list; 
	stackDocuments stack;

	// Variables for the file lines
	int num = 0;
	string name = "";
	int total_pags = 0;

	// Reading the number of printers
	input >> num;
	input.ignore();

	// Adding printers
	for (int i=0; i<num;i++){
		getline(input,name);
		addPrinter(&list, name);
	}
	
	//Reading the number of documents
	input >> num;
	input.ignore();

	// Variables of control
	unsigned int pags = 0, smaller = 0, new_smaller = 0, cont = 0;
	printer* current = list.begin;

	// Initializing printers
	while(num!=0 && current!=0){
		input >> name >> pags;
		total_pags+=pags;

		addDocument(current,name,pags);
		output << "[" << current -> name << "] " << current -> historic << "\n";

		current = current -> succ;
		num--;

		//Calculating the lowest number of pages
		if (cont==0){
			smaller = pags;
		}

		if (pags < smaller){
			smaller = pags;
		}
		cont++;
	}

	// Adding the rest of the documents
	while(num!=0){

		current = list.begin;
		cont = 0;

		while (current!=0){

			current -> pags -= smaller;
		
			if (current -> pags == 0){
				//Identifying end of print
				addStack(&stack, current -> last);

				if(num!=0){
					input >> name >> pags;
					total_pags+=pags;
					addDocument(current, name, pags);
					output << "[" << current -> name << "] " << current -> historic << "\n";
					num--;
				}	
			}

			if (current -> pags == 0){
				removePrinter(&list, current);
			}

			//Calculating the lowest number of pages
			if(cont == 0){
				new_smaller = current -> pags;
				cont++;
			}
			if (current -> pags < new_smaller){
				new_smaller = current -> pags;
			}

			current = current -> succ;
		} 
		smaller = new_smaller;
	}

	//Identifying end of print
	while (list.begin != 0){

		current = list.begin;
		smaller = new_smaller;
		cont = 0;

		while (current!=0){
			current -> pags -= smaller;

			if (current -> pags == 0){
				addStack(&stack, current -> last);
				removePrinter(&list, current);

			}else{
				//Calculating the lowest number of pages
				if (cont == 0){
					new_smaller = current -> pags;
					cont++;
				}
				if (current -> pags < new_smaller){
					new_smaller = current -> pags;
				}
			}
			current = current -> succ;
		}
	}

	// Printing the stack
	output << total_pags << "p\n";
	document* currentDoc = stack.top;
	while (currentDoc != 0){
		output << currentDoc -> name << "\n";
		currentDoc = currentDoc -> succ;
	}

	//Closing input file and output file
	input.close();
	output.close();

	return 0;
}