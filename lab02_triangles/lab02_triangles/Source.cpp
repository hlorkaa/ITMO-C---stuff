#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

struct point {
    double x = 0;
    double y = 0;
    double z = 0;
};


void triangleRays() {

    random_device rd;
    mt19937 generator(rd());

    //Получаем данные
    int numberOfRays;
    point A, B, C;

    cout << "Enter number of rays: ";
    cin >> numberOfRays;
    cout << endl;

    //Данные для варианта 8
    A.x = 20;
    A.y = 0;
    A.z = 10;
    cout << "First point [x, y, z] coords: [" << A.x << ", " << A.y << ", " << A.z << "]" << endl;

    B.x = 0;
    B.y = 0;
    B.z = 0;
    cout << "Second point [x, y, z] coords: [" << B.x << ", " << B.y << ", " << B.z << "]" << endl;

    C.x = 30;
    C.y = 0;
    C.z = 0;
    cout << "Third point [x, y, z] coords: [" << C.x << ", " << C.y << ", " << C.z << "]" << endl;

    point AB;
    AB.x = B.x - A.x;
    AB.y = B.y - A.y;
    AB.z = B.z - A.z;

    point AC;
    AC.x = C.x - A.x;
    AC.y = C.y - A.y;
    AC.z = C.z - A.z;

    int maxRandVal = 10'000'000;
    string appendix = "\t0.000\t1.000\t0.000\t1 1 1\n";

    ofstream out;
    out.open("light.ray");

    out << "Rayset\n\tName triangle\n\tColorModel RGB\n\tNormalFlux 100\n\tScale 1\n\tRayNumber " << numberOfRays << "\n\tRayFlux Relative\n\tRays\n" << endl;
    double x, y, z;
    for (int i = 0; i < numberOfRays; i++) {
        double randomForAB = generator() % maxRandVal;
        double randomForAC = generator() % maxRandVal;
        while (randomForAB + randomForAC > maxRandVal) {
            randomForAB = generator() % maxRandVal;
            randomForAC = generator() % maxRandVal;
        }
        randomForAB /= double(maxRandVal);
        randomForAC /= double(maxRandVal);

        x = randomForAB * AB.x + randomForAC * AC.x + A.x;
        y = randomForAB * AB.y + randomForAC * AC.y + A.y;
        z = randomForAB * AB.z + randomForAC * AC.z + A.z;
        out << "\t" << x << "\t" << y << "\t" << z << appendix;
    }

    out.close();
}

void circleRays() {

    random_device rd;
    mt19937 generator(rd());

    int numberOfRays;
    point A;

    cout << "Enter number of rays: ";
    cin >> numberOfRays;

    A.x = 0;
    A.y = 0;
    A.z = 0;
    cout << "Circle center [x, y, z] coords: [" << A.x << ", " << A.y << ", " << A.z << "]" << endl;

    double R = 15;
    cout << "Circle radius: " << R << endl;

    int maxRandVal = 10'000'000;
    string appendix = "\t0.000\t1.000\t0.000\t1 1 1\n";

    ofstream out;
    out.open("light.ray");

    out << "Rayset\n\tName circle\n\tColorModel RGB\n\tNormalFlux 100\n\tScale 1\n\tRayNumber " << numberOfRays << "\n\tRayFlux Relative\n\tRays\n" << endl;
    double phi, randomR_coef, x, z;

    for (int i = 0; i < numberOfRays; i++) {
        double randomForPhi = (generator() % maxRandVal) / double(maxRandVal);
        double randomForR = (generator() % maxRandVal) / double(maxRandVal);

        phi = randomForPhi * 2.0 * M_PI;
        randomR_coef = sqrt(randomForR);

        x = cos(phi) * R * randomR_coef;
        z = sin(phi) * R * randomR_coef;

        out << "\t" << x + A.x << "\t" << A.y << "\t" << z + A.z << appendix;
    }

    out.close();
}

int main()
{
    int mode;
    cout << "Choose mode (0 = triangle, 1 = circle): ";
    cin >> mode;
    switch (mode) {
    case 0:
        cout << "Triangle mode!" << endl;
        triangleRays();
        break;
    case 1:
        cout << "Circle mode!" << endl;
        circleRays();
        break;
    default:
        cout << "ERROR";
        return 1;
    }

    cout << "Done!" << endl;

    return 0;
}