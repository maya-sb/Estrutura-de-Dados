#include <iostream>
#include <string>
#include <fstream>

using namespace std;

typedef struct node {
	string name = "";
	string type = "";
	string size = "";
	unsigned int index = 0;

	node* left = 0;
	node* right = 0;

}node;

typedef struct binaryTree {
	node* root = 0;

}binaryTree;

node* searchPosition(binaryTree* tree, string name){
	node* current = tree -> root;
	node* next = 0;

	int comp;

	do {
		comp = name.compare(current->name);

		if (comp > 0){
			next = current -> right; 
		}else{
			next = current -> left;
		}

		if (next == 0){
			return current;
		}else{
			current = next;
		}

	}while(next!=0);

	//cout << "Algo errado 1" << endl;
	return 0;
}

node* searchNode(binaryTree* tree, string name){
	node* current = tree -> root;

	int comp;

	if (current != 0){
		do {

		comp = name.compare(current->name);

		if (comp > 0 ){
			current = current -> right;
		}else if(comp < -0){	
			current = current -> left;
		}else{
			return current;
		}

		}while(current!=0);
	
	}
	//else{
	//	return 0;
	//}
	//cout << "Algo errado" << endl;
	return 0;

}
void addNode(binaryTree* tree, string name, string type, string size, int index){
	node* search;
	node* newNode = new node;
	newNode -> name = name;
	newNode -> type = type;
	newNode -> size = size;
	newNode -> index = index;
	if (index==0){
		tree -> root = newNode;
		//cout << "Passei" << endl;
	}else{
		search = searchNode(tree,name);
		//cout << "Busquei" << endl;
		if (search != 0){
			//cout << "Repetido" << endl;
			if(search -> type == "rw"){
				search -> type = type;
				search -> size = size;
				search -> index = index;
			}
			delete newNode;
		}else{
			search = searchPosition(tree,name);
			//cout << "Deu" << endl;
			if (name.compare(search -> name) > 0){
				search -> right = newNode;
			}else{
				search -> left = newNode;
			}
		}
	}
	//cout << "Saí" << endl;

}

void printEPD(node* root, ofstream* output){
	 if (root != 0){
	 	printEPD(root -> left, output);
	 	*output << root-> index << " ";
	 	*output << root -> name << " ";
	 	*output << root -> type << " ";
	 	*output << root -> size << " ";
	 	if (root -> size == "1"){
	 		*output << "byte\n"; 
	 	}else{
	 		*output << "bytes\n";
	 	}
	 	printEPD(root -> right, output);
	 }
}

void printPED(node* root, ofstream* output){
	if (root != 0){
		*output << root-> index << " ";
	 	*output << root -> name << " ";
	 	*output << root -> type << " ";
	 	*output << root -> size << " ";
	 	if (root -> size == "1"){
	 		*output << "byte\n"; 
	 	}else{
	 		*output << "bytes\n";
	 	}
	 	printPED(root -> left, output);
	 	printPED(root -> right, output);
	 }
}

void printEDP(node* root, ofstream* output){
	if (root != 0){
		printEDP(root -> left, output);
	 	printEDP(root -> right, output);
		*output << root-> index << " ";
	 	*output << root -> name << " ";
	 	*output << root -> type << " ";
	 	*output << root -> size << " ";
	 	if (root -> size == "1"){
	 		*output << "byte\n"; 
	 	}else{
	 		*output << "bytes\n";
	 	}
	 }
}


int main(int argc, char* argv[]){

    // Opening input file and output file
	ifstream input;
	input.open(argv[1]);
	ofstream output;
	output.open(argv[2]);

	binaryTree* tree = new binaryTree;

	string name;
	string type;
	string size;
	string line;

	int num;
	input >> num;

	for(int i=0; i<num; i++){
		input >> name;
		input >> type;
		input >> size;

		//cout << name << " " << type << " " << size << endl;
		
		addNode(tree,name,type,size,i);

	}

	output << "EPD:" << endl;
	printEPD(tree->root,&output);
	output << "PED:" << endl;
	printPED(tree->root,&output);
	output << "EDP:" << endl;
	printEDP(tree->root,&output);

	//Closing input file and output file
	input.close();
	output.close();

	return 0;
}