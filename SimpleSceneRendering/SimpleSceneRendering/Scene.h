#pragma once

#define _USE_MATH_DEFINES
#include <random>
//#include <cmath>
#include <fstream>

#include "Ray.h"
#include "Material.h"
#include "Polygon.h"

using namespace std;

class Scene
{
	bool nearestIntersection(Ray& ray, const vector<Polygon>& polygons, Vector_3D& hit, Vector_3D& N, Material& material); // ���� ���� ����������� � ����� �� ������, ���������� ���������
	Ray castRay(Ray ray, const vector<Polygon>& polygons, const vector<LightSource>& lightSources, int depth = 0); // ��������� ���, ������ ��� �������������� � ����������� �� ����, ��� � ��� ��������� �� ����
	Ray createRayFromCamera(const double x, const double y, const Vector_3D camera_cords, const vector<LightSource> lightSources); // ������� ��� �� ������

public:
	// �������������
	Scene(); // ����������� �����
	~Scene(); // ����������

	void render(const vector<Polygon>& polygons, const vector<LightSource>& lightSources); // ��������� ����� ������� �������� �����������
};

