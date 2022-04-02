#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Alumno {
    char nombre[12];
    char apellido[12];
public:
    void setData() {
        cout << "Nombre: ";
        cin >> nombre;
        cout << "Apellidos: ";
        cin >> apellido;

    }
    
    void showData() {
        cout << nombre << " - " << apellido << endl;
    }
};

class FixedRecordFile {
    string filename;
public: 
    FixedRecordFile(string filename): filename(filename) {};

    void writeRecord(Alumno record) {
        ofstream file(filename, ios::app | ios::binary);
        if (!file.is_open()) throw("No se pudo abrir archivo");
        file.write((char*) &record, sizeof(record));
        file.close();
    }

    vector<Alumno> scanAll() {
        ifstream file(filename, ios::binary);
        if (!file.is_open()) throw("No se pudo abrir archivo");

        vector<Alumno> alumnos;
        Alumno record;
        while (!file.eof()) {
            record = Alumno();
            file.read((char*) &record, sizeof(Alumno));
            alumnos.push_back(record);
        }

        return alumnos;
    }

    Alumno readRecord(int pos) {
        ofstream file(filename);

        file.seekp(25*pos, ios::beg);
        
    }
};

int main() {

}
