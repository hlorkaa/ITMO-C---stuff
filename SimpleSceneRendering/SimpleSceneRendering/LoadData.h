#pragma once
#include <iostream>
#include <sstream>

//#include "Material.h"
#include "LightSource.h"
//#include "Vector_3D.h"
#include "Polygon.h"

void loadMaterials(vector<Material> materials);

void loadLightSources(vector<LightSource> lightSources);

void loadGeometry(vector<Vector_3D> points, vector<Polygon> polygons, vector<int> objectIDs, vector<Material> materials);