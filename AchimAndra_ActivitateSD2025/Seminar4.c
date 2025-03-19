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

//creare structura pentru un nod dintr-o lista simplu inlantuita
typedef struct StructuraMasina Masina;
typedef struct Nod Nod;

struct Nod
{
	Masina info;
	Nod* urm;
};
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

void afisareListaMasini(Nod* cap) {
	//afiseaza toate elemente de tip masina din lista simplu inlantuita
	//prin apelarea functiei afisareMasina()
	while (cap)
	{
		afisareMasina(cap->info);
		cap = cap->urm;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	//ne cream un nod, copiem acest 1 in nodul nostru, deoarece inseram si la final va fi null 
	//in care cap suntem, cap = null => adresa a7 retine adresa a7 
	//daca exista cap-> ne deplasam cu cel nou

	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua; //o dezalocam- nu; facem shallow copy;
	nou->urm = NULL; // verificam nodul urmator

	if ((*cap) == NULL)
	{
		*cap = nou; //ii dau o adresa, trebuie sa l modific prin adresa, de asta are pointer

	}
	else //daca ne deplasam cu cap nu mai putem accesa cele din lista trecuta(ant), facem un nod care se misca, cap ramane tot acolo
	{
		Nod* p = *cap;
		while (p->urm)
		{
			p = p->urm; //adresa nodului urmator, cat timp exista p urm ma pot deplasa 


		}
		p->urm = nou; //gen ma gandesc ca vreau sa merg la urmatorul, nu la null-ul ramas cum ar veni 
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->urm = *cap;
	*cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier, "r"); //deschidem in modul citire read r 
	Nod* lista = NULL;//ne declaram lista, trb sa o initializam obligatoriu
	while (!feof(f))
	{
		adaugaLaInceputInLista(&lista, citireMasinaDinFisier(f));
	}

	fclose(f);
	return lista;
}

void dezalocareListaMasini(Nod** cap) {
	//sunt dezalocate toate masinile si lista de elemente
	while ((*cap) != NULL)
	{
		Nod* aux = (*cap);
		*cap = (*cap)->urm;
		free(aux->info.numeSofer);
		free(aux->info.model);
		free(aux);

	}
}

float calculeazaPretMediu(Nod* lista) {
	//calculeaza pretul mediu al masinilor din lista.

	//if (!Lista) return 0;
	int count = 0; //cate elem avem in lista 
	float sum = 0;
	//parcurgem
	while (lista) //putem parcurge cu litsa ca e doar o copie
	{
		sum += lista->info.pret;
		count++;
		lista = lista->urm;

	}

	return (count>0) ? sum/count :0; 
}

void stergeMasiniDinSeria(/*lista masini*/ char serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	float sum = 0;
	while (lista)
	{
		
		if (strcmp(numeSofer, lista->info.numeSofer) == 0)
		{
			sum += lista->info.pret;
		}
		lista = lista->urm;
	}
	return sum;
}

char* getCeaMaiScumpaMasina(Nod* lista)
{
	float pretulMax = 0;
	float pretulCelMaiMare = 0;
	char* modelScump = NULL ;
	while (lista)
	{
		if (lista->info.pret > pretulMax)
		{
			pretulMax = lista->info.pret;
			modelScump = lista->info.model;
			
		}
		lista = lista->urm;

	}
	if (pretulMax > 0)
	{
		//luam alt pointer la char
		char* nou = malloc(strlen(modelScump) + 1);
		strcpy_s(nou, strlen(modelScump) + 1, modelScump);
		return nou;
	}
}

int main() {
	Nod* cap = NULL;
	cap = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(cap);
	printf("%f \n\n", calculeazaPretMediu(cap));
	printf("%f", calculeazaPretulMasinilorUnuiSofer(cap, "Ionescu"));
	printf("%s", getCeaMaiScumpaMasina(cap));
	/*printf("%s", model);
	if (model)
	{
		free(model);
	}*/

	dezalocareListaMasini(&cap);
	return 0;
}