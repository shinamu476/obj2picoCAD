
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<vector>
#include <string>
#include<fstream>
#include <cstdlib>

using namespace std;
std::vector<std::string> split(std::string str, char del) {
    int first = 0;
    int last = str.find_first_of(del);

    std::vector<std::string> result;

    while (first < str.size()) {
        std::string subStr(str, first, last - first);

        result.push_back(subStr);

        first = last + 1;
        last = str.find_first_of(del, first);

        if (last == std::string::npos) {
            last = str.size();
        }
    }

    return result;
}
class Vector3
{
public:
    float x;
    float y;
    float z;
    Vector3() { x= y= z = 0; }
    Vector3(float v1, float v2, float v3) { x = v1; y = v2; z = v3; }
   void print() { cout << x << " , " << y << " , " << z << endl; }
   char* toString();
   char* toString(float size);
private:

};
char* Vector3::toString() {
    char buffer[256];
    sprintf(buffer, "%f,%f,%f", x, y,z);
    return buffer;
}
char* Vector3::toString(float size) {
    char buffer[256];
    sprintf(buffer, "%f,%f,%f", x*size, y * size, z * size);
    return buffer;
}

class Vector2
{
public:
   float x;
   float y;
    Vector2() { x = y = 0; }
    Vector2(float v1, float v2) { x = v1; y = v2; }
    void print() { cout << x << " , " << y <<  endl; }
    char* toString();
private:

};
char*  Vector2::toString() {
    char buffer[256];
    sprintf(buffer, "%f,%f", x, y);
    return buffer;
}

#define PI 3.141592653589
vector<float> en(int n, int i, float r0) {
  float r= r0 * pow(2,0.5);
    float d = ((float)i / n) * 2*PI;
    d += (PI/4);
    return { round((cos(d) * r+r0+1) *1000)/1000, round((sin(d) * r +r0)*1000)/1000};
 }


//--------------------------------------------------------
//  フルパスからファイル名を返す
//--------------------------------------------------------
string GetFileName(string fullpath) {
    int path_i = fullpath.find_last_of("\\") + 1;
    int ext_i = fullpath.find_last_of(".");
    std::string pathname = fullpath.substr(0, path_i + 1);//0文字目から７文字切り出す "C:\\aaa\\"
    std::string extname = fullpath.substr(ext_i, fullpath.size() - ext_i); // 10文字目から４文字切り出す ".txt"
    std::string filename = fullpath.substr(path_i, ext_i - path_i);// ７文字目から３文字切り出す　"bbb"
    return filename;
}
void maxMin(Vector3 data, float* maxX, float* minX, float* maxY, float* minY, float* maxZ, float* minZ) {
    *maxX = max(*maxX, data.x);
    *minX = min(*minX, data.x);
    *maxY = max(*maxY, data.y);
    *minY = min(*minY, data.y);
    *maxZ = max(*maxZ, data.z);
    *minZ = min(*minZ, data.z);
}

int main(int argc, char* argv[]) {

    cout << "obj2picoCAD v1.00 by.sinamu\n\n" << endl;
    char* txt = new char[4096];
    vector< Vector3> v;
    vector<vector<int>> f;
    string fullpath;
    char s0[1024];


    //---------------------scan---------------------------------------
    if (argc == 1) {
        cout << "Please enter the path of the obj file." << endl;
        cout << "path : ";
        getline(cin, fullpath);
    }
    else {
        fullpath = argv[1];
    }


    //---------------------scanSize---------------------------------------
    float size0, size;
    cout << "\n\nPlease enter the converted size." << endl;
    cout << "The value of the largest coordinate is the standard." << endl;
    cout << "size : ";
    cin >> size0;


    //--------------------- meshMode---------------------------------------
    int meshMode;
    cout << "Front or Back?" << endl;
    cout << "Front:0 , Back:1" << endl;
    cout << "\"1\" recommended" << endl;
    cout << "Front or Back : ";
    cin >> meshMode;

    //---------------------fileOpen---------------------------------------
    FILE* fp;
    fopen_s(&fp, fullpath.c_str(), "r");


    if (fp == 0) {
        cout << "File read error" << endl;
        system("pause");
        return 0;
    }


    const int BUF_SIZE = 1024;
    char buffer[BUF_SIZE];
    float maxX=-20000, minX=20000, maxY = -20000, minY = 20000, maxZ = -20000, minZ = 20000;

    //---------------------read---------------------------------------
    while (fgets(buffer, BUF_SIZE, fp) != nullptr)
    {
        float v1, v2, v3;
        if (sscanf_s(buffer, "v %f %f %f", &v1, &v2, &v3)) {
            v.push_back({ v1,v2 ,v3  });
            maxMin({ v1,v2 ,v3 }, &maxX, &minX, &maxY, &minY, &maxZ, &minZ);
        }else if (buffer[0]=='f') {
            vector<string>s = split(buffer, ' ');
            vector<int> f0;
            for (int i = 1; i < s.size(); i++) {
                int num;
                sscanf_s(s[i].c_str(), "%d", &num);
                f0.push_back(num);
            }
            f.push_back(f0);
        }

    }

    size = size0/max(abs(maxX - minX), max(abs(maxY - minY), abs(maxZ - minZ)));
    /*
    //---------------------Debag--------------------------------------
    for (int i = 0; i < v.size(); i++) {
        cout << "v" << i << " ";
        v[i].print();
    }
    for (int i = 0; i < f.size(); i++) {
        cout << "f" << i<<" ";
        for (int j = 0; j < f[i].size(); j++) {
           
            cout << f[i][j] << " ";
        }
        cout << "\n";
    }
    */


    string filename = GetFileName(fullpath);


    ofstream outputfile(filename +".txt");



    //------------wite------------------------------------------------------------

    //---fileSettings---
    outputfile << "{\n name=\'"<< filename<<"\',pos={0,0,0},rot={0,0,0},\n";

    //----v------
    outputfile << " v={\n";
    for (int i = 0; i < v.size(); i++) {
        outputfile << "  {" << v[i].x*size<<","<<v[i].y * size <<","<<v[i].z * size << "}";
        if (i + 1 != v.size())  outputfile << ",\n";
        else outputfile << "\n";
    }
    outputfile << " },\n";

    //----f------
    outputfile << " f={\n";
    for (int i = 0; i < f.size(); i++) {
        outputfile << "  {";
        for (int j = 0; j < f[i].size(); j++) {
            if (meshMode == 0) { //正順
                outputfile << f[i][j] << ",";
            }
            else {              //逆順
                outputfile << f[i][f[i].size() - j - 1] << ",";
            }
        }
        outputfile <<" c=10, uv={";
        for (int j = 0; j < f[i].size(); j++) {
            auto d = en(f[i].size(), j, 1);
            outputfile << d[0] << "," << d[1];
            if (j + 1 != f[i].size()) outputfile << ",";
            else outputfile << "} ";
        }
        outputfile << "}";
        if (i + 1 != f.size())  outputfile << ",\n";
        else outputfile << "\n";
    }
    outputfile << " }\n}";





    outputfile.close();
    system("pause");
    return 0;
}

