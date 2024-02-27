#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <wingdi.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "windowsshit.h"
#include "points.h"
#include "filehandling.h"

#define DIM 3 // Preprocessor constant for amount  of dimensions (everything will probably break if not 3)
#define IMPORT // Will try and import model.obj if defined

/*
TODO :
- Implement normals everywhere and figure out a way to find their orientation
  THE ONE THAT IS THE CLOSEST TO THE CAMERA NEEDS TO BE FACING TOWARDS THE CAMERA OF COURSE or THE ONE THAT IS FURTHEST ON THE Y DIRECTION NEEDS TO BE FACING IN THAT DIRECTION
  Maybe calculate only one normal in case of triangulation ? Actually i don't care

- Add a 4th value to each face to act as an index of the order of the faces for rendering (so like, if face[0][3] = 54, then the first faces to draw is face[54])(I'm a genius)(it could be in another array but why not do it in the same, right ? Because society fucking says so ? Fuck you
  and maybe don't generate that value if the normal point away from the camera or something, idk. EFFICIENCY !!!!!!

- Maybe a 5th value for color ? Or in the normals array ?
*/

/*int main()
{
	int index[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int pos[10];
	pos[0] = (int[]){ 9, 0, 0 };
	pos[1] = (int[]){ 5, 0, 0 };
	pos[2] = (int[]){ 4, 0, 0 };
	pos[3] = (int[]){ 6, 0, 0 };
	pos[4] = (int[]){ 7, 0, 0 };
	pos[5] = (int[]){ 2, 0, 0 };
	pos[6] = (int[]){ 1, 0, 0 };
	pos[7] = (int[]){ 3, 0, 0 };
	pos[8] = (int[]){ 8, 0, 0 };
	pos[9] = (int[]){ 0, 0, 0 };
	QuickSortX(index, pos, 0, 9);
	for (int i = 0; i < 10; i++) {
		printf_s("%d ", index[i]);
	}
}*/

int main()
{
	const int maxfps = 40;
	
	// Step 0: Setup 3d stuff
	struct Scene scene0;
	
	scene0.npts = 0;
	scene0.point = NULL;

	scene0.nedg = 0;
	scene0.edge = NULL;

	scene0.nfcs = 0;
	scene0.face = NULL;
	scene0.normal = NULL;

	//Cube((double[]) { 0, 0, 0 }, 2, &(scene0.point), &(scene0.npts), &(scene0.edge), &(scene0.nedg));
	//CalculateNormals(scene0.normal, scene0.point, scene0.npts, scene0.edge, scene0.nedg, scene0.face, scene0.nfcs);
	//PrintFaces(scene0.face, scene0.nfcs);
	//PrintNormals(scene0.normal, scene0.nfcs);

	scene0.campos[0] = 0;
	scene0.campos[1] = -6.5;
	scene0.campos[2] = 0;
	
	scene0.camrot[0] = M_PI_2;
	scene0.camrot[1] = 0;
	scene0.camrot[2] = 0;
	
	scene0.sensorsize = 36;
	scene0.focall = 50;

	scene0.renderedges = TRUE;

	// Step 0.75: Maybe do the whole .obj import shtick
	#ifdef IMPORT
	FILE* f = NULL;
	fopen_s(&f,"model.obj","r");
	if (f != NULL) {
		ReadObj(f, &scene0);
		fclose(f);
	}
	#endif

	// Step 1: Register the window class
	const char CLASS_NAME[] = "Main Window Class";

	WNDCLASS wc = { 0 };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Step 2: Create the window
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Sugondese",                   // Window text     // Le L juste avant les string pour passer le char* en wchar_t*
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,

		NULL,       // Parent window    
		NULL,       // Menu
		GetModuleHandle(NULL),  // Instance handle
		&scene0        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	// Step 3: Show the window
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	// Start timer
	SetTimer(hwnd, 1, 1000/maxfps, NULL); // 16 milliseconds = 60 FPS

	// Step 4: Run the message loop
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}