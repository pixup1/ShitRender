#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <wingdi.h>
#include <math.h>

#include "points.h"

#define DIM 3

int NewPoint(double npoint[], double*** point, int* npts)
// On voudra modifier la valeur de point, donc on passe un pointeur vers point (donc bien un triple pointeur, ouais moi aussi je commence à péter un cable là)
{
	(*npts)++;
	if (*point == NULL) // Cas où le tableau est vide (genre au début)
	{
		*point = (double**)malloc(sizeof(double*));
	} else {
		*point = (double**)realloc(*point, sizeof(double*) * (*npts)); // Réallocation de la mémoire pour tous les points
	}
	if (*point == NULL) { //askip faut faire ça jsp
		fprintf(stderr, "Error: Failed to reallocate memory\n");
		exit(EXIT_FAILURE);
	}
	double* newpoint = (double*)malloc(DIM * sizeof(double)); // Allocation pour les coordonnées du nouveau point
	if (newpoint == NULL) { //pffff
		fprintf(stderr, "Error: Failed to reallocate memory\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < DIM; i++) {
		newpoint[i] = npoint[i];
	}
	
	(*point)[(*npts) - 1] = newpoint;
	return (*npts) - 1;
}

void DelPoint(int id, double*** point, int* npts)
{
	if (id >= *npts || id < 0) {
		return;
	}

	(*npts)--;
	
	for (int i = id; i < *npts; i++){
		for (int j = 0; j < DIM; j++) {
			(*point)[i][j] = (*point)[i + 1][j];
		}
	}

	if (*npts == 0) {
		free((*point)[0]);
		free(*point);
		*point = NULL;
	} else {
		free((*point)[*npts]); // On libère Lacrim     (*Lacrim = nom du dernier point)
		*point = (double**)realloc(*point, sizeof(double*) * (*npts)); // Réallocation de la mémoire pour tous les points
		if (*point == NULL) {
			fprintf(stderr, "Error: Failed to reallocate memory\n");
			exit(EXIT_FAILURE);
		}
	}
}

void FreePoints(double*** point, int* npts)
{
	for (int i = (*npts) - 1; i >= 0; i--)
	{
		DelPoint(i, point, npts);
	}
}

void PrintPoints(double** point, int npts)
{
	if (point == NULL) {
		printf_s("Il n'y a point de points\n");
	} else {
		printf_s("%d points:\n", npts);
		for (int i = 0; i < npts; i++)
		{
			for (int j = 0; j < DIM; j++)
			{
				printf_s("%.4lf; ", point[i][j]);
			}
			printf_s("\n");
		}
	}
}

int NewEdge(int nedge[], int*** edge, int* nedg)
{
	(*nedg)++;
	if (*edge == NULL) // Cas où le tableau est vide (genre au début)
	{
		*edge = (int**)malloc(sizeof(int*));
	}
	else {
		*edge = (int**)realloc(*edge, sizeof(int*) * (*nedg)); // Réallocation de la mémoire pour toutes les aretes
	}
	if (*edge == NULL) {
		fprintf(stderr, "Error: Failed to reallocate memory\n");
		exit(EXIT_FAILURE);
	}
	int* newedge = (int*)malloc(2 * sizeof(int)); // Allocation pour les données de la nouvelle arete
	if (newedge == NULL) {
		fprintf(stderr, "Error: Failed to reallocate memory\n");
		exit(EXIT_FAILURE);
	}
	
	newedge[0] = nedge[0];
	newedge[1] = nedge[1];
	
	(*edge)[(*nedg) - 1] = newedge;
	return (*nedg) - 1;
}

void DelEdge(int id, int*** edge, int* nedg)
{
	if (id >= *nedg || id < 0) {
		return;
	}

	(*nedg)--;

	for (int i = id; i < *nedg; i++) {
		for (int j = 0; j < 2; j++) {
			(*edge)[i][j] = (*edge)[i + 1][j];
		}
	}

	if (*nedg == 0) {
		free((*edge)[0]);
		free(*edge);
		*edge = NULL;
	}
	else {
		free((*edge)[*nedg]); // On libère Lacrim     (*Lacrim = nom du dernier edge)
		*edge = (int**)realloc(*edge, sizeof(int*) * (*nedg)); // Réallocation de la mémoire pour tous les edges
		if (*edge == NULL) {
			fprintf(stderr, "Error: Failed to reallocate memory\n");
			exit(EXIT_FAILURE);
		}
	}
}

void DelDoubleEdges(int*** edge, int* nedg)
{
	for (int i = 0; i < *nedg; i++)
	{
		for (int j = 0; j < *nedg; j++)
		{
			if (j != i)
			{
				if (((*edge)[i][0] == (*edge)[j][0] && (*edge)[i][1] == (*edge)[j][1]) || ((*edge)[i][0] == (*edge)[j][1] && (*edge)[i][1] == (*edge)[j][0]))
				{
					DelEdge(j, edge, nedg);
				}
			}
		}
	}
}

void FreeEdges(double*** edge, int* nedg)
{
	for (int i = (*nedg) - 1; i >= 0; i--)
	{
		DelEdge(i, edge, nedg);
	}
}

void PrintEdges(int** edge, int nedg)
{
	if (edge == NULL) {
		printf_s("Il n'y a point d'aretes\n");
	}
	else {
		printf_s("%d edges:\n", nedg);
		for (int i = 0; i < nedg; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				printf_s("%d; ", edge[i][j]);
			}
			printf_s("\n");
		}
	}
}

int NewFace(int nface[], int*** face, int* nfcs, int*** edge, int* nedg, double*** normal)
{
	(*nfcs)++;
	if (*face == NULL) // Cas où le tableau est vide (genre au début)
	{
		*face = (int**)malloc(sizeof(int*));
		*normal = (double**)malloc(sizeof(double*));
	}
	else {
		*face = (int**)realloc(*face, sizeof(int*) * (*nfcs)); // Réallocation de la mémoire pour toutes les faces et normales
		*normal = (double**)realloc(*normal, sizeof(double*) * (*nfcs));
	}
	if ((*face == NULL) || (*normal == NULL)) {
		fprintf(stderr, "Error: Failed to reallocate memory\n");
		exit(EXIT_FAILURE);
	}
	int* newface = (int*)malloc(3 * sizeof(int)); // Allocation pour les données de la nouvelle face et nouvelle normale
	double* newnormal = (double*)malloc(DIM * sizeof(double));
	if ((newface == NULL) || ((*normal)[(*nfcs) - 1] == NULL)) {
		fprintf(stderr, "Error: Failed to reallocate memory\n");
		exit(EXIT_FAILURE);
	}

	newface[0] = nface[0];
	newface[1] = nface[1];
	newface[2] = nface[2];
	
	// On initialise la normale sans la calculer
	newnormal[0] = 0;
	newnormal[1] = 0;
	newnormal[2] = 0;

	// Faut aussi faire les aretes
	NewEdge((int[]) { nface[0], nface[1] }, edge, nedg);
	NewEdge((int[]) { nface[1], nface[2] }, edge, nedg);
	NewEdge((int[]) { nface[2], nface[0] }, edge, nedg);

	(*face)[(*nfcs) - 1] = newface;
	(*normal)[(*nfcs) - 1] = newnormal;
	return (*nfcs) - 1;
}

int Triangulate(int nface[4], int*** face, int* nfcs, int*** edge, int* nedg, double*** normal) // A priori y'a des chances que ça marche comme ça
{
	int tmpnedg = *nedg;
	NewFace((int[]) { nface[0], nface[2], nface[1] }, face, nfcs, edge, nedg, normal);
	NewFace((int[]) { nface[0], nface[2], nface[3] }, face, nfcs, edge, nedg, normal);
	DelEdge(tmpnedg, edge, nedg); // Clean up by deleting double edge
}

void DelFace(int id, int*** face, int* nfcs, double*** normal)
{
	if (id >= *nfcs || id < 0) {
		return;
	}

	(*nfcs)--;

	for (int i = id; i < *nfcs; i++) {
		for (int j = 0; j < 3; j++) {
			(*face)[i][j] = (*face)[i + 1][j];
		}
	}

	for (int i = id; i < *nfcs; i++) { // Pareil pr les normales
		for (int j = 0; j < DIM; j++) {
			(*normal)[i][j] = (*normal)[i + 1][j];
		}
	}

	if (*nfcs == 0) {
		free((*face)[0]);
		free(*face);
		*face = NULL;
		free((*normal)[0]);
		free(*normal);
		*normal = NULL;
	}
	else {
		free((*face)[*nfcs]); // On libère Lacrim     (*Lacrim = nom de la derniere face)
		free((*normal)[*nfcs]);
		*face = (int**)realloc(*face, sizeof(int*) * (*nfcs)); // Réallocation de la mémoire pour toutes les faces
		*normal = (double**)realloc(*normal, sizeof(double*) * (*nfcs));
		if (*face == NULL) {
			fprintf(stderr, "Error: Failed to reallocate memory\n");
			exit(EXIT_FAILURE);
		}
	}
}

void FreeFaces(double*** face, int* nfcs, double*** normal)
{
	for (int i = (*nfcs) - 1; i >= 0; i--)
	{
		DelFace(i, face, nfcs, normal);
	}
}

void PrintFaces(int** face, int nfcs)
{
	if (face == NULL) {
		printf_s("Il n'y a point de faces\n");
	}
	else {
		printf_s("%d faces:\n", nfcs);
		for (int i = 0; i < nfcs; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				printf_s("%d; ", face[i][j]);
			}
			printf_s("\n");
		}
	}
}

void CalculateNormals(double** normal, double** point, int npts, int** edge, int nedg, int** face, int nfcs)
{
	if (nfcs == 0) {
		return; // On va pas se faire chier si y'a pas de faces
	}
	/*int** facesfromedge = (int**)malloc(sizeof(int*) * nedg); // Array des deux faces correspondand à une arete
	for (int i = 0; i < nedg; i++)
	{
		facesfromedge[i] = (int*)malloc(2 * sizeof(int));

		// On va chercher quelles faces contiennent à la fois edge[i][0] et edge[i][1]
		int j = 0;
		int cpt = 0;
		while (cpt < 2 && j < nfcs)
		{
			if (((face[j][0] == edge[i][0]) || (face[j][1] == edge[i][0]) || (face[j][2] == edge[i][0])) && ((face[j][0] == edge[i][1]) || (face[j][1] == edge[i][1]) || (face[j][2] == edge[i][1])))
			{
				facesfromedge[i][cpt] = j;
				cpt++;
			}
			j++;
		}
		while (cpt < 2)
		{
			facesfromedge[i][cpt] = -1; // -1 ça veut dire qu'il y a pas de face
		}
	}*/

	int** faceadj = (int**)malloc(sizeof(int*) * nfcs); // Array des trois faces adjacentes à une face
	for (int i = 0; i < nfcs; i++)
	{
		faceadj[i] = (int*)malloc(3 * sizeof(int));

		faceadj[i][0] = -1;
		faceadj[i][1] = -1; // Default values
		faceadj[i][2] = -1;

		// On trouve une face partageant face[i][0] et face[i][1]
		for (int j = 0; j < nfcs; j++)
		{
			if (((face[i][0] == face[j][0]) || (face[i][0] == face[j][1]) || (face[i][0] == face[j][2])) && ((face[i][1] == face[j][0]) || (face[i][1] == face[j][1]) || (face[i][1] == face[j][2])))
			{
				faceadj[i][0] = j;
				break;
			}
		}
		// On trouve une face partageant face[i][1] et face[i][2]
		for (int j = 0; j < nfcs; j++)
		{
			if (((face[i][1] == face[j][0]) || (face[i][1] == face[j][1]) || (face[i][1] == face[j][2])) && ((face[i][2] == face[j][0]) || (face[i][2] == face[j][1]) || (face[i][2] == face[j][2])))
			{
				faceadj[i][1] = j;
				break;
			}
		}
		// On trouve une face partageant face[i][2] et face[i][0]
		for (int j = 0; j < nfcs; j++)
		{
			if (((face[i][2] == face[j][0]) || (face[i][2] == face[j][1]) || (face[i][2] == face[j][2])) && ((face[i][0] == face[j][0]) || (face[i][0] == face[j][1]) || (face[i][0] == face[j][2])))
			{
				faceadj[i][2] = j;
				break;
			}
		}
	}

	int* index = (int*)malloc(sizeof(int) * nfcs); // Array qui sera trié contenant l'ordre des faces

	for (int i = 0; i < nfcs; i++) // On le remplit de valeurs croissantes, qui seront triées juste après.
	{
		index[i] = i;

		// On va en profiter pour réinitialiser toutes les normales
		for (int j = 0; j < DIM; j++)
		{
			normal[i][j] = 0;
		}
	}

	SortFaces(index, face, nfcs, point, (double[]) { 1, 0, 0 });

	printf_s("Ordre des faces:\n");
	for (int i = 0; i < nfcs; i++) {
		printf_s("%d\n", index[i]);
	}

	int nNormal = 0; // On s'en servira pour suivre le nombre de normales qu'on calcule
	while (nNormal < nfcs)
	{
		int i = 0;
		while ((i < nfcs) && ((normal[index[i]][0] == 0) && (normal[index[i]][1] == 0) && (normal[index[i]][2] == 0)) == FALSE) // On prend le premier i sur les x dont le normal est pas calculé
		{
			i++;
		}
		// We do a little cross product for dat normal
		double v1[3] = { point[face[index[i]][1]][0] - point[face[index[i]][0]][0], point[face[index[i]][1]][1] - point[face[index[i]][0]][1], point[face[index[i]][1]][2] - point[face[index[i]][0]][2] }; // Vecteur 0 - 1
		double v2[3] = { point[face[index[i]][1]][0] - point[face[index[i]][2]][0], point[face[index[i]][1]][1] - point[face[index[i]][2]][1], point[face[index[i]][1]][2] - point[face[index[i]][2]][2] }; // Vecteur 2 - 1
		
		normal[index[i]] = (double[]) { v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0]};
		NormalizeVector(normal[index[i]]);

		boolean asc;
		if (normal[index[i]][0] < 0) { // On inverse la normale si elle est dans le mauvais sens
			for (int j = 0; j < DIM; j++) {
				normal[index[i]][j] *= -1;
			}
			asc = TRUE;
		} else {
			// c bon
			asc = FALSE;
		}

		if (faceadj[index[i]][0] != -1) {
			// On regarde si l'ordre des points est inversé dans la face d'à côté et on inverse encore asc si c'est le cas
			if (((face[index[i]][0] == face[faceadj[index[i]][0]][0]) && (face[index[i]][1] == face[faceadj[index[i]][0]][1])) || ((face[index[i]][0] == face[faceadj[index[i]][0]][1]) && (face[index[i]][1] == face[faceadj[index[i]][0]][2])) || ((face[index[i]][0] == face[faceadj[index[i]][0]][2]) && (face[index[i]][1] == face[faceadj[index[i]][0]][0])))
			{
				asc = (asc == FALSE);
			}
			RecursNormal(normal, &nNormal, face, faceadj, point, faceadj[index[i]][0], asc);
		}
		if (faceadj[index[i]][1] != -1) {
			if (((face[index[i]][0] == face[faceadj[index[i]][1]][0]) && (face[index[i]][1] == face[faceadj[index[i]][1]][1])) || ((face[index[i]][0] == face[faceadj[index[i]][1]][1]) && (face[index[i]][1] == face[faceadj[index[i]][1]][2])) || ((face[index[i]][0] == face[faceadj[index[i]][1]][2]) && (face[index[i]][1] == face[faceadj[index[i]][1]][0])))
			{
				asc = (asc == FALSE);
			}
			RecursNormal(normal, &nNormal, face, faceadj, point, faceadj[index[i]][1], asc);
		}
		if (faceadj[index[i]][2] != -1) {
			if (((face[index[i]][0] == face[faceadj[index[i]][2]][0]) && (face[index[i]][1] == face[faceadj[index[i]][2]][1])) || ((face[index[i]][0] == face[faceadj[index[i]][2]][1]) && (face[index[i]][1] == face[faceadj[index[i]][2]][2])) || ((face[index[i]][0] == face[faceadj[index[i]][2]][2]) && (face[index[i]][1] == face[faceadj[index[i]][2]][0])))
			{
				asc = (asc == FALSE);
			}
			RecursNormal(normal, &nNormal, face, faceadj, point, faceadj[index[i]][2], asc);
		}
	}
	printf_s("nNormal : %d", nNormal);

	// On libère toute la mémoire qu'on a prise
	for (int i = 0; i < nfcs; i++)
	{
		free(faceadj[i]);
	}
	free(faceadj);
	free(index);
	/*for (int i = 0; i < nedg; i++)
	{
		free(facesfromedge[i]);
	}
	free(facesfromedge);*/
}

void RecursNormal(double** normal, int* nNormal, int** face, int** faceadj, double** point, int id, boolean ascending)
{
	double v1[3] = { point[face[id][1]][0] - point[face[id][0]][0], point[face[id][1]][1] - point[face[id][0]][1], point[face[id][1]][2] - point[face[id][0]][2] }; // Vecteur 0 - 1
	double v2[3] = { point[face[id][1]][0] - point[face[id][2]][0], point[face[id][1]][1] - point[face[id][2]][1], point[face[id][1]][2] - point[face[id][2]][2] }; // Vecteur 2 - 1

	if (ascending){
		normal[id] = (double[]){ v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0] };
	} else {
		normal[id] = (double[]){ v2[1] * v1[2] - v2[2] * v1[1], v2[2] * v1[0] - v2[0] * v1[2], v2[0] * v1[1] - v2[1] * v1[0] };
	}

	NormalizeVector(normal[id]);
	(*nNormal)++;

	ascending = (ascending == FALSE);

	// For each adjacent face, we check if : 1) It exists and 2) It's set to 0, which means it hasn't been calculated yet.
	if (faceadj[id][0] != -1) {
		if (face[faceadj[id][0]][0] == 0 && face[faceadj[id][0]][1] == 0 && face[faceadj[id][0]][2] == 0)
		{
			if (((face[id][0] == face[faceadj[id][0]][0]) && (face[id][1] == face[faceadj[id][0]][1])) || ((face[id][0] == face[faceadj[id][0]][1]) && (face[id][1] == face[faceadj[id][0]][2])) || ((face[id][0] == face[faceadj[id][0]][2]) && (face[id][1] == face[faceadj[id][0]][0])))
			{
				ascending = (ascending == FALSE);
			}
			RecursNormal(normal, nNormal, face, faceadj, point, faceadj[id][0], ascending);
		}
	}
	if (faceadj[id][1] != -1) {
		if (face[faceadj[id][1]][0] == 0 && face[faceadj[id][1]][1] == 0 && face[faceadj[id][1]][2] == 0)
		{
			if (((face[id][0] == face[faceadj[id][1]][0]) && (face[id][1] == face[faceadj[id][1]][1])) || ((face[id][0] == face[faceadj[id][1]][1]) && (face[id][1] == face[faceadj[id][1]][2])) || ((face[id][0] == face[faceadj[id][1]][2]) && (face[id][1] == face[faceadj[id][1]][0])))
			{
				ascending = (ascending == FALSE);
			}
			RecursNormal(normal, nNormal, face, faceadj, point, faceadj[id][1], ascending);
		}
	}
	if (faceadj[id][2] != -1) {
		if (face[faceadj[id][2]][0] == 0 && face[faceadj[id][2]][1] == 0 && face[faceadj[id][2]][2] == 0)
		{
			if (((face[id][0] == face[faceadj[id][2]][0]) && (face[id][1] == face[faceadj[id][2]][1])) || ((face[id][0] == face[faceadj[id][2]][1]) && (face[id][1] == face[faceadj[id][2]][2])) || ((face[id][0] == face[faceadj[id][2]][2]) && (face[id][1] == face[faceadj[id][2]][0])))
			{
				ascending = (ascending == FALSE);
			}
			RecursNormal(normal, nNormal, face, faceadj, point, faceadj[id][2], ascending);
		}
	}
}

void PrintNormals(double** normal, int nfcs)
{
	if (normal == NULL) {
		printf_s("Il n'y a point de normales\n");
	}
	else {
		printf_s("%d normales:\n", nfcs);
		for (int i = 0; i < nfcs; i++)
		{
			for (int j = 0; j < DIM; j++)
			{
				printf_s("%.4lf; ", normal[i][j]);
			}
			printf_s("\n");
		}
	}
}

void SortFaces(int* index, int** face, int nfcs, double** point, double ref[DIM])
{
	double** avrface = (double**)malloc(sizeof(double*) * nfcs); // Array des positions moyennes des faces
	for (int i = 0; i < nfcs; i++)
	{
		avrface[i] = (int*)malloc(DIM * sizeof(int));

		for (int j = 0; j < DIM; j++)
		{
			avrface[i][j] = (point[face[i][0]][j] + point[face[i][1]][j] + point[face[i][2]][j])/3;
		}
	}

	// Let's rotate all of avrface to make the first coordinate the determining index. (ie. follow the same rotation that would turn ref[] into { 1, 0, 0 }
	// First we'll normalize ref just in case
	NormalizeVector(ref);

	// Logically we should only need to rotate stuff around z and y (...right ??????????????????????)
	RotateObject3d((double[]) { 0, asin(ref[2]), -asin(ref[1])}, avrface, nfcs); // pas sûr là

	// k. Now we only need to sort the array based on the x dimension ! Yipee !                      epstein didn't kill himself
	QuickSortX(index, avrface, 0, nfcs - 1);
}


void QuickSortX(int* index, double** avrface, int left, int right) // ChatGPT gave me this, I don't even care anymore
{
 	int i = left, j = right;
	int tmp;
	double pivot = avrface[index[right]][0];

	// partition
	while (i <= j) {
		while (avrface[index[i]][0] < pivot) // Move i to the right until it points to an element greater than or equal to the pivot
		{i++;}
		while (avrface[index[j]][0] > pivot) // Move j to the left until it points to an element less than or equal to the pivot
		{j--;}
		if (i <= j) {
			tmp = index[i];
			index[i] = index[j];
			index[j] = tmp;
			i++;
			j--;
		}
	}

	// recursion
	if (left < j)// If the left subarray has more than one element
		QuickSortX(index, avrface, left, j);
	if (i < right) // If the right subarray has more than one element
		QuickSortX(index, avrface, i, right);
}

/*
void QuickSortX(int* index, double** avrface, int left, int right) // Different sorting alg, but actually the other one works fine and is more efficient
{
	// I don't know what algorithm this is but I can guarantee it won't be fucking quicksort
	for (int i = left; i <= right; i++)
	{
		int lowest = i;

		for (int i2 = i; i2 <= right; i2++)
		{
			if (avrface[index[i2]][0] < avrface[index[lowest]][0])
			{
				lowest = i2;
			}
		}

		int tmp;
		
		if (lowest != i) {
			tmp = index[i];
			index[i] = index[lowest];
			index[lowest] = tmp;
		}
	}
}
*/

void NormalizeVector(double v[DIM])
{
	//Find the length of v and then divide every dimension by that.
	int length = 0;
	for (int i = 0; i < DIM; i++)
	{
		length += v[i] * v[i];
	}
	length = sqrt(length);
	for (int i = 0; i < DIM; i++)
	{
		v[i] /= length;
	}
}

void RotatePoint3d(double rot[], double* point)
{
	double tmpoint[3];
	// This function uses rotation matrix transcriptions
	// Rotate around x
	for (int j = 0; j < 3; j++) // We make a copy of the point to act as the initial position matrix
	{
		tmpoint[j] = point[j];
	}
	point[1] = cos(rot[0]) * tmpoint[1] - sin(rot[0]) * tmpoint[2];
	point[2] = sin(rot[0]) * tmpoint[1] + cos(rot[0]) * tmpoint[2];

	// Rotate around y
	for (int j = 0; j < 3; j++)
	{
		tmpoint[j] = point[j];
	}
	point[0] = cos(rot[1]) * tmpoint[0] + sin(rot[1]) * tmpoint[2];
	point[2] = -sin(rot[1]) * tmpoint[0] + cos(rot[1]) * tmpoint[2];

	// Rotate around z
	for (int j = 0; j < 3; j++)
	{
		tmpoint[j] = point[j];
	}
	point[0] = cos(rot[2]) * tmpoint[0] - sin(rot[2]) * tmpoint[1];
	point[1] = sin(rot[2]) * tmpoint[0] + cos(rot[2]) * tmpoint[1];
}

void MovePoint(double delta[], double* point)
{
	for (int j = 0; j < DIM; j++)
	{
		point[j] += delta[j];
	}
}

void RotateObject3d(double rot[], double** point, int npts)
{
	double tmpoint[3];
	for (int i = 0; i < npts; i++) // We do this whole shtick for each point
	{   
		RotatePoint3d(rot, point[i]);
	}
}

void MoveObject(double delta[], double** point, int npts)
{
	for (int i = 0; i < npts; i++)
	{
		MovePoint(delta, point[i]);
	}
}

void switchyz(double point[])
{
	double z = point[1];
	point[1] = point[2];
	point[2] = z;
}

void Cube(double position[], double size, double*** point, int* npts, int*** edge, int* nedg)
{
	int ids[8];
	
	// Bottom points and edges
	ids[0] = NewPoint((double[]) { position[0] + size / 2, position[1] + size / 2, position[2] - size / 2 }, point, npts);
	ids[1] = NewPoint((double[]) { position[0] - size / 2, position[1] + size / 2, position[2] - size / 2 }, point, npts);
	ids[2] = NewPoint((double[]) { position[0] - size / 2, position[1] - size / 2, position[2] - size / 2 }, point, npts);
	ids[3] = NewPoint((double[]) { position[0] + size / 2, position[1] - size / 2, position[2] - size / 2 }, point, npts);

	NewEdge((int[]) { ids[0], ids[1] }, edge, nedg);
	NewEdge((int[]) { ids[1], ids[2] }, edge, nedg);
	NewEdge((int[]) { ids[2], ids[3] }, edge, nedg);
	NewEdge((int[]) { ids[3], ids[0] }, edge, nedg);

	// Top points and edges
	ids[4] = NewPoint((double[]) { position[0] + size / 2, position[1] + size / 2, position[2] + size / 2 }, point, npts);
	ids[5] = NewPoint((double[]) { position[0] - size / 2, position[1] + size / 2, position[2] + size / 2 }, point, npts);
	ids[6] = NewPoint((double[]) { position[0] - size / 2, position[1] - size / 2, position[2] + size / 2 }, point, npts);
	ids[7] = NewPoint((double[]) { position[0] + size / 2, position[1] - size / 2, position[2] + size / 2 }, point, npts);

	NewEdge((int[]) { ids[4], ids[5] }, edge, nedg);
	NewEdge((int[]) { ids[5], ids[6] }, edge, nedg);
	NewEdge((int[]) { ids[6], ids[7] }, edge, nedg);
	NewEdge((int[]) { ids[7], ids[4] }, edge, nedg);

	// Middle edges
	NewEdge((int[]) { ids[0], ids[4] }, edge, nedg);
	NewEdge((int[]) { ids[1], ids[5] }, edge, nedg);
	NewEdge((int[]) { ids[2], ids[6] }, edge, nedg);
	NewEdge((int[]) { ids[3], ids[7] }, edge, nedg);
}