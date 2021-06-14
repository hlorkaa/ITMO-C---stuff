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
	bool nearestIntersection(Ray& ray, const vector<Polygon>& polygons, Vector_3D& hit, Vector_3D& N, Material& material); // Если есть пересечения с лучом из камеры, возвращает ближайшее
	Ray castRay(Ray ray, const vector<Polygon>& polygons, const vector<LightSource>& lightSources, int depth = 0); // Запускает луч, меняет его характеристики в зависимости от того, что с ним произошло по пути
	Ray createRayFromCamera(const double x, const double y, const Vector_3D camera_cords, const vector<LightSource> lightSources); // Создает луч из камеры

public:
	// Инициализация
	Scene(); // Конструктор сцены
	~Scene(); // Деструктор

	void render(const vector<Polygon>& polygons, const vector<LightSource>& lightSources); // Рендеринг сцены методом обратной трассировки
};

