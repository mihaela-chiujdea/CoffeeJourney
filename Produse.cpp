// Produse.cpp - Implementari pentru clasele legate de produse si gestionarea stocurilor
#include "Produse.h"
#include <algorithm>
#include <fstream>

// Implementari pentru clasa Produs
Produs::Produs() : nume(""), traducereEngleza(""), pret(0.0), cantitate(0) {}

Produs::Produs(const std::string& nume, double pret, int cantitate)
        : nume(nume), traducereEngleza(""), pret(pret), cantitate(cantitate) {}

Produs::~Produs() {}

string Produs::getNume() const { return nume; }
void Produs::setNume(const std::string& n) {
    nume = n;
}

std::string Produs::getTraducereEngleza() const { return traducereEngleza; }
void Produs::setTraducereEngleza(const std::string& traducere) { this->traducereEngleza = traducere; }

double Produs::getPret() const { return pret; }
void Produs::setPret(double p) { this->pret = p; }

int Produs::getCantitate() const { return cantitate; }
void Produs::setCantitate(int c) { this->cantitate = c; }

void Produs::afisareDetalii() const {
    cout << "Produs: " << nume << ", Pret: " << pret << ", Cantitate: " << cantitate << endl;
}

// Implementari pentru clasa GestionareStoc
void GestionareStoc::adaugaSauActualizeazaProdus(const Produs& produs) {
    for (auto& p : produse) {
        if (p.getNume() == produs.getNume()) {
            p.setCantitate(p.getCantitate() + produs.getCantitate());
            p.setPret(produs.getPret());
            p.setTraducereEngleza(produs.getTraducereEngleza());
            return;
        }
    }
    produse.push_back(produs);
}

void GestionareStoc::eliminaProdus(const string& nume) {
    produse.erase(remove_if(produse.begin(), produse.end(), [&nume](const Produs& p) {
        return p.getNume() == nume;
    }), produse.end());
}

double GestionareStoc::calculeazaProfitZilnic(double costuri) const {
    double venit = 0;
    for (const auto& produs : produse) {
        venit += produs.getPret() * produs.getCantitate();
    }
    return venit - costuri;
}

void GestionareStoc::afisareStocuri() const {
    for (const auto& produs : produse) {
        produs.afisareDetalii();
    }
}

void GestionareStoc::raportareStocInsuficient(int prag) const {
    cout << "Produse cu stoc insuficient (mai putin de " << prag << " unitati):\n";
    for (const auto& produs : produse) {
        if (produs.getCantitate() < prag) {
            produs.afisareDetalii();
        }
    }
}

void GestionareStoc::citesteProduseDinCSV(const string& fileNameRo, const string& fileNameEn) {
    ifstream fileRo(fileNameRo);
    ifstream fileEn(fileNameEn);

    if (!fileRo.is_open() || !fileEn.is_open()) {
        cerr << "Eroare la deschiderea fisierelor " << fileNameRo << " si " << fileNameEn << " pentru citire!\n";
        return;
    }

    string lineRo, lineEn;

    while (getline(fileRo, lineRo) && getline(fileEn, lineEn)) {
        size_t pos1 = lineRo.find(",");
        size_t pos2 = lineRo.find_last_of(",");

        string numeRo = lineRo.substr(0, pos1);
        double pret = stod(lineRo.substr(pos1 + 1, pos2 - pos1 - 1));
        int cantitate = stoi(lineRo.substr(pos2 + 1));

        string numeEn = lineEn.substr(0, lineEn.find(","));

        Produs produs(numeRo, pret, cantitate);
        produs.setTraducereEngleza(numeEn);

        produse.push_back(produs);
    }

    fileRo.close();
    fileEn.close();
}

void GestionareStoc::scrieProduseInCSV(const string& fileNameRo, const string& fileNameEn) const {
    ofstream fileRo(fileNameRo);
    ofstream fileEn(fileNameEn);

    if (!fileRo.is_open() || !fileEn.is_open()) {
        cerr << "Eroare la deschiderea fisierelor " << fileNameRo << " si " << fileNameEn << " pentru scriere!\n";
        return;
    }

    for (const auto& produs : produse) {
        // Salvare in romana
        fileRo << produs.getNume() << "," << produs.getPret() << "," << produs.getCantitate() << endl;

        // Salvare in engleza
        fileEn << produs.getTraducereEngleza() << "," << produs.getPret() << "," << produs.getCantitate() << endl;
    }

    fileRo.close();
    fileEn.close();
}
