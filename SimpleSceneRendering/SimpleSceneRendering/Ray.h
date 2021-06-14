#pragma once
//#include "Vector_3D.h"
#include "LightSource.h"
//#include "Polygon.h"
#include <map>
#include <vector>

using namespace std;

class Ray
{
    Vector_3D origin; // ���������� ��������� ����
    Vector_3D direction; // ���������� ����������� ����
    map <int, double> ray_spectralIntensity; // ������������� ��������� �� ������ ���� (int - �����, double - �������������)
    map <int, double> density; // ��������� (�������) ��������� �� ������ ���� (int - �����, double - �������������)
    int intersectedPolygonObjectID = -1; // ID �������, � �������� ����������� ������������ ����� �������

    void fillWavelength(vector<LightSource> lightSources); // �������� ������������ ������������� ���� �� ���������, ������������ ���

public:
    // �������������
    Ray(); // ������ ���
    Ray(const Vector_3D& newOrigin, const Vector_3D& newDirection); // ��� � ������� � ������������
    Ray(const Vector_3D& newOrigin, const Vector_3D& newDirection, const map<int, double>& new_ray_spectralIntensity, const map<int, double>& newL); // ��� � �������, ������������ � ������������� ����������������
    ~Ray(); // ����������


    //friend bool Polygon::isIntersected(const Ray& ray, double& t) const;
    // �������� ����������� ������, ������ ���������� ������������� ������� ������ ��������� ������������� ������, ���� ��� � ����� ���������
    // TODO: ������� � ��������� ������� ���������, ���������� ������ �� friend �������

    // ������������� ������
    friend class Polygon;
    friend class Scene;
};

