#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <wingdi.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "windowsshit.h"
#include "points.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // Fonction qui gère les messages envoyés par Windows par rapport à la fenêtre (ex: fermeture, OK pour dessiner, changement de taille...)
{
	struct Scene* scene = (struct Scene*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	
	// Steup another buffer to draw stuff to
	static HDC hdcBackBuffer = NULL;
	static HBITMAP hbmBackBuffer = NULL;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT clientRect;

	switch (uMsg)
	{
		case WM_CREATE:
		{
			// Save the Scene pointer in the window user data
			LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)lpcs->lpCreateParams);
			
			// Do the other buffer stuff
			GetClientRect(hwnd, &clientRect);
			hdc = GetDC(hwnd);
			hdcBackBuffer = CreateCompatibleDC(hdc);
			hbmBackBuffer = CreateCompatibleBitmap(hdc, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
			SelectObject(hdcBackBuffer, hbmBackBuffer);
			ReleaseDC(hwnd, hdc);

			return 0;
		}

		case WM_SIZE: // Cas ou l'utilisateur (ou Windows) resize la fenêtre. Il faut créer un nouveau backbuffer car sinon on ne le crée que lors de la création de la fenêtre, et il faut le recréer car on ne peut paas changer dynamiquement sa taille (enfin je crois)
		{
			// Retrieve the new client area size
			GetClientRect(hwnd, &clientRect);

			// Delete the old off-screen bitmap and device context
			DeleteObject(hbmBackBuffer);
			DeleteDC(hdcBackBuffer);

			// Create a new off-screen bitmap with the new size
			hdc = GetDC(hwnd);
			hdcBackBuffer = CreateCompatibleDC(hdc);
			hbmBackBuffer = CreateCompatibleBitmap(hdc, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
			SelectObject(hdcBackBuffer, hbmBackBuffer);
			ReleaseDC(hwnd, hdc);
			
			return 0;
		}

		case WM_PAINT: // Cas où Windows nous dit qu'on peut dessiner dans la fenêtre
		{
			GetClientRect(hwnd, &clientRect);
			int wLength = clientRect.right - clientRect.left;
			int wHeight = clientRect.bottom - clientRect.top;
			int ldim = min(wLength, wHeight); // Limiting dimension, will be the one corresponding to the FOV
			double FOV = 2 * atan((scene->sensorsize) / (2 * scene->focall)); // FOV in radians

			hdc = BeginPaint(hwnd, &ps);
			BitBlt(hdcBackBuffer, 0, 0, wLength, wHeight, NULL, 0, 0, WHITENESS);

			// Draw the stuff
			if (scene->renderedges)
			{
				for (int i = 0; i < scene->nedg; i++)
				{
					double point1[3];
					double point2[3];

					for (int j = 0; j < 3; j++) { // Copy coordinates from edge points to our points
						point1[j] = scene->point[scene->edge[i][0]][j]; // First point of the edge
						point2[j] = scene->point[scene->edge[i][1]][j]; // Second point
					}

					// Put point1 and point2 in camera referential
					MovePoint((double[]) { -(scene->campos[0]), -(scene->campos[1]), -(scene->campos[2]) }, point1);
					RotatePoint3d((double[]) { -(scene->camrot[0]) + M_PI_2, -(scene->camrot[1]), -(scene->camrot[2]) }, point1);

					MovePoint((double[]) { -(scene->campos[0]), -(scene->campos[1]), -(scene->campos[2]) }, point2);
					RotatePoint3d((double[]) { -(scene->camrot[0]) + M_PI_2, -(scene->camrot[1]), -(scene->camrot[2]) }, point2);

					//if ((point1[1]<0 && point2[1]<0)==FALSE) // Don't render stuff that's behind the camera // Whoopsie that doesn't work
					//{
						// Find in-window effective point coordinates // d = (vraiedistance*(ldim/2))/(2*tan(FOV/2)*distanceàlacam) --> startx = (scene->point[scene->edge[i][0]][0]*(ldim/2)) / (2*tan(FOV/2)*scene->point[scene->edge[i][0]][1]) + wLength/2;
					int startx = (point1[0] * ldim) / (2 * tan(FOV / 2) * point1[1]) + wLength / 2;
					int starty = -(point1[2] * ldim) / (2 * tan(FOV / 2) * point1[1]) + wHeight / 2;

					int endx = (point2[0] * ldim) / (2 * tan(FOV / 2) * point2[1]) + wLength / 2;
					int endy = -(point2[2] * ldim) / (2 * tan(FOV / 2) * point2[1]) + wHeight / 2;

					// Draw line
					MoveToEx(hdcBackBuffer, startx, starty, NULL); // Start point
					LineTo(hdcBackBuffer, endx, endy); // End point
					//}
				}
			} else {
				//render faces
			}
			BitBlt(hdc, 0, 0, wLength, wHeight, hdcBackBuffer, 0, 0, SRCCOPY);
			EndPaint(hwnd, &ps);
			return 0;
		}

		case WM_TIMER: // Cas du timer, arrive 60 fois par seconde max
		{
			// Update the state of the game
			// ...

			//system("cls");
			
			RotateObject3d((double[]) {0.0,0.0,0.1}, scene->point, scene->npts);
			//scene->camrot[0] += 0.01;

			// Request a repaint of the window
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		}

		case WM_SETCURSOR:
		{
			// If the mouse is over the client area, set the cursor to the arrow cursor instead of whatever the fuck DefWindowProc does
			if (LOWORD(lParam) == HTCLIENT)
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				return 0;
			} else {
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
		}

		case WM_DESTROY: // Cas où Windows nous dit que l'utilisateur a appuyé sur le bouton de fermeture de la fenêtre
		{
			FreePoints(&(scene->point), &(scene->npts));
			FreeEdges(&(scene->edge), &(scene->nedg));
			FreeFaces(&(scene->face), &(scene->nfcs), &(scene->normal));
			DeleteDC(hdcBackBuffer); // These two are the only ones that I specifically alloted a value to, so they're the only GDI objects I actually need to delete (or not I have no clue what I'm doing)
			DeleteObject(hbmBackBuffer);
			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam); // Fonction WindowProc par défaut, qui ici s'occupera de tous les autres messages qu'on n'a pas traités
		}
	}

	return 0;
}