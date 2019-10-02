#include <iostream>
#include <string>
#include <fstream>

using namespace std;

typedef struct file{
	string name = "";
	int size = 0 ;
	string hash = "";
}file;

typedef struct node{
	file* files;
 	node** children;
 	int sizeUsed = 0;
 	node* dad = 0;
 	int pos = -1;
}node;

typedef struct bTree{
 	int order;
 	node* root = nullptr;
}bTree;

void printResul(node* resul, string search, ofstream* output){
	*output << "[" << search << "]\n";
	if (resul != nullptr){
		for (int i=0; i<resul->sizeUsed; i++){
			*output << resul->files[i].name << ":size=" << resul->files[i].size << ",hash=" << resul->files[i].hash << "\n";
		}
	}else{
		*output << "-\n";
	}
}

node* searchPosition(node* root, string search){
	node* resul = nullptr;
	if (root != 0){
		int i = 0;
		while(i < root->sizeUsed && search.compare(root->files[i].hash) >= 0){
			i++;
		}
		if (root->children[0] == 0){
			resul = root;
		}else{
			resul = searchPosition(root->children[i], search);
		}
	}	
	return resul;
}

node* searchNode(node* root, string search){
	node* resul = nullptr;

	if(root!=0){
		int i = 0;
		while(i < root->sizeUsed && search.compare(root->files[i].hash) > 0){
			i++;
		}
		if (i < root->sizeUsed && search == root->files[i].hash){
			resul = root;
		}else{
			resul = searchNode(root->children[i], search);
		}
	}	
	return resul;
}

node* join(node* current){
	node* dad = current->dad;
	file newFile = current->files[0];
	file auxFile;
	node* auxNode = new node;
	node* newNode = new node;

	int pos = current->pos;
	int i = pos;

	if (dad!=0){
		//Ordenando os arquivos no nó
		do{
			auxFile = dad->files[i];
			dad->files[i] = newFile;
			i++;
			newFile = auxFile;
		}while(i<=dad->sizeUsed);
		//Alterando a ordem dos filhos
		i = pos + 1;
		if (i <= dad->sizeUsed){
			newNode = dad->children[i];
			do{
				i++;
				auxNode = dad->children[i];
				newNode->pos = i;
				dad->children[i] = newNode;
				newNode = auxNode;
			}while(i<=(dad->sizeUsed));
		}
		//Incrementando o tamanho do nó pai
		i = pos;
		dad->sizeUsed += 1;
		//Mudando os filhos e setando os pais certos
		dad->children[i] = current->children[0];
		dad->children[i]->dad = dad;
		dad->children[i]->pos = i;
		dad->children[i+1] = current->children[1];
		dad->children[i+1]->dad = dad;
		dad->children[i+1]->pos = i+1;

	}
	return current->dad;
}

int sortNode(bTree* tree, node* current, file newFile){
	int i=0;
	file auxFile;

	for(i=0;i<=tree->order;i++){
		if(((newFile.hash).compare(current->files[i].hash) <= 0) || i==current->sizeUsed){
			break;
		}
	}
	if (i==(current->sizeUsed)){
		current->files[i] = newFile;
	}else if (i<(current->sizeUsed)){
		do{
			auxFile = current->files[i];
			current->files[i] = newFile;
			i++;
			newFile = auxFile;
		}while(i<=current->sizeUsed);
			
	}
	current->sizeUsed += 1;
	if(current->sizeUsed==tree->order){
		return 1;
	}
	return 0;
}

void splitNode(bTree* tree, node* current){
	int i = 0;
	int order = tree->order;

	file* files = new file[order];
	node** children = new node*[tree->order+1];
	for (int i=0; i<=tree->order; i++){
			children[i] = 0;
		}
	node* y = new node;
	y->files = files;
	y->children = children;

	file* files2 = new file[order];
	node** children2 = new node*[tree->order+1];
	for (int i=0; i<=tree->order; i++){
			children2[i] = 0;
		}

	node* z = new node;
	z->files = files2;
	z->children = children2;

	for (i=0;i<order/2;i++){
		y->files[i] = current->files[i];
		y->children[i] = current->children[i];
		if (y->children[i]!=0){
			y->children[i]->dad = y;
		}
		y->sizeUsed++;
	}
	y->children[i] = current->children[i];
	if (y->children[i]!=0){
			y->children[i]->dad = y;
	}

	for (i = (order/2) + 1; i<current->sizeUsed; i++){
		z->files[i-(order/2)-1] = current->files[i];
		z->children[i-(order/2)-1] = current->children[i];
		if (z->children[i-(order/2)-1]!=0){
			z->children[i-(order/2)-1]->dad = z;
		}
		if (current->children[i]!=0){
			z->children[i-(order/2)-1]->pos = i-(order/2)-1;
		}
		z->sizeUsed++;
	}
	z->children[i-(order/2)-1] = current->children[i];
	if (z->children[i-(order/2)-1]!=0){
			z->children[i-(order/2)-1]->dad = z;
		}
	if (current->children[i]!=0){
		z->children[i-(order/2)-1]->pos = i-(order/2)-1;
	}

	y->dad = current;
	y->pos = 0;
	z->dad = current;
	z->pos = 1;

	current->files[0] = current->files[order/2];
	current->children[0] = y;
	current->children[1] = z;
	current->sizeUsed = 1;

}

void addNode(bTree* tree, file newFile){
	node* search;
	int resul;
	node* auxNode;
	if (tree->root == nullptr){
		node* leaf = new node;
		file* files = new file[tree->order];
		node** children = new node*[tree->order+1];
		for (int i=0; i<=tree->order; i++){
			children[i] = 0;
		}
		leaf->files = files;
		leaf->children = children;
		leaf->files[0] = newFile;
		leaf->sizeUsed = 1;
		tree->root = leaf;
	}else{
		search = searchPosition(tree->root, newFile.hash);
		resul = sortNode(tree, search, newFile);
		if (resul == 1){
			//Dividir o nó que está cheio
			splitNode(tree, search);
			auxNode = join(search);

			if (auxNode!=0){
				while ((auxNode->sizeUsed == tree->order) && auxNode!=0){
					splitNode(tree,auxNode);
					auxNode = join(auxNode);
					if (auxNode == 0){
						break;
					}
				}
			}
		}
	}
}


int main(int argc, char* argv[]){

    // Opening input file and output file
	ifstream input;
	input.open(argv[1]);
	ofstream output;
	output.open(argv[2]);

	bTree* tree = new bTree;

	string name;
	string hash;
	int size, num;
	node* resul;

	//Lendo ordem da árvore
	input >> num;
	tree->order = num;

	//Lendo número de adições
	input >> num;
	for(int i=0; i<num; i++){
		input >> name >> size >> hash;
		//Criando file 
		file* newFile = new file;
		newFile->name = name;
		newFile->size = size;
		newFile->hash = hash;
		//Adicionando arquivo ao nó
		addNode(tree, *newFile);
	}

	//Lendo número de operações
	input >> num;
	string select, search;

	for(int i=0; i<num; i++){
		input >> select;
		if (select == "INSERT"){
			input >> name >> size >> hash;
			//Criando file
			file* newFile = new file;
			newFile->name = name;
			newFile->size = size;
			newFile->hash = hash;
			//Adicionando arquivo ao nó
			addNode(tree, *newFile);
		}else{
			input >> search;
			resul = searchNode(tree->root, search);
			printResul(resul, search, &output);
		}
	}
	//Closing input file and output file
	input.close();
	output.close();

	return 0;
}