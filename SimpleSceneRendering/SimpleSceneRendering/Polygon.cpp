#include "Polygon.h"

#include "Material.h"
#include "Ray.h"

Polygon::Polygon() {}

Polygon::Polygon(const Vector_3D& newVertex_1, const Vector_3D& newVertex_2, const Vector_3D& newVertex_3) {
    this->vertex_1 = newVertex_1;
    this->vertex_2 = newVertex_2;
    this->vertex_3 = newVertex_3;
}

Polygon::Polygon(const Vector_3D& newVertex_1, const Vector_3D& newVertex_2, const Vector_3D& newVertex_3, const int& newObjectID) {
    this->vertex_1 = newVertex_1;
    this->vertex_2 = newVertex_2;
    this->vertex_3 = newVertex_3;
    this->objectID = newObjectID;
}

Polygon::Polygon(const Vector_3D& newVertex_1, const Vector_3D& newVertex_2, const Vector_3D& newVertex_3, const int& newObjectID, const Material& newMaterial) {
    this->vertex_1 = newVertex_1;
    this->vertex_2 = newVertex_2;
    this->vertex_3 = newVertex_3;
    this->objectID = newObjectID;
    this->material = newMaterial;
}

Polygon::~Polygon() {}

Vector_3D Polygon::getNormal() const {
    Vector_3D normal = cross(vertex_2 - vertex_1, vertex_3 - vertex_1);
    normal = normal.normalize();
    return normal;
}

Vector_3D Polygon::getNormalByObserver(const Vector_3D& observer) const {
    Vector_3D normal = cross(vertex_2 - vertex_1, vertex_3 - vertex_1);
    normal = normal.normalize();

    if (dot(observer.normalize(), normal) < 0) { // Если угол между нормалью полигона и нормалью наблюдателя тупой
        normal = cross(vertex_3 - vertex_1, vertex_2 - vertex_1).normalize(); // Берем относительно другой стороны
    }

    return normal;
}

bool Polygon::isIntersected(const Ray& ray, double& t) const {
    Vector_3D side12 = vertex_2 - vertex_1;
    Vector_3D side13 = vertex_3 - vertex_1;
    Vector_3D pVec = cross(ray.direction, side13);
    double det = dot(side12, pVec);

    if (det < 1e-8 && det > -1e-8) return 0; // Луч параллелен полигону

    double inv_det = 1 / det;
    Vector_3D tVec = ray.origin - vertex_1;
    double u = dot(tVec, pVec) * inv_det;
    if (u < 0 || u > 1) return 0; // Луч не попал в полигон

    Vector_3D qvec = cross(tVec, side12);
    double v = dot(ray.direction, qvec) * inv_det;
    if (v < 0 || v + u > 1) return 0; // Луч не попал в полигон

    t = dot(side13, qvec) * inv_det; // Точка пересечения
    return t > 1e-8;
}