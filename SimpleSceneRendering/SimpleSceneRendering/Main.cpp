#include "Scene.h"
#include "LoadData.h"

using namespace std;

int main() {
    vector<Material> materials;
    loadMaterials(materials);

    vector<Vector_3D> points;
    vector<Polygon> polygons;
    vector<int> objectIDs;
    loadGeometry(points, polygons, objectIDs, materials);

    vector<LightSource> lightSources;
    loadLightSources(lightSources);

    Scene scene;
    //278, 548.7, -279.5
    cout << "Rendering scene..." << endl;
    scene.render(polygons, lightSources);
    cout << "Scene rendered!" << endl;

    return 0;
}