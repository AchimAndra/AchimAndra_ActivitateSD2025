#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct Nod Nod;
struct Nod {
	Nod* st;
	Nod* dr;
	Masina info;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	Masina m1;

	if (fgets(buffer, 100, file) == NULL) {
		m1.id = -1;
		m1.model = NULL;
		m1.numeSofer = NULL;
		return m1;
	}

	char* aux;
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
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	if ((*radacina) != NULL) {
		if (masinaNoua.id < (*radacina)->info.id) {
			adaugaMasinaInArbore(&((*radacina)->st), masinaNoua);
		}
		else if (masinaNoua.id > (*radacina)->info.id) {
			adaugaMasinaInArbore(&((*radacina)->dr), masinaNoua);
		}
		else {
			free((*radacina)->info.model);
			free((*radacina)->info.numeSofer);
			(*radacina)->info.nrUsi = masinaNoua.nrUsi;
			(*radacina)->info.pret = masinaNoua.pret;
			(*radacina)->info.serie = masinaNoua.serie;
			(*radacina)->info.model = (char*)malloc(strlen(masinaNoua.model) + 1);
			strcpy_s((*radacina)->info.model, strlen(masinaNoua.model) + 1, masinaNoua.model);
			(*radacina)->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
			strcpy_s((*radacina)->info.numeSofer, strlen(masinaNoua.numeSofer) + 1, masinaNoua.numeSofer);
		}
	}
	else {
		(*radacina) = (Nod*)malloc(sizeof(Nod));
		(*radacina)->info.id = masinaNoua.id;
		(*radacina)->info.nrUsi = masinaNoua.nrUsi;
		(*radacina)->info.pret = masinaNoua.pret;
		(*radacina)->info.serie = masinaNoua.serie;
		(*radacina)->info.model = (char*)malloc(strlen(masinaNoua.model) + 1);
		strcpy_s((*radacina)->info.model, strlen(masinaNoua.model) + 1, masinaNoua.model);
		(*radacina)->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
		strcpy_s((*radacina)->info.numeSofer, strlen(masinaNoua.numeSofer) + 1, masinaNoua.numeSofer);
		(*radacina)->st = NULL;
		(*radacina)->dr = NULL;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier)
{
	Nod* radacina = NULL;
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		perror("Eroare deschidere fisier");
		return NULL;
	}

	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		if (m.id == -1)
			break;
		adaugaMasinaInArbore(&radacina, m);
		free(m.model);
		free(m.numeSofer);
	}
	fclose(file);
	return radacina;
}

void afisareMasiniDinArborePreordine(Nod* radacina)
{

	if (radacina != NULL) {

		afisareMasina(radacina->info);
		afisareMasiniDinArborePreordine(radacina->st);
		afisareMasiniDinArborePreordine(radacina->dr);
	}
}

void afisareMasiniDinArboreInordine(Nod* radacina) {
	if (radacina != NULL) {
		afisareMasiniDinArboreInordine(radacina->st);
		afisareMasina(radacina->info);
		afisareMasiniDinArboreInordine(radacina->dr);
	}
}

void dezalocareArboreDeMasini(Nod** radacina) {
	if ((*radacina) != NULL) {
		dezalocareArboreDeMasini(&((*radacina)->st));
		dezalocareArboreDeMasini(&((*radacina)->dr));
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);
		(*radacina) = NULL;
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
	Masina m;
	m.id = -1;
	m.model = NULL;
	m.numeSofer = NULL;

	if (radacina == NULL) {
		return m;
	}
	if (id < radacina->info.id) {
		return getMasinaByID(radacina->st, id);
	}
	else if (id > radacina->info.id) {
		return getMasinaByID(radacina->dr, id);
	}
	else {
		m = radacina->info;
		m.model = (char*)malloc(strlen(radacina->info.model) + 1);
		strcpy_s(m.model, strlen(radacina->info.model) + 1, radacina->info.model);
		m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
		strcpy_s(m.numeSofer, strlen(radacina->info.numeSofer) + 1, radacina->info.numeSofer);
		return m;
	}
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina != NULL) {
		return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
	}
	return 0;
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina == NULL)
		return 0;
	else
	{
		int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);

		int inaltimeMaxima;
		if (inaltimeSt > inaltimeDr)
			inaltimeMaxima = inaltimeSt;
		else
			inaltimeMaxima = inaltimeDr;

		return 1 + inaltimeMaxima;
	}
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina == NULL)
		return 0;
	else
	{
		return radacina->info.pret + calculeazaPretTotal(radacina->st) + calculeazaPretTotal(radacina->dr);
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina == NULL)
		return 0;
	else
	{
		float suma = 0;
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0)
		{
			suma += radacina->info.pret;
		}
		suma += calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer);
		suma += calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer);

		return suma;
	}
}

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");

	printf("Masini in inordine:\n");
	afisareMasiniDinArboreInordine(arbore);

	printf("\n----------------------------\n");
	Masina m = getMasinaByID(arbore, 2);
	if (m.id != -1) {
		printf("Masina cautata cu ID 2:\n");
		afisareMasina(m);
		free(m.model);
		free(m.numeSofer);
	}

	printf("\n-----------------------------\n");
	int nrNoduri = determinaNumarNoduri(arbore);
	printf("Numarul total de noduri in arbore: %d\n", nrNoduri);
	printf("Inaltimea arborelui: %d\n", calculeazaInaltimeArbore(arbore));

	printf("Pretul total al masinilor: %.2f\n", calculeazaPretTotal(arbore));
	printf("Pretul masinilor soferului Gigel: %.2f\n", calculeazaPretulMasinilorUnuiSofer(arbore, "Gigel"));

	dezalocareArboreDeMasini(&arbore);
	return 0;
}