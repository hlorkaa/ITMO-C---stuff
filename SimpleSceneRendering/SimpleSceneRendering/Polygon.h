#pragma once
//#include "Vector_3D.h"
#include "Material.h"
#include "Ray.h"

using namespace std;

class Polygon
{
	Vector_3D vertex_1, vertex_2, vertex_3; // ������� ��������
	int objectID = -1; // ID �������, � �������� ����������� �������
	Material material; // �������� ��������

	Vector_3D getNormal() const; // ������� � ��������
	Vector_3D getNormalByObserver(const Vector_3D& observer) const; // ������� � �������� ������������ �����������

public:
	// �������������
	Polygon(); // ������ �������
	Polygon(const Vector_3D& newVertex_1, const Vector_3D& newVertex_2, const Vector_3D& newVertex_3); // ������� � ���������
	Polygon(const Vector_3D& newVertex_1, const Vector_3D& newVertex_2, const Vector_3D& newVertex_3, const int& newObjectID); // ������� � ��������� � ID
	Polygon(const Vector_3D& newVertex_1, const Vector_3D& newVertex_2, const Vector_3D& newVertex_3, const int& newObjectID, const Material& newMaterial); // ������� � ���������, ID � ����������
	~Polygon(); // ����������

	bool isIntersected(const Ray& ray, double& t) const; // ��������� �� ������� ���� �����?

	// ������������� ������
	friend class Scene;
};

