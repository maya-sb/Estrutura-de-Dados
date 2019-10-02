#include <iostream>
#include <string>
#include <fstream>

using namespace std;

typedef struct livro {
	long long isbn = 0;
	string autor = "";
	string titulo = "";

}livro;

typedef struct busca {
	int num_binaria = 0;
	int num_interpolada = 0;

}busca;

int busca_binaria(long long isbn, livro* livros, string* autor, string* titulo, int num_livros){
	int cont = 1;

	int i =0, j = num_livros - 1;
	int p = (i+j)/2;

	while (j >= i && livros[p].isbn != isbn){
		if (livros[p].isbn > isbn){
			j = p - 1;
		}else{
			i = p + 1;
		}
		p = (i+j)/2;
		cont++;
	}

	if (livros[p].isbn == isbn){
		*autor = livros[p].autor;
		*titulo =  livros[p].titulo;
	}else{
		*autor = "";
		*titulo = "";
	}

	return cont;
}

int busca_interpolada(long long isbn, livro* livros, string* autor, string* titulo, int num_livros){
	int cont = 1;

	int i =0, j = num_livros - 1;
	long long p = 0;

	if (j >= 0 && i >= 0 && j>=i){
		p = i + ((livros[j].isbn - livros[i].isbn) % (j - i + 1));
	}

	while (j >= i && livros[p].isbn != isbn){
		if (livros[p].isbn > isbn){
			j = p - 1;
		}else{
			i = p + 1;
		}	
		if (j >= 0 && i >= 0 && j>=i){
			p = i + ((livros[j].isbn - livros[i].isbn) % (j - i + 1));
			cont++;
		}
	}

	if (livros[p].isbn == isbn){
		*autor = livros[p].autor;
		*titulo = livros[p].titulo;
	}else{
		*autor = "";
		*titulo = "";
	}

	return cont;
}

string buscar(long long isbn, livro* livros, busca* busca_atual, int num_livros){

	string autor = "";
	string titulo = "";

	string resul;
	int binaria = 0, interpolada = 0;

	binaria = busca_binaria(isbn, livros, &autor, &titulo, num_livros);
	interpolada = busca_interpolada(isbn, livros, &autor, &titulo, num_livros);

	busca_atual -> num_binaria = binaria;
	busca_atual -> num_interpolada = interpolada;

	if (autor != ""){
		resul = "[" + to_string(isbn) + "]B=" + to_string(binaria) + ",I=" + to_string(interpolada) + ":Author:" + autor + ",Title:" + titulo+ "\n";
	}else{
		resul = "[" + to_string(isbn) + "]B=" + to_string(binaria) + ",I=" + to_string(interpolada) + ":ISBN_NOT_FOUND\n";
	}

	return resul;
}

string gerar_resultado(busca* buscas, int num_buscas){

	string resul = "";

	int menor_binaria=0, menor_interpolada=0, media_binaria=0, media_interpolada=0;

	for (int i=0; i<num_buscas; i++){
		media_binaria += buscas[i].num_binaria;
		media_interpolada += buscas[i].num_interpolada;

		if (buscas[i].num_interpolada<=buscas[i].num_binaria){
			menor_interpolada++;
		}else{
			menor_binaria++;
		}
	}
	media_binaria = media_binaria/num_buscas;
	media_interpolada = media_interpolada/num_buscas;

	resul = "BINARY=" + to_string(menor_binaria) + ":" + to_string(media_binaria) + "\n" + "INTERPOLATION=" + to_string(menor_interpolada) + ":" + to_string(media_interpolada) + "\n";
	return resul;
}

int main(int argc, char* argv[]){

	ifstream input;
	input.open(argv[1]);
	ofstream output;
	output.open(argv[2]);

	int num_livros, pos;
	long long isbn;
	string titulo_autor, titulo, autor;

	input >> num_livros;
	livro* livros = new livro[num_livros];

	for (int i=0; i<num_livros; i++){
		input >> isbn;
		input.ignore();
		getline(input, titulo_autor);

		pos = titulo_autor.find("&");
		autor = titulo_autor.substr(0,pos);
		titulo = titulo_autor.substr(pos+1);

		livros[i].isbn = isbn;
		livros[i].autor = autor;
		livros[i].titulo = titulo;
	}

	int num_buscas; 
	long long busca_isbn;
	string resul;

	input >> num_buscas;
	busca* buscas = new busca[num_buscas];

	for (int i=0; i<num_buscas; i++){
		input >> busca_isbn;
		resul = buscar(busca_isbn, livros, &buscas[i], num_livros);
		output << resul;
	}

	resul = gerar_resultado(buscas, num_buscas);
	output << resul;

	input.close();
	output.close();

	return 0;

}