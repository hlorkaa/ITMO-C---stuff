#pragma once
#include "Vector_3D.h"
#include <map>

using namespace std;

class LightSource
{
    Vector_3D position; // ��������� ��������� ����� � ������������
    double totalIntensity = 0; // ������������� ��������� (W/sr)
    map <int, double> spectralIntensity; // ������������� ��������� �� ������ ���� (int - �����, double - �������������)

public:
    // �������������
    LightSource(); // ����� ������ ��������
    LightSource(const Vector_3D& newPosition, double newTotalIntensity); // ����� �������� � �������� � ��������������
    LightSource(const Vector_3D& newPosition, double newTotalIntensity, const map<int, double> newSpectralIntensity); //����� �������� � ��������, �������������� � ������������� ����������������
    ~LightSource(); // ����������

    // ������������� ������
    friend class Ray;
    friend class Scene;
};

