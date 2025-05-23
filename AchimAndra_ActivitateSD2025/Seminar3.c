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

void afisareMasina(Masina masina) {

	printf("id: %d\n", masina.id);
	printf("nrUsi: %d\n", masina.nrUsi);
	printf("pret: %.2f\n", masina.pret);
	printf("model: %s\n", masina.model);
	printf("nume Sofer: %s\n", masina.numeSofer);
	printf("serie: %c\n\\n\n\n", masina.serie);


}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++)
	{
		afisareMasina(masini[i]);
	}

}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));

	for (int i = 0; i < *nrMasini; i++)
	{
		aux[i] = (*masini)[i];
	}
	aux[(*nrMasini)] = masinaNoua;
	(*nrMasini)++;
	free(*masini);
	(*masini) = aux;


}

Masina citireMasinaFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	fgets(buffer, 100, file);
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = (char*) malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)  malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);

	return m1;

}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* file = fopen(numeFisier, "r");
	Masina* masini = NULL;

	do
	{
		adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(file));




	} while (!feof(file));
	fclose(file);
	return masini;



}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < (*nrMasini); i++)
	{
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	*vector = NULL;
	*nrMasini = 0;

}

float pretMediuDupaUsi(Masina* vector, int nrMasini, int nrUsi)
{
	float suma = 0;
	int ind = 0;
	for (int i = 0; i < nrMasini; i++)
	{
		if (vector[i].nrUsi == nrUsi)
		{
			suma += vector[i].pret;
			ind++;

		}
	}
	if (ind > 0)

		return suma / ind;

	return 0;
}

int main() {

	Masina* masini = NULL;
	int nrMasini = 0;
	masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);



}