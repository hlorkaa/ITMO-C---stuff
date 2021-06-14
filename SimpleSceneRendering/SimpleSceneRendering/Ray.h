#pragma once
//#include "Vector_3D.h"
#include "LightSource.h"
//#include "Polygon.h"
#include <map>
#include <vector>

using namespace std;

class Ray
{
    Vector_3D origin; // Координаты источника луча
    Vector_3D direction; // Координаты направления луча
    map <int, double> ray_spectralIntensity; // Интенсивность излучения по длинам волн (int - длина, double - интенсивность)
    map <int, double> density; // Плотность (яркость) излучения по длинам волн (int - длина, double - интенсивность)
    int intersectedPolygonObjectID = -1; // ID объекта, к которому принадлежит пересеченный лучом полигон

    void fillWavelength(vector<LightSource> lightSources); // Передача спектральных характеристик лучу от источника, испустившего его

public:
    // Инициализация
    Ray(); // Пустой луч
    Ray(const Vector_3D& newOrigin, const Vector_3D& newDirection); // Луч с началом и направлением
    Ray(const Vector_3D& newOrigin, const Vector_3D& newDirection, const map<int, double>& new_ray_spectralIntensity, const map<int, double>& newL); // Луч с началом, направлением и спектральными характеристиками
    ~Ray(); // Деструктор


    //friend bool Polygon::isIntersected(const Ray& ray, double& t) const;
    // Вызывает критические ошибки, вместо объявления дружественных функций решено объявлять дружественные классы, хоть это и менее корректно
    // TODO: выявить и устранить причину поведения, переписать классы на friend функции

    // Дружественные классы
    friend class Polygon;
    friend class Scene;
};

