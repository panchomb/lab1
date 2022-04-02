#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void printChar(char c[], int size) {
    for (int i = 0; i < size;i++) {
        cout << c[i];
    }
}

struct Alumno
{
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];

    Alumno() {};
    Alumno(string cod, string nom, string ape, string carr) {
        for (int i = 0; i < 5; i++) {
            codigo[i] = (i < cod.size())?cod[i]:' ';
        }
        codigo[4] = '\0';
        for (int i = 0; i < 11; i++) {
            nombre[i] = (i < nom.size())?nom[i]:' ';
        }
        nombre[10] = '\0';
        for (int i = 0; i < 20; i++) {
            apellidos[i] = (i < ape.size())?ape[i]:' ';
        }
        apellidos[19] = '\0';
        for (int i = 0; i < 15; i++) {
            carrera[i] = (i<carr.size())?carr[i]:' ';
        }
        carrera[14] = '\0';
    }

    void show() {
        cout << codigo << " " << nombre << " " << apellidos << " " << carrera << endl;
        /*
        printChar(codigo, 5);
        cout << ", ";
        printChar(nombre, 11);
        cout << ", ";
        printChar(apellidos, 20);
        cout << ", ";
        printChar(carrera, 15);
        cout << endl;
        */
    }
};

istream & operator>>(istream & stream, Alumno & record) {
    stream.get(record.codigo, 5);
    stream.get(record.nombre, 11);
    stream.get(record.apellidos, 20);
    stream.get(record.carrera, 15);
    return stream;
}

ostream & operator<<(ostream & stream, Alumno & record) {
    stream << record.codigo;
    stream << record.nombre;
    stream << record.apellidos;
    stream << record.carrera;
    stream << endl;
    stream << flush;
    return stream;
}

class FixedRecord {
    string nombre;

public:
    FixedRecord(string nombre): nombre(nombre) {};

    vector<Alumno> load() {
        ifstream file(nombre);
        vector<Alumno> alumnos;

        string record, codigo, nombre, apellidos, carrera;
        while (getline(file, record)) {
            codigo = record.substr(0, 5);
            nombre = record.substr(5, 11);
            apellidos = record.substr(16, 20);
            carrera = record.substr(36, 15);
            alumnos.emplace_back(codigo, nombre, apellidos, carrera);
        }
        file.close();
        return alumnos;
    }

    void add(Alumno record) {
        ofstream file(nombre, ios::app);

        file << record;

        file.close();
    }

    Alumno readRecord(int pos) {
        ifstream file(nombre);
        Alumno record;

        file.seekg(48*pos, ios::beg);
        file >> record;

        file.close();
        return record;
    }
};

void readFromConsole(char buffer [], int size) {
    string temp;
    cin >> temp;
    for (int i = 0; i < size; i++) {
        buffer[i] = (i < temp.size()) ? temp[i] : ' ';
    }
    buffer[size-1] = '\0';
    cin.clear();
}


int main() {
    FixedRecord archivo("records.txt");
    
    // Anadiendo nueva entrada a archivo
    Alumno record;
    readFromConsole(record.codigo, 5);
    readFromConsole(record.nombre, 11);
    readFromConsole(record.apellidos, 20);
    readFromConsole(record.carrera, 15);

    archivo.add(record);
    
    // Buscando entrada en posicion 1 de archivo
    archivo.readRecord(4).show();


    // Mostrando todas las entradas en el archivo
    vector<Alumno> alumnos = archivo.load();
    for (Alumno a: alumnos) {
        a.show();
    }

}