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
 	int leaf = 0;
 	node* right = nullptr;
 	node* left = nullptr;
}node;

typedef struct bTree{
 	int order;
 	node* root = nullptr;
}bTree;

typedef struct nodes{
	node** resul;
	int size = 0;
}nodes;

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

void percorrerLista(node* search){
	while(search != nullptr){
		for(int i=0; i<search->sizeUsed; i++){
			cout << search->files[i].hash << endl;
		}
		search = search->right;
	}
}

void printResulRange(node* resul, ofstream* output, string range2){
	while (resul != nullptr && ((resul->files[0].hash).compare(range2) <= 0)){
		for (int i=0; i<resul->sizeUsed; i++){
			*output << resul->files[i].name << ":size=" << resul->files[i].size << ",hash=" << resul->files[i].hash << "\n";
		}
		resul = resul->right;
	}
}

void searchRange(node* resul, string range1, string range2, ofstream* output){
	int cont = 0;
	if (resul != nullptr && ((resul->files[0].hash).compare(range1) <=0 || (resul->files[0].hash).compare(range2) <= 0)){
		for (int i=0; i<resul->sizeUsed; i++){
			if (((resul->files[i].hash).compare(range1) >= 0) && ((resul->files[i].hash).compare(range2) <=0 )){
				printResulRange(resul, output, range2);
				cont++;
				break;
			}
		}
		if (cont==0){
			searchRange(resul->right, range1, range2, output);
			//*output << "-\n";
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
		//if (root->children[0] == 0){
		if (root->leaf == 1){
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
		//if (search == root->files[i].hash && root->children[0] == 0){
		if (search == root->files[i].hash && root->leaf == 1 && i < root->sizeUsed){
			resul = root;
		}else{
			if (search.compare(root->files[i].hash) == 0){
				resul = searchNode(root->children[++i], search);
			}else{
				resul = searchNode(root->children[i], search);
			}
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


	//Primeiro nó
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

	//Segundo nó
	if (current->leaf == 1){
		//Divisão de um nó folha
		for (i = (order/2); i<current->sizeUsed; i++){
			z->files[i-(order/2)] = current->files[i];
			z->children[i-(order/2)] = current->children[i];
			if (z->children[i-(order/2)]!=0){
				z->children[i-(order/2)]->dad = z;
			}
			if (current->children[i]!=0){
				z->children[i-(order/2)]->pos = i-(order/2);
			}
			z->sizeUsed++;
		}
		z->children[i-(order/2)] = current->children[i];
		if (z->children[i-(order/2)]!=0){
				z->children[i-(order/2)]->dad = z;
			}
		if (current->children[i]!=0){
			z->children[i-(order/2)]->pos = i-(order/2);
		}

		//Definindo como folha
		y->leaf = 1;
		z->leaf = 1;
		//Definindo como não folha
		current->leaf = 0;
		//Mudando os ponteiros da lista encadeada
		y->right = z;
		z->left = y;
		if(current->right!=nullptr){
			current->right->left = z;
			z->right = current->right;
		}
		if (current->left!=nullptr){
			current->left->right = y;
			y->left = current->left;
		}

	}else{
		//Divisão de um nó não folha
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
		leaf->leaf = 1;
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
	string select, search, select2, range1, range2;

	for(int i=0; i<num; i++){
		input >> select >> select2;
		if (select2 == "RANGE"){
			//Busca por intervalo
			input >> range1 >> range2;
			if (range1.compare(range2) <= 0){
				output << "[" << range1 << "," << range2 << "]\n";
			}else{
				output << "[" << range2 << "," << range1 << "]\n";	
				string aux = range1;
				range1 = range2;
				range2 = aux;
			}
			
			resul = searchPosition(tree->root, range1);
			searchRange(resul, range1, range2, &output);
		}else{
			if (select == "INSERT"){
			//Inserir
			input >> size >> hash;

			file* newFile = new file;
			newFile->name = select2;
			newFile->size = size;
			newFile->hash = hash;
			//Adicionando arquivo ao nó
			addNode(tree, *newFile);

			}else{
				//Busca exata
				resul = searchNode(tree->root, select2);
				printResul(resul, select2, &output);
			}
		}
	}
	//Closing input file and output file
	input.close();
	output.close();

	return 0;
}