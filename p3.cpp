#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Alumno {
    string nombre;
    string apellidos;
    string carrera;
    float mensualidad;


    Alumno() {};
    Alumno(string n, string a, string c, float m) {
        nombre = n;
        apellidos = a;
        carrera = c;
        mensualidad = m;
    }

    void show() {
        cout << nombre <<", " << apellidos << ", " << carrera << ", " << mensualidad << endl;
    }
};

Alumno desempaquetar(string buffer) {
    string temp;
    vector<string> valores;
    for (auto &c: buffer) {
        if (c != '|') {
            temp += c;
        } else {
            valores.push_back(temp);
            temp = "";
        }
    }
    valores.push_back(temp);

    Alumno record;
    record.nombre = valores[0];
    record.apellidos = valores[1];
    record.carrera = valores[2];
    record.mensualidad = stof(valores[3]);
    return record;
}

class VariableRecord {
    string filename;

public:
    VariableRecord(string filename): filename(filename) {};

    vector<Alumno> load() {
        ifstream file(filename);
        vector<Alumno> alumnos;
        string buffer;

        while (getline(file, buffer)) {
            alumnos.push_back(desempaquetar(buffer));
        }

        return alumnos;
    }

    void add(Alumno record) {
        ofstream file(filename, ios::app);

        file << record.nombre << "|" << record.apellidos << "|" << record.carrera << "|" << record.mensualidad << endl;
    }

    Alumno read(int pos) {
        ifstream file(filename);
        string buffer;

        int i = 0;
        getline(file, buffer);
        while (i < pos) {
            i++;
            getline(file, buffer);
        }

        if (buffer.length() == 0) throw("Fuera de rango");

        return desempaquetar(buffer);
    }
};

int main() {
    VariableRecord vr("p3.txt");

    //Para ejectuar
    Alumno al("Heider", "Sanchez", "CS", 10000000.0);
    vr.add(al);

    vector<Alumno> alumnos = vr.load();

    for (Alumno a: alumnos) {
        a.show();
    }
}