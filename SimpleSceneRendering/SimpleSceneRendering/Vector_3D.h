#pragma once
#include <cmath>

using namespace std;

class Vector_3D
{
	double X = 0; // ���������� �� ��� X
	double Y = 0; // ���������� �� ��� Y
	double Z = 0; // ���������� �� ��� Z

public:
	// �������������
	Vector_3D();  // ����� ������ ������
	Vector_3D(double newX, double newY, double newZ);  // ����� ������ �� �������� �����������
	Vector_3D(const Vector_3D& anotherVector); // ����� ������ � ������������, ����������� ���������� ������� �������
	~Vector_3D(); // ����������

	// �������� �������� ��� ���������
	Vector_3D operator + (const Vector_3D& anotherVector) const; // �������� ���� ��������
	Vector_3D operator - (const Vector_3D& anotherVector) const; // ��������� ������ ������� �� �������
	Vector_3D operator * (double multiplier) const; // ��������� ������� �� �����
	Vector_3D operator / (double divisor) const; // ������� ������� �� �����

	// �������������� ��������
	double length() const; // ����� (������) �������
	Vector_3D normalize() const; // ��� ������� (������������ �������������)

	friend double dot(const Vector_3D& firstVector, const Vector_3D& secondVector); // ��������� ������������ ���� ��������
	friend Vector_3D cross(const Vector_3D& firstVector, const Vector_3D& secondVector); // ��������� ������������ ���� ��������
	friend Vector_3D reflect(const Vector_3D& vector, const Vector_3D& polygonNormal); // ��������� �������
};



