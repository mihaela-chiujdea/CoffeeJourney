#include "Evenimente.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

// Constructori
Eveniment::Eveniment() : nume(""), traducereEngleza(""), data(""), costuri(0.0), venituri(0.0) {}

Eveniment::Eveniment(const std::string& nume, const std::string& data, double costuri, double venituri)
        : nume(nume), traducereEngleza(""), data(data), costuri(costuri), venituri(venituri) {}

// Destructor pentru Eveniment
Eveniment::~Eveniment() {
    // Curata resursele evenimentului daca este cazul
}

// Destructor pentru GestionareEvenimente
GestionareEvenimente::~GestionareEvenimente() {
    // Curata lista de evenimente sau alte resurse daca este cazul
}

// Constructor implicit pentru GestionareEvenimente
GestionareEvenimente::GestionareEvenimente() {
    // Initializeaza resursele daca este cazul
}

// Getter si setter pentru nume
std::string Eveniment::getNume() const { return nume; }
void Eveniment::setNume(const std::string& nume) { this->nume = nume; }

// Getter si setter pentru traducere engleza
std::string Eveniment::getTraducereEngleza() const { return traducereEngleza; }
void Eveniment::setTraducereEngleza(const std::string& traducere) { this->traducereEngleza = traducere; }

// Getter si setter pentru data
std::string Eveniment::getData() const { return data; }
void Eveniment::setData(const std::string& data) { this->data = data; }

// Getter si setter pentru costuri
double Eveniment::getCosturi() const { return costuri; }
void Eveniment::setCosturi(double costuri) { this->costuri = costuri; }

// Getter si setter pentru venituri
double Eveniment::getVenituri() const { return venituri; }
void Eveniment::setVenituri(double venituri) { this->venituri = venituri; }

// Calcularea profitului
double Eveniment::calculeazaProfit() const { return venituri - costuri; }

// Afisarea detaliilor evenimentului
void Eveniment::afiseazaDetalii() const {
    std::cout << "Eveniment: " << nume
              << " (" << traducereEngleza << ")"
              << "\nData: " << data
              << "\nCosturi: " << std::fixed << std::setprecision(2) << costuri
              << "\nVenituri: " << venituri
              << "\nProfit: " << calculeazaProfit() << " lei\n";
}

// GestionareEvenimente: Adaugare eveniment
void GestionareEvenimente::adaugaEveniment(const Eveniment& eveniment) {
    evenimente.push_back(eveniment);
}

// GestionareEvenimente: stergere eveniment
void GestionareEvenimente::eliminaEveniment(const std::string& nume) {
    try {
        auto it = std::remove_if(evenimente.begin(), evenimente.end(),
                                 [&nume](const Eveniment& eveniment) { return eveniment.getNume() == nume; });
        if (it == evenimente.end()) throw std::runtime_error("Evenimentul nu a fost gasit!");

        evenimente.erase(it, evenimente.end());
    } catch (const std::exception& e) {
        std::cerr << "Eroare la stergerea evenimentului: " << e.what() << "\n";
    }
}


// GestionareEvenimente: Afisare evenimente
void GestionareEvenimente::afiseazaToateEvenimentele() const {
    for (const auto& eveniment : evenimente) {
        eveniment.afiseazaDetalii();
        std::cout << "========================\n";
    }
}

// GestionareEvenimente: Calculare profit total
double GestionareEvenimente::calculeazaProfitTotal() const {
    double profitTotal = 0.0;
    for (const auto& eveniment : evenimente) {
        profitTotal += eveniment.calculeazaProfit();
    }
    return profitTotal;
}

// Getter pentru lista de evenimente
const std::vector<Eveniment>& GestionareEvenimente::getEvenimente() const {
    return evenimente;
}

