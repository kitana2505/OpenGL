#pragma once
#include "big.h"
#include "medium.h"
#include "small.h"
const int fire_NVertices = fire_bigNVertices + fire_mediumNVertices + fire_smallNVertices;
const int fire_NTriangles = fire_bigNTriangles + fire_mediumNTriangles + fire_smallNTriangles;
const int fire_NAttribsPerVertex = 8;
extern float fire_Vertices[];
extern unsigned fire_Triangles[];