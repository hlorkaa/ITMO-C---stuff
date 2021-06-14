#pragma once
#include "Vector_3D.h"
#include <map>

using namespace std;

class LightSource
{
    Vector_3D position; // Положение источника света в пространстве
    double totalIntensity = 0; // Интенсивность излучения (W/sr)
    map <int, double> spectralIntensity; // Интенсивность излучения по длинам волн (int - длина, double - интенсивность)

public:
    // Инициализация
    LightSource(); // Новый пустой источник
    LightSource(const Vector_3D& newPosition, double newTotalIntensity); // Новый источник с позицией и интенсивностью
    LightSource(const Vector_3D& newPosition, double newTotalIntensity, const map<int, double> newSpectralIntensity); //Новый источник с позицией, интенсивностью и спектральными характеристиками
    ~LightSource(); // Деструктор

    // Дружественные классы
    friend class Ray;
    friend class Scene;
};

