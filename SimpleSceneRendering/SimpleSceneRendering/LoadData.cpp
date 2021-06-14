#include "LoadData.h"

using namespace std;

void loadMaterials(vector<Material> materials) {

    cout << "Loading hardcoded materials..." << endl;
    //Хардкод, так как в файле нет спектральных свойств поверхностей
    //Переделать в чтение из файла
    map <int, double> spec_Kd_color_brs_0;
    spec_Kd_color_brs_0.insert(make_pair(400, 0.343));
    spec_Kd_color_brs_0.insert(make_pair(500, 0.747));
    spec_Kd_color_brs_0.insert(make_pair(600, 0.74));
    spec_Kd_color_brs_0.insert(make_pair(700, 0.737));
    Material brs_0 = Material(spec_Kd_color_brs_0, 0, 20);

    map <int, double> spec_Kd_color_brs_1;
    spec_Kd_color_brs_1.insert(make_pair(400, 0.092));
    spec_Kd_color_brs_1.insert(make_pair(500, 0.285));
    spec_Kd_color_brs_1.insert(make_pair(600, 0.16));
    spec_Kd_color_brs_1.insert(make_pair(700, 0.159));
    Material brs_1 = Material(spec_Kd_color_brs_1, 0, 20);

    map <int, double> spec_Kd_color_brs_2;
    spec_Kd_color_brs_2.insert(make_pair(400, 0.04));
    spec_Kd_color_brs_2.insert(make_pair(500, 0.058));
    spec_Kd_color_brs_2.insert(make_pair(600, 0.287));
    spec_Kd_color_brs_2.insert(make_pair(700, 0.642));
    Material brs_2 = Material(spec_Kd_color_brs_2, 0, 20);

    map <int, double> spec_Kd_color_brs_3;
    spec_Kd_color_brs_3.insert(make_pair(400, 0.343));
    spec_Kd_color_brs_3.insert(make_pair(500, 0.747));
    spec_Kd_color_brs_3.insert(make_pair(600, 0.74));
    spec_Kd_color_brs_3.insert(make_pair(700, 0.737));
    Material brs_3 = Material(spec_Kd_color_brs_3, 0.5, 20);

    map <int, double> spec_Kd_color_brs_4;
    spec_Kd_color_brs_4.insert(make_pair(400, 0.343));
    spec_Kd_color_brs_4.insert(make_pair(500, 0.747));
    spec_Kd_color_brs_4.insert(make_pair(600, 0.74));
    spec_Kd_color_brs_4.insert(make_pair(700, 0.737));
    Material brs_4 = Material(spec_Kd_color_brs_4, 0.5, 20);

    materials.push_back(brs_0);
    materials.push_back(brs_1);
    materials.push_back(brs_2);
    materials.push_back(brs_3);
    materials.push_back(brs_4);

    cout << "Hardcoded materials loaded!" << endl;
}

void loadLightSources(vector<LightSource> lightSources) {

    cout << "Loading hardcoded light sources..." << endl;
    //Хардкод, так как в файле нет свойств и положения источника света
    //Переделать в чтение из файла
    int total_intensity = 1445872; // W/sr
    double Itotal = 2100;
    double Isim_400 = 0;
    double Isim_500 = 400;
    double Isim_600 = 780;
    double Isim_700 = 920;

    map <int, double> spec_intensity;

    spec_intensity.insert(make_pair(400, total_intensity * (Isim_400 / Itotal)));
    spec_intensity.insert(make_pair(500, total_intensity * (Isim_500 / Itotal)));
    spec_intensity.insert(make_pair(600, total_intensity * (Isim_600 / Itotal)));
    spec_intensity.insert(make_pair(700, total_intensity * (Isim_700 / Itotal)));

    lightSources.push_back(LightSource(Vector_3D(278, 548.7, -279.5), 1445872, spec_intensity));

    cout << "Hardcoded light sources loaded!" << endl;
}

void loadGeometry(vector<Vector_3D> points, vector<Polygon> polygons, vector<int> objectIDs, vector<Material> materials) {

    string s;
    ifstream file("cornel_box0.shp");

    if (file) {
        cout << ".shp file found! Reading data..." << endl;
    }
    else {
        cout << "ERROR: .shp file not found!" << endl;
        return;
    }

    int state = 0; //1 - define breps brs_ найден, 2 - Number of vertices найден, Number of triangles найден
    int points_size = 0;
    while (getline(file, s)) {

        if (s.find("Number of parts") != string::npos) {
            state = 0;
            continue;
        }

        if (s.find("Number of triangles") != string::npos) {
            state = 3;
            continue;
        }

        if (state == 3) {
            istringstream iss(s);
            vector<int> number_of_triangles;
            for (std::string s; iss >> s;)
                number_of_triangles.push_back(stoi(s));

            Polygon polygon = Polygon(points[points_size + number_of_triangles[0]],
                points[points_size + number_of_triangles[1]],
                points[points_size + number_of_triangles[2]],
                objectIDs[objectIDs.size() - 1],
                materials[objectIDs[objectIDs.size() - 1]]
            );
            polygons.push_back(polygon);
            continue;
        }

        if (s.find("define breps brs_") != string::npos) {
            istringstream iss(s);
            string token;
            while (getline(iss, token, '_')) {}
            objectIDs.push_back(stoi(token));
            state = 1;
            continue;
        }

        if (s.find("Number of vertices") != string::npos) {
            state = 2;
            points_size = points.size();
            continue;
        }

        if (state == 2) {
            istringstream iss(s);
            vector<double> cords;
            for (std::string s; iss >> s; )
                cords.push_back(stod(s));
            Vector_3D new_point = Vector_3D(cords[0], cords[1], cords[2]);
            points.push_back(new_point);
        }
    }

    file.close();
    cout << "Geometry loaded!" << endl;

    cout << polygons.size() << endl;
}