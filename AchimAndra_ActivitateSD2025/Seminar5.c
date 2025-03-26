#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;
struct Nod {
	Masina info;
	struct Nod* urmator;
	struct Nod* precedent;

};

typedef struct Nod Nod;

//creare structura pentru un nod dintr-o lista dublu inlantuita


//creare structura pentru Lista Dubla 
struct Lista
{
	Nod* prim;
	Nod* ultim;
};

typedef struct Lista Lista;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniInceput(Lista list) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	//parrcurg de la inceput

	Nod* p = list.prim; //p = inceput
	while (p)
	{
		afisareMasina(p->info);
		p = p->urmator;
	}


}

void afisareListaMasiniSf(Lista list)
{
	Nod* p = list.ultim;
	while (p)
	{
		afisareMasina(p->info);
		p = p->precedent;
	}
} //la fel ca mai sus numai ca mergem de la final spre inceput practic si se schimba urmator cu precedent etc 
void adaugaMasinaInLista( Lista* list, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru

	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->precedent = list->ultim;
	p->urmator = NULL;
	
	if (list->ultim) //sa fie dif de null ca sa putem trece la urmatorul nod
	{
		list->ultim->urmator = p;
	}
	else 
	{ 
		list->prim = p; 
	}
	list->ultim = p; //noul nod e ultimul acum

	

}

void adaugaLaInceputInLista(Lista* list,  Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->urmator = list->prim;
	p->precedent = NULL;
	if (list->prim)
	{
		list->prim->precedent = p;
	}
	else
	{
		list->ultim = p;
	}
	
	list->prim = p;
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	Lista list;
	list.prim = NULL;
	list.ultim = NULL;
	while (!feof(f))
	{
		//adaugaMasinaInLista(&list, citireMasinaDinFisier(f));
		adaugaLaInceputInLista(&list, citireMasinaDinFisier(f));
	}
	//inchidem fisierul;
	fclose(f);
	return list;
}

void dezalocareLDMasini(Lista* lista) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
	Nod* p = lista->prim;
	while (p->urmator != NULL)
	{
		free(p->info.numeSofer);
		free(p->info.model);
		p = p->urmator;
		free(p->precedent);

	}
	free(p->info.numeSofer);
	free(p->info.model);
	free(p);
	lista->prim = NULL;
	lista->ultim = NULL;


}

float calculeazaPretMediu(Lista lista) {
	//calculeaza pretul mediu al masinilor din lista.
	Nod* aux = lista.prim;
	float pretMediu = 0;
	float suma = 0;
	float cnt = 1; //luam un contor
	while (aux)
	{
		suma += aux->info.pret;
		aux = aux->urmator;
		cnt++;
	}
	return suma;
}

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}

int main() {
	Lista lista;
	lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniInceput(lista);

	

	float pretMediu = calculeazaPretMediu(lista);
	printf("Pret mediu : %2f\n", pretMediu);
	dezalocareLDMasini(&lista);

	return 0;
}