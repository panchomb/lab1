#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Matricula {
    string codigo;
    int ciclo;
    float mensualidad;
    string observaciones;

    Matricula() {};
    Matricula(string cod, int cic, float mens, string obs) {
        codigo = cod;
        ciclo = cic;
        mensualidad = mens;
        observaciones = obs;
    }

    int sizeBytes() {
        int size = 0;
        size += sizeof(char)*codigo.length();
        size += sizeof(int);
        size += sizeof(float);
        size += sizeof(char)*observaciones.length();
        return size;
    }

    void show() {
        cout << codigo << ", " << ciclo << ", " << mensualidad << ", " << observaciones << endl;
    }
};

string empaquetar(Matricula record) {
    string buffer = "";
    buffer += record.codigo;
    buffer += "|";
    buffer += to_string(record.ciclo);
    buffer += "|";
    buffer += to_string(record.mensualidad);
    buffer += "|";
    buffer += record.observaciones;

    return buffer;
}


Matricula desempaquetar(char* & buffer, int size) {
    string temp;
    vector<string> valores;
    for (int i = 0; i < size; i++) {
        if (buffer[i] != '|') {
            temp += buffer[i];
        } else {
            valores.push_back(temp);
            temp = "";
        }
    }
    valores.push_back(temp);

    Matricula record;
    record.codigo = valores[0];
    record.ciclo = stoi(valores[1]);
    record.mensualidad = stof(valores[2]);
    record.observaciones = valores[3];

    return record;
}

struct EntryMeta {
    long position;
    int size;


    EntryMeta() {};
    EntryMeta(long pos, int s) {
        position = pos;
        size = s;
    }
};

class VariableRecord {
    string headerfile;
    string filename;

public:
    VariableRecord(string filename): filename(filename + ".txt"), headerfile(filename + ".dat") {};

    vector<Matricula> load() {
        ifstream file(filename);
        ifstream header(headerfile, ios::binary);
        vector<Matricula> matriculas;
        Matricula record;
        EntryMeta em;

        while (!header.eof()) {
            header.read((char*) & em, sizeof(em));
            file.seekg(em.position, ios::beg);
            file.read((char*) & record, em.size);
            matriculas.push_back(record);
        }
        
        return matriculas;
    }

    void add(Matricula record) {
        ofstream file(filename, ios::app);
        ofstream header(headerfile, ios::app | ios::binary);
        EntryMeta recordMeta;

        file.seekp(0, ios::end);
        int position = file.tellp();
        int size = record.sizeBytes();
        recordMeta = EntryMeta(position, size);

        string buffer = empaquetar(record);
        file << buffer << endl;

        header.seekp(0, ios::end);
        header.write((char*) & recordMeta, sizeof(recordMeta));
    }

    Matricula readRecord(int pos) {
        cout << "reading" << endl;
        ifstream file(filename);
        ifstream header(headerfile, ios::binary);
        EntryMeta recordMeta;
        Matricula record;

        header.seekg(pos*(sizeof(EntryMeta)));
        header.read((char*) & recordMeta, sizeof(EntryMeta));
        file.seekg(recordMeta.position);
        char* buffer = new char[recordMeta.size];
        file.read(buffer, recordMeta.size);


        record = desempaquetar(buffer, recordMeta.size);
        return record;
    }
};

int main() {
    VariableRecord vr("p4");

    Matricula m("202010308", 5, 4000.0, "Ninguna");

    vr.add(m);

    vr.readRecord(0).show();
}