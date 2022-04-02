#include <iostream>
#include <vector>
#include <fstream>
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

    int ciclo;
    float mensualidad;
    int nextDel;

    Alumno() {};
    Alumno(string cod, string nom, string ape, string carr, int cic, float mens) {
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
        ciclo = cic;
        mensualidad = mens;
        nextDel = -1;
    }

    void show() {
        printChar(codigo, 5);
        cout << ", ";
        printChar(nombre, 11);
        cout << ", ";
        printChar(apellidos, 20);
        cout << ", ";
        printChar(carrera, 15);
        cout << ciclo << ", " << mensualidad << ", " << nextDel;
        cout << endl;
    }
};

class FixedRecord {
    string nombre;

public:
    FixedRecord(string nombre): nombre(nombre) {};

    vector<Alumno> load() {
        ifstream file(nombre, ios::binary);
        vector<Alumno> alumnos;
        Alumno record;

        file.seekg(sizeof(int), ios::beg);

        //string codigo = "20201", nombre = "Francisco", apellidos = "Magot Barrera", carrera = "CS";
        while (!file.eof()) {
            file.read((char*) & record, sizeof(record));
            record.show();
            if (record.nextDel == -1) alumnos.push_back(record);
        }

        return alumnos;
    }

    void add(Alumno record) {
        ofstream file(nombre, ios::app | ios::binary);

        file.write((char*) &record, sizeof(record));
        file.close();
    }

    Alumno readRecord(int pos) {
        ifstream file(nombre, ios::binary);
        Alumno record;


        file.seekg(sizeof(int) + pos*sizeof(record));
        file.read((char*) & record, sizeof(record));

        //if (record.nextDel != -1) throw("Elemento eliminado");
        return record;
    }

    bool del(int pos) {
        cout << "deleting" << endl;
        fstream file(nombre, ios::app | ios::binary);
        int nextDel;
        Alumno record;

        file.seekg(0, ios::beg);
        file.read(reinterpret_cast<char*>(&nextDel), sizeof(nextDel));
        cout << nextDel << endl;
        record = readRecord(pos);
        record.show();

        switch (nextDel) {
            case -1: 
                record.nextDel = -2;
                break;
            default: record.nextDel = nextDel;
        }

        file.seekg(0, ios::beg);
        file.write(reinterpret_cast<char*>(&pos), sizeof(pos));
        file.seekg(sizeof(int) + pos*sizeof(Alumno), ios::beg);
        file.write((char*) & record, sizeof(record));

        return true;
    }
};

void setup() {
    ofstream file("p2.dat", ios::binary);
    int def = -1;

    file.write(reinterpret_cast<char*>(&def), sizeof(def));
    FixedRecord fr("p2.dat");

    
    Alumno record("2345", "Javier", "Barrera", "CS", 5, (float) 4000);
    fr.add(record);

    fr.readRecord(0).show();

}

int main() {
    FixedRecord fr("p2.dat");

    vector<Alumno> alumnos = fr.load();
    cout << "loaded" << endl;
    for (Alumno a: alumnos) {
        a.show();
    }
    cout << "showed" << endl;
    cout << "reading first record" << endl;
    Alumno record2 = fr.readRecord(0);
    record2.show();
    cout << "read first record" << endl;


    fr.del(0);
    cout << "deleted" << endl;
    cout << "reading first record" << endl;
    record2 = fr.readRecord(0);
    record2.show();
    cout << "read first record" << endl;
    */
}