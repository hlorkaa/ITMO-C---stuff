#include "LightSource.h"

LightSource::LightSource() {}

LightSource::LightSource(const Vector_3D& newPosition, double newTotalIntensity) {
	position = newPosition;
	totalIntensity = newTotalIntensity;
}

LightSource::LightSource(const Vector_3D& newPosition, double newTotalIntensity, const map<int, double> newSpectralIntensity) {
	position = newPosition;
	totalIntensity = newTotalIntensity;
	spectralIntensity = newSpectralIntensity;
}

LightSource::~LightSource() {}