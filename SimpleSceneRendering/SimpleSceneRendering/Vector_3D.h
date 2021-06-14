#pragma once
#include <cmath>

using namespace std;

class Vector_3D
{
	double X = 0; // Координата по оси X
	double Y = 0; // Координата по оси Y
	double Z = 0; // Координата по оси Z

public:
	// Инициализация
	Vector_3D();  // Новый пустой вектор
	Vector_3D(double newX, double newY, double newZ);  // Новый вектор по заданным координатам
	Vector_3D(const Vector_3D& anotherVector); // Новый вектор с координатами, копирующими координаты другого вектора
	~Vector_3D(); // Деструктор

	// Линейные операции над векторами
	Vector_3D operator + (const Vector_3D& anotherVector) const; // Сложение двух векторов
	Vector_3D operator - (const Vector_3D& anotherVector) const; // Вычитание одного вектора из другого
	Vector_3D operator * (double multiplier) const; // Умножение вектора на число
	Vector_3D operator / (double divisor) const; // Деление вектора на число

	// Дополнительные операции
	double length() const; // Длина (модуль) вектора
	Vector_3D normalize() const; // Орт вектора (нормализация существующего)

	friend double dot(const Vector_3D& firstVector, const Vector_3D& secondVector); // Скалярное произведение двух векторов
	friend Vector_3D cross(const Vector_3D& firstVector, const Vector_3D& secondVector); // Векторное произведение двух векторов
	friend Vector_3D reflect(const Vector_3D& vector, const Vector_3D& polygonNormal); // Отражение вектора
};



