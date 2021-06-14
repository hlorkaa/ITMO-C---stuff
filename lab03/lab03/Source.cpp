#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

int main(){

    random_device rd;
    mt19937 generator(rd());

    int numberOfRays;
    double R;
    int mode;
    cout << "Choose mode (equal = 0, lambert = 1): ";
    cin >> mode;

    cout << "Enter number of rays: ";
    cin >> numberOfRays;

    R = 15;
    cout << "Sphere radius: " << R << endl;

    int maxRandVal = 10'000'000;

    ofstream out;
    out.open("light.ray");

    out << "Rayset\n\tName sphere\n\tColorModel RGB\n\tNormalFlux 1000\n\tScale 1\n\tRayNumber " << numberOfRays << "\n\tRayFlux Relative\n\tRays\n" << endl;


    for (int i = 0; i < numberOfRays; i++) {
        double randomForTheta = (generator() % maxRandVal) / double(maxRandVal);
        double randomForPhi = (generator() % maxRandVal) / double(maxRandVal);

        double theta = 2 * M_PI * randomForTheta;
        double phi = acos(1 - 2 * randomForPhi);

        double x;
        double y;
        double z;
        if (mode == 0) {
            x = sin(phi) * cos(theta) * R;
            y = sin(phi) * sin(theta) * R;
            z = cos(phi) * R;
        }
        else {
            double normX = 0;
            double normY = 1;
            double normZ = 0;

            x = sin(phi) * cos(theta) + normX;
            y = sin(phi) * sin(theta) + normY;
            z = cos(phi) + normZ;
            double length = sqrt(x * x + y * y + z * z);

            x = (x / length) * R;
            y = (y / length) * R;
            z = (z / length) * R;
        }


        out << x << "\t" << y << "\t" << z << "\t" << x << "\t" << y << "\t" << z << "\t" << "1 1 1\n";
    }

    out.close();
    cout << "DONE!" << endl;
    return 0;
}