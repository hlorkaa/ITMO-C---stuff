#include "Material.h"

Material::Material() {}

Material::Material(const map <int, double>& newDiffuseReflection) {
	this->diffuseReflection = newDiffuseReflection;
}

Material::Material(const map <int, double>& newDiffuseReflection, double newSpecularReflection, double newAlphaChannel) {
	this->diffuseReflection = newDiffuseReflection;
	this->specularReflection = newSpecularReflection;
	this->alphaChannel = newAlphaChannel;
}

Material::~Material() {}