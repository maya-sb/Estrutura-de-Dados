#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

typedef struct word{
	string name;
	string synonyms;
	int balancing;
	
	word* left = nullptr;
	word* right = nullptr;
}word;

typedef struct dictionary{
	word* root = nullptr;
}dictionary;

word* searchPosition(dictionary* tree, string name){
	word* current = tree -> root;
	word* next = nullptr;

	int comp;

	do {
		comp = name.compare(current->name);

		if (comp > 0){
			next = current -> right; 
		}else{
			next = current -> left;
		}

		if (next == nullptr){
			return current;
		}else{
			current = next;
		}

	}while(next!=nullptr);

	//cout << "Algo errado 1" << endl;
	return 0;

}

void addWord(word* node, dictionary* tree){
	
	word* search;

	if (tree -> root == nullptr){
		tree -> root = node;
		//cout << "Primeiro" << endl;
	}else{
		search = searchPosition(tree,node -> name);
		//cout << "Meh" << endl;
 		if (node -> name.compare(search -> name)>0){
			search -> right = node;
		}else{
			search -> left = node;
		}
	}
}


void searchWord(string name, dictionary* tree, ofstream* output){
	word* current = tree -> root;
	word* next = nullptr;

	int comp;

		*output << "[";

	do {
		comp = name.compare(current->name);

		*output << current -> name;

		if (comp > 0 ){
			*output << "->";
			next = current -> right; 
		}else if(comp < 0){
			*output << "->";
			next = current -> left;
		}else if (comp == 0){
			*output << "]\n";
			*output << current -> synonyms << "\n";
			break;
		}

		if (next == nullptr){
			*output << "?]\n-\n";
			break;
		}else{
			current = next;
		}

	}while(next!=nullptr);

}

void addBalancing(word* root, int left, int right){
	int b = abs(left-right);
	if (left > right){
		root -> balancing = -b;
	}else{
		root -> balancing = b;
	}
}

word* rotateLeft(word** r){
	word* root = (*r);
	word* eixo = root -> right;
	root -> right = eixo -> left;
 	eixo -> left = root;
 	root = eixo;
 	(*r) = root;
 	return *r;
}

word* rotateRight (word** r){
	word* root = *r;
	word* eixo = root -> left;
	root -> left = eixo -> right;
	eixo -> right = root;
	root = eixo;
	(*r) = root;
	return *r;
}

word* rotateLeftRight(word** r){
	word* root = (*r);
	word* new_root;
	new_root = rotateLeft(&(root->left));
	new_root = rotateRight(&root);
	(*r) = root;
	return new_root;
}

word* rotateRightLeft(word** r){
	word* root = (*r);
	word* new_root;
	new_root = rotateRight(&(root->right));
	new_root = rotateLeft(&root);
	(*r) = root;
	return new_root;
}

word* rotate(word** root){
	word* new_root;

	if ((*root)->balancing>0){
		if ((*root)->right->balancing>=0){
			new_root = rotateLeft(root);
		}else{
			new_root = rotateRightLeft(root);
		}
	}else{
		if ((*root)->left->balancing>0){
			new_root = rotateLeftRight(root);
		}else{
			new_root = rotateRight(root);
		}
	}
	return new_root;
}

int balance(word** root){
	if (root==nullptr){
		return 0;
	}else{ 
		int left, right;

		if ((*root)->left == nullptr){
			left = 0;
		}else{
			left = 1 + balance(&((*root)->left));
		}

		if ((*root)->right == nullptr){
			right = 0;
		}else{
			right = 1 + balance(&((*root)->right));
		}		

		if(abs(left - right)<2){
			//cout << "Adicionar balanceamento" << endl;
			if (left>=right){
				addBalancing(*root, left, right);
				return left;
			}else{
				addBalancing(*root, left, right);
				return right;
			}
		}else{
			//cout << "Rotacionar" << endl;
			word* new_root;
			addBalancing(*root, left, right);
			new_root = rotate(root);
			//cout << "Rotacionei" << endl;
			return balance(&new_root);
		}
		
	}
}

int main(int argc, char* argv[]){

	ifstream input;
	input.open(argv[1]);
	ofstream output;
	output.open(argv[2]);
	
	dictionary* tree = new dictionary;
	
	int numWords;
	input >> numWords;
	
	cout << numWords << endl;
	string name, synonymous;
	int size;

	
	for(int i=0; i<numWords;i++){
		input >> name;
		input >> size;
		string synonyms = "";
		input >> synonymous; 
		synonyms += synonymous;
		for (int j=1; j<size; j++){
			input >> synonymous; 
			synonymous = "," + synonymous;
			synonyms += synonymous;
		} 
		cout << name << " " << size << " " << synonyms << endl;
		word* vocable = new word;
		vocable -> name = name;
		vocable -> synonyms = synonyms;
		addWord(vocable, tree);
		//balance(&(tree -> root));
	}
	
	numWords = 0;
	input >> numWords;
	cout << numWords << endl;
	for (int i=0; i<numWords; i++){
		input >> name;
		cout << name << endl;
		searchWord(name, tree, &output);
	}
	
	input.close();
	output.close();

	return 0;
}
