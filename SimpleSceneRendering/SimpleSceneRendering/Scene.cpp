#include "Scene.h"

Scene::Scene() {}

Scene::~Scene() {}

bool Scene::nearestIntersection(Ray& ray, const vector<Polygon>& polygons, Vector_3D& hit, Vector_3D& normal, Material& material) {

    double polygon_dist = numeric_limits<double>::max(); // ����������� ��������� ���������, ������� ������ � double

    for (unsigned int i = 0; i < polygons.size(); i++) {
        double t;
        // ���� � ������� �������� ��� � �� ��������� �� ���������� ������ �������� ������������, �� ����� ���
        if (polygons[i].isIntersected(ray, t) && t < polygon_dist) {
            polygon_dist = t; 
            hit = ray.origin + ray.direction * t;
            normal = polygons[i].getNormalByObserver(ray.origin - hit);
            material = polygons[i].material;
            ray.intersectedPolygonObjectID = polygons[i].objectID;
        }
    }
    return polygon_dist < numeric_limits<double>::max();
}

Ray Scene::castRay(Ray ray, const vector<Polygon>& polygons, const vector<LightSource>& lightSources, int reflectionCounter) {
    Vector_3D hitPoint, normal;
    Material material;

    if (reflectionCounter > 3 || !nearestIntersection(ray, polygons, hitPoint, normal, material)) { // ���� ��� ��������� ������ ���� ��� ��� ������ �� �����, ����������
        return ray;
    }

    // ���� ����� �� ���������, �� ��� ����� � ������� � ������ ���� ��������

    for (auto& item : material.diffuseReflection) { // �������� ������������ ������������� ���� �� ��������� ��������� ���������
        ray.ray_spectralIntensity.find(item.first)->second = (ray.ray_spectralIntensity.find(item.first)->second) * item.second;
    }

    Vector_3D cameraDirection = (ray.origin - hitPoint).normalize();

    // ���������� ���
    Vector_3D reflectionDirection = reflect(ray.direction.normalize(), normal.normalize()).normalize();
    Vector_3D reflectionOrigin = hitPoint + normal * 1e-3;
    Ray reflectedRay = Ray(reflectionOrigin, reflectionDirection);
    reflectedRay.fillWavelength(lightSources);

    if (material.specularReflection > 0) { // ���� �������� �������� �������� � ������� ��������� �� ��������������� � �� �������� �������� � ���������, ��������� ���������� ���������� ���
        if (dot(reflectionDirection, normal) > 0) {
            reflectedRay = castRay(reflectedRay, polygons, lightSources, reflectionCounter + 1);
        }
    }

    bool isMaterialSpecular = false;
    double diffuse = 0;
    double specular = 0;

    // ��� ������� ��������� �����
    for (unsigned int i = 0; i < lightSources.size(); i++) {
        Vector_3D lightDirection = (lightSources[i].position - hitPoint).normalize();
        Vector_3D lightDirection_reverse = (hitPoint - lightSources[i].position).normalize();
        double distance = (lightSources[i].position - hitPoint).length();
        double cos_theta = dot(lightDirection, normal);
        bool includeDiffuse = true;
        if (cos_theta <= 0) { // ���� �������� ��������� � �������� ������� �� ��������, ������� ��������� ��������� �� ����
            includeDiffuse = false;
        }

        double BRDF_diffuseCoeff = 0;
        double BRDF_specularCoeff = 0;

        // ������� ���
        Vector_3D shadowOrigin = hitPoint + normal * 1e-3;
        Ray shadowRay = Ray(shadowOrigin, lightDirection);
        Vector_3D shadowHit, shadowNormal;
        Material shadowMaterial;

        if (nearestIntersection(shadowRay, polygons, shadowHit, shadowNormal, shadowMaterial) && (shadowHit - shadowOrigin).length() < distance) {
            // ���� ����, ������� ��������� ��������� �� ����
            includeDiffuse = false;
        }

        for (auto& item : ray.density) {
            double illuminationLevel = (((lightSources[i].spectralIntensity.find(item.first)->second) * cos_theta) / (distance * distance)); // ������������

            if (includeDiffuse == true) {
                BRDF_diffuseCoeff = (ray.ray_spectralIntensity.find(item.first)->second); // ��������� ���������
            }

            BRDF_specularCoeff = (pow(max(0., dot(reflect(lightDirection_reverse, normal), cameraDirection)), material.alphaChannel) * material.specularReflection); // ���������� ���������

            double BRDF = BRDF_diffuseCoeff + BRDF_specularCoeff;

            item.second = ((illuminationLevel * BRDF) / (double)M_PI) + ((reflectedRay.density.find(item.first)->second)) * material.specularReflection; // ��������� ���������
        }
    }

    return ray;
}

Ray Scene::createRayFromCamera(const double x, const double y, const Vector_3D camera_cords, const vector<LightSource> lightSources) {

    Vector_3D direction = Vector_3D(x, y, -1).normalize(); // -1 � Z �� ������ ���������� ����� � �����
    Ray ray = Ray(camera_cords, direction);
    ray.fillWavelength(lightSources);
    return ray;
}

void Scene::render(const vector<Polygon>& polygons, const vector<LightSource>& lightSources) {
    const int width = 1024;
    const int height = 768;
    const double fov = M_PI / 3.;
    vector <Ray> framebuffer(width * height);
    vector <map <int, double>> Lbuffer(width * height);
    Vector_3D camera = Vector_3D(250, 300, 500);

    vector<int> wavelengths;
    wavelengths.push_back(400);
    wavelengths.push_back(500);
    wavelengths.push_back(600);
    wavelengths.push_back(700);

#pragma omp parallel for // ����������� ���� �� ����� ���������� OpenMP
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            //��������� ��� �� ������ � ����� "�������", ��������� ��������
            Ray renderingRay;
            double x_center = -(2 * (j + 0.5) / (double)width - 1) * tan(fov / 2.) * width / (double)height;
            double y_center = -(2 * (i + 0.5) / (double)height - 1) * tan(fov / 2.);
            renderingRay = createRayFromCamera(x_center, y_center, camera, lightSources);

            framebuffer[j + i * width] = castRay(renderingRay, polygons, lightSources);
            Lbuffer[j + i * width] = framebuffer[j + i * width].density;
        }
    }

    ofstream fout("results.txt");

    for (unsigned int k = 0; k < wavelengths.size(); k++) { // ����� � .txt ������ �� ���������� �� ������ ����
        fout << "wavelength" << " " << wavelengths[k] << endl;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0) {
                    fout << (Lbuffer[j + i * width].find(wavelengths[k])->second) / 100;
                }
                else {
                    fout << " " << (Lbuffer[j + i * width].find(wavelengths[k])->second) / 100;
                }
            }
            fout << endl;
        }
        fout << "\n";
    }

    fout.close();
}