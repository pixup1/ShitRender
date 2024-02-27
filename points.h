#pragma once

struct Scene{
    int npts; // Nombre de points
    double** point; // Tableau à 2 dimensions des points, modélisé par un pointeur vers les points, puis un autre vers les coordonnées. Il est vide au début. Logique.
    
    int nedg; // Nombre d'aretes
    int** edge; // Tableau à deux dimension d'aretes, une arete est modélisée par un pointeur vers deux valeurs, qui sont les deux ids des points reliés.
    
    int nfcs; // Nombre de faces
    int** face; // Tableau à deux dimension de faces, une face est modélisée par un pointeur vers trois valeurs, qui sont les trois ids des points reliés.
    double** normal; // On va avoir des normales yipee !!!!!!!!!!!!!!!! Alors bon les normales sont modélisées par trois dimensions, logique c'est un vecteur, et l'id sera le même que la face correspondante.

    double campos[3];
    double camrot[3];

    double sensorsize; // Camera sensor size in mm (will be applied to whichever dimension is the smallest)
    double focall; // Camera focal length in mm

    boolean renderedges; // Render in wireframe if true. It's kinda shit
};

int NewPoint(double npoint[], double*** point, int* npts); // Creates a new point at npoint[] coordinates in the *point table and increments *npts. Returns point id.

void DelPoint(int id, double*** point, int* npts); // Deletes *point[id] from the table *point and decrements *npts.

void FreePoints(double*** point, int* npts); // Frees memory for (and deletes) all the points in table point

void PrintPoints(double** point, int npts); // Prints a list of point positions.


int NewEdge(int nedge[2], int*** edge, int* nedg); // Creates an edge between the two points which's ids are in nedge[]. Returns edge id.

void DelEdge(int id, int*** edge, int* nedg); // Deletes an edge using its id.

void DelDoubleEdges(int*** edge, int* nedg); // Deletes all edges that are present twice

void FreeEdges(double*** edge, int* nedg); // Frees memory for (and deletes) all the edges in table edge

void PrintEdges(int** edge, int nedg); // Prints a list of edge ids.


int NewFace(int nface[3], int*** face, int* nfcs, int*** edge, int* nedg, double*** normal); // Creates a face between the three points which's ids are in nface[]. Returns face id. (lol get it)

int Triangulate(int nface[4], int*** face, int* nfcs, int*** edge, int* nedg, double*** normal); // Creates two triangle faces from points ids in nface[] of one quad face. Returns face id.

void DelFace(int id, int*** face, int* nfcs, double*** normal); // Deletes a face using its id. Does not delete any edges.

void FreeFaces(double*** face, int* nfcs, double*** normal); // Frees memory for (and deletes) all the faces in table face

void PrintFaces(int** face, int nfcs); // Prints a list of face ids.


void CalculateNormals(double** normal, double** point, int npts, int** edge, int nedg, int** face, int nfcs); // Calculates all normals of faces in face array, only works for strictly manifold meshes.

void RecursNormal(double** normal, int* nNormal, int** face, int** faceadj, double** point, int id, boolean ascending); // Internal recursaive normal calculation function used by CalculateNormals(). Calculates normal of face at id, and adj faces if they have not been calculated yet.

void PrintNormals(double** normal, int nfcs); // *sigh*


void SortFaces(int* index, int** face, int nfcs, double** point, double ref[3]); // Sorts faces in order of vector ref[], the sorted ids will be put into index[]. Only works in 3d I decided. Really, that could be fixed in like 20 seconds. I just don't care.

void QuickSortX(int* index, double** avrface, int left, int right); // Recursive af dawg, quoicoubeh. Sorts points in order of the first dimension of the elements in face, but actually just writes the ordered indexes in, you guessed it, my ass. I mean index.

void NormalizeVector(double v[]); // ...Yeah


void RotatePoint3d(double rot[], double* point); // Rotates one 3d point on each axis by the 3 factors in rot[], which should be in radians

void MovePoint(double delta[], double* point); // Moves one point by delta

void RotateObject3d(double rot[], double** point, int npts); // Rotates 3d points in point on each axis by the 3 factors in rot[], which should be in radians

void MoveObject(double delta[], double** point, int npts); // Moves all points by delta

void switchyz(double point[]); // Switches 1st and 2nd dimension of a point

void Cube(double position[], double size, double*** point, int* npts, int*** edge, int* nedg); // Creates a cube around position[] of side size. To do : add rotation