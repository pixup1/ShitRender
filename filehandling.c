#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <wingdi.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "points.h"
#include "filehandling.h"

#define DIM 3

void ReadObj(FILE* f, struct Scene* scene)
{
	int nptsi;
	char line[100];

	while (fgets(line, 100, f) != NULL)
	{
		nptsi = scene->npts; // On garde en mémoire le nombre de points qui sont déjà là au début

		while (line[0] != 'v') // On attend de lire un point
		{
			// Do nothing lol, just skip dem lines
			if (fgets(line, 100, f) == NULL) { break; }
		}
		while (line[0] == 'v') // Tant qu'on lit des points
		{
			if (line[1] != 'n')
			{
				int j = 2;
				double npoint[DIM];
				
				for (int i = 0; i < DIM; i++)
				{
					int k = 0;
					char strtmp[20]; // Temp string for reading numbers

					while (line[j] != ' ' && line[j] != '\n')
					{
						strtmp[k] = line[j];
						j++; k++;
					}
					strtmp[k] = '\0'; // Il faut un caractère nul à la fin du string pour atof
					npoint[i] = atof(strtmp);
					
					j++; // Le prochain caractère lu doit être celui après l'espace
				}

				switchyz(npoint); // By default, the second dimension in a .obj file is actually the up-down dimension
				NewPoint(npoint, &(scene->point), &(scene->npts));
			}
			if (fgets(line, 100, f) == NULL) { break; }
		}
		while (line[0] == 'f' || line[0] == 's' || line[0] == 'u') // Tant qu'on lit des faces
		{
			if (line[0] == 'f')
			{
				int j = 2;
				int nface[4]; // 4 points max
				int i = 0;
				boolean quad = FALSE;

				while ((quad && i < 4) || (quad == FALSE && i < 3))
				{
					int k = 0;
					char strtmp[20]; // Temp string for reading numbers

					while (line[j] != ' ' && line[j] != '\n' && line[j] != '/')
					{
						strtmp[k] = line[j];
						j++; k++;
					}

					if (line[j] == '/')
					{
						while (line[j] != ' ' && line[j] != '\n')
						{
							j++;
						}
					}
					
					if (line[j] == ' ' && i == 2) { quad = TRUE; } // Si il reste des trucs c'est qu'il y aura 4 points

					strtmp[k] = '\0'; // Il faut un caractère nul à la fin du string pour atoi
					nface[i] = atoi(strtmp) + nptsi - 1;

					j++; // Le prochain caractère lu doit être celui après l'espace
					i++;
				}

				if (quad)
				{
					Triangulate(nface, &(scene->face), &(scene->nfcs), &(scene->edge), &(scene->nedg), &(scene->normal));
				} else {
					NewFace(nface, &(scene->face), &(scene->nfcs), &(scene->edge), &(scene->nedg), &(scene->normal));
				}
			}
			if (fgets(line, 100, f) == NULL) { break; }
		}
		while (line[0] == 'l') // Tant qu'on lit des aretes
		{
			int j = 2;
			int nedge[2];

			for (int i = 0; i < 2; i++)
			{
				int k = 0;
				char strtmp[20]; // Temp string for reading numbers

				while (line[j] != ' ' && line[j] != '\n')
				{
					strtmp[k] = line[j];
					j++; k++;
				}
				strtmp[k] = '\0'; // Il faut un caractère nul à la fin du string pour atoi
				nedge[i] = atoi(strtmp) + nptsi - 1;

				j++; // Le prochain caractère lu doit être celui après l'espace
			}

			NewEdge(nedge, &(scene->edge), &(scene->nedg));
			if (fgets(line, 100, f) == NULL) { break; }
		}
	}
	DelDoubleEdges(&(scene->edge), &(scene->nedg));
	PrintPoints(scene->point, scene->npts);
	PrintEdges(scene->edge, scene->nedg);
	PrintFaces(scene->face, scene->nfcs);
	CalculateNormals(scene->normal, scene->point, scene->npts, scene->edge, scene->nedg, scene->face, scene->nfcs);
	PrintNormals(scene->normal, scene->nfcs);
}

