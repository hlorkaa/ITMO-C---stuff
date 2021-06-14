#pragma once
#include <map>

using namespace std;

class Material
{
    map <int, double> diffuseReflection; // ����������� ���������� ��������� �� ������� (int ����� �����, double ����)
    double specularReflection = 0; // ����������� ����������� ���������
    double alphaChannel = 1; // ����������� ������������ (�����-�����)

public:
    // �������������
    Material(); // ������ ��������
    Material(const map <int, double>& newDiffuseReflection); // �������� � ��������� ����������
    Material(const map <int, double>& newDiffuseReflection, double newSpecularReflection, double newAlphaChannel); // �������� � ��������� � ���������� ���������� � �������������
    ~Material(); // ����������

    //������������� ������
    friend class Scene;
};

