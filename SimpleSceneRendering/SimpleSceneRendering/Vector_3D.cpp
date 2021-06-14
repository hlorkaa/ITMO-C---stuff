#include "Vector_3D.h"

Vector_3D::Vector_3D() {}

Vector_3D::Vector_3D(double newX, double newY, double newZ) {
    X = newX;
    Y = newY;
    Z = newZ;
}

Vector_3D::Vector_3D(const Vector_3D& anotherVector) {
    if (&anotherVector != this) {
        this->X = anotherVector.X;
        this->Y = anotherVector.Y;
        this->Z = anotherVector.Z;
    }
}

Vector_3D::~Vector_3D() {}



Vector_3D Vector_3D::operator + (const Vector_3D& anotherVector) const {
    return Vector_3D(X + anotherVector.X, Y + anotherVector.Y, Z + anotherVector.Z);
}

Vector_3D Vector_3D::operator - (const Vector_3D& anotherVector) const {
    return Vector_3D(X - anotherVector.X, Y - anotherVector.Y, Z - anotherVector.Z);
}

Vector_3D Vector_3D::operator * (double multiplier) const {
    return Vector_3D(X * multiplier, Y * multiplier, Z * multiplier);
}

Vector_3D Vector_3D::operator / (double divisor) const {
        return Vector_3D(X / divisor, Y / divisor, Z / divisor);
}

double Vector_3D::length() const {
    return sqrt(X * X + Y * Y + Z * Z);
}

Vector_3D Vector_3D::normalize() const {
    double length = Vector_3D::length();
    return Vector_3D(X / length, Y / length, Z / length);
}

double dot(const Vector_3D& firstVector, const Vector_3D& secondVector) {
    return (firstVector.X * secondVector.X + firstVector.Y * secondVector.Y + firstVector.Z * secondVector.Z);
}

Vector_3D cross(const Vector_3D& firstVector, const Vector_3D& secondVector) {
    return Vector_3D(
        firstVector.Y * secondVector.Z - firstVector.Z * secondVector.Y,
        firstVector.Z * secondVector.X - firstVector.X * secondVector.Z,
        firstVector.X * secondVector.Y - firstVector.Y * secondVector.X
    );
}

Vector_3D reflect(const Vector_3D& vector, const Vector_3D& polygonNormal) {
    return vector - polygonNormal * (2 * (dot(vector, polygonNormal)));
}