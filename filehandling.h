#pragma once

void ReadObj(FILE* f, struct Scene* scene); // Imports points from ASCII obj f* into scene*. Must only be tris and quads. Also it might not work if there are certain data types in obj file. idk I did like no research