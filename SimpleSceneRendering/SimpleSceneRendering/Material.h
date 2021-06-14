#pragma once
#include <map>

using namespace std;

class Material
{
    map <int, double> diffuseReflection; // Коэффициент диффузного отражения по спектру (int длина волны, double цвет)
    double specularReflection = 0; // Коэффициент зеркального отражения
    double alphaChannel = 1; // Коэффициент прозрачности (альфа-канал)

public:
    // Инициализация
    Material(); // Пустой материал
    Material(const map <int, double>& newDiffuseReflection); // Материал с диффузным отражением
    Material(const map <int, double>& newDiffuseReflection, double newSpecularReflection, double newAlphaChannel); // Материал с диффузным и зеркальным отражением и прозрачностью
    ~Material(); // Деструктор

    //Дружественные классы
    friend class Scene;
};

