#pragma once
//#include "Vector_3D.h"
#include "Material.h"
#include "Ray.h"

using namespace std;

class Polygon
{
	Vector_3D vertex_1, vertex_2, vertex_3; // Вершины полигона
	int objectID = -1; // ID объекта, к которому принадлежит полигон
	Material material; // Материал полигона

	Vector_3D getNormal() const; // Нормаль к полигону
	Vector_3D getNormalByObserver(const Vector_3D& observer) const; // Нормаль к полигону относительно наблюдателя

public:
	// Инициализация
	Polygon(); // Пустой полигон
	Polygon(const Vector_3D& newVertex_1, const Vector_3D& newVertex_2, const Vector_3D& newVertex_3); // Полигон с вершинами
	Polygon(const Vector_3D& newVertex_1, const Vector_3D& newVertex_2, const Vector_3D& newVertex_3, const int& newObjectID); // Полигон с вершинами и ID
	Polygon(const Vector_3D& newVertex_1, const Vector_3D& newVertex_2, const Vector_3D& newVertex_3, const int& newObjectID, const Material& newMaterial); // Полигон с вершинами, ID и материалом
	~Polygon(); // Деструктор

	bool isIntersected(const Ray& ray, double& t) const; // Пересечен ли полигон этим лучом?

	// Дружественные классы
	friend class Scene;
};

