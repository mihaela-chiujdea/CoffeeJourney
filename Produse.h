#ifndef PRODUSE_H
#define PRODUSE_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

// Clasa Produs
class Produs {
private:
    std::string nume;               // Nume produs in romana
    std::string traducereEngleza;   // Nume produs in engleza
    double pret;                    // Pret produs
    int cantitate;                  // Cantitate in stoc

public:
    // Constructori
    Produs();
    Produs(const std::string& nume, double pret, int cantitate);

    // Destructor
    ~Produs();

    // Getteri
    std::string getNume() const;
    std::string getTraducereEngleza() const;
    double getPret() const;
    int getCantitate() const;

    // Setteri
    void setNume(const std::string& n);
    void setTraducereEngleza(const std::string& traducere);
    void setPret(double p);
    void setCantitate(int c);

    // Afisare detalii produs
    void afisareDetalii() const;
};

// Clasa GestionareStoc
class GestionareStoc {
private:
    vector<Produs> produse; // Lista de produse gestionate

public:
    // Constructor si destructor
    GestionareStoc();
    ~GestionareStoc();

    // Functii de gestionare stoc
    void adaugaSauActualizeazaProdus(const Produs& produs);
    void eliminaProdus(const std::string& nume);
    double calculeazaProfitZilnic(double costuri) const;
    void afisareStocuri() const;
    void raportareStocInsuficient(int prag) const;

    // Functii pentru CSV
    void citesteProduseDinCSV(const std::string& fileNameRo, const std::string& fileNameEn);
    void scrieProduseInCSV(const std::string& fileNameRo, const std::string& fileNameEn) const;

    // Getteri si setteri pentru lista de produse
    const std::vector<Produs>& getProduse() const;
    void setProduse(const std::vector<Produs>& produse);
};

#endif
