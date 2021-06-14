#include "Ray.h"

Ray::Ray() {}

Ray::Ray(const Vector_3D& newOrigin, const Vector_3D& newDirection) {
    this->origin = newOrigin;
    this->direction = newDirection;
}

Ray::Ray(const Vector_3D& newOrigin, const Vector_3D& newDirection, const map<int, double>& new_ray_spectralIntensity, const map<int, double>& newDensity) {
    this->origin = newOrigin;
    this->direction = newDirection;
    this->ray_spectralIntensity = new_ray_spectralIntensity;
    this->density = newDensity;
}

Ray::~Ray() {}


void Ray::fillWavelength(vector<LightSource> lightSources) {
    map <int, double> new_ray_spectralIntensity;
    map <int, double> newL;

    for (unsigned int i = 0; i < lightSources.size(); i++) {
        for (auto& item : lightSources[i].spectralIntensity) {
            new_ray_spectralIntensity.insert(make_pair(item.first, item.second)); // Для равноинтенсивного излучения (item.first, 1)
            newL.insert(make_pair(item.first, 0));
        }
    }

    ray_spectralIntensity = new_ray_spectralIntensity;
    density = newL;
}
