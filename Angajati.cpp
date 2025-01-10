// Angajati.cpp - Implementari pentru clasele legate de angajati
#include "Angajati.h"
#include <map>
#include <string>

// Dictionar pentru traducerea functiilor
const std::map<std::string, std::string> traduceriFunctii = {
        {"Manager", "Manager"},
        {"Barista", "Barista"},
        {"Ospatar", "Waiter"}
};

// Functie pentru traducerea functiilor
std::string traduceFunctie(const std::string& functie, bool inEngleza) {
    if (inEngleza) {
        auto it = traduceriFunctii.find(functie);
        if (it != traduceriFunctii.end()) {
            return it->second; // Traducere in engleza
        }
    } else {
        for (const auto& pair : traduceriFunctii) {
            if (pair.second == functie) {
                return pair.first; // Traducere in romana
            }
        }
    }
    return functie; // Daca nu exista traducerea, returnam functia originala
}

// Constructori si destructor pentru clasa Angajat
Angajat::Angajat() : nume(""), functie(""), oraStart(""), oraEnd("") {}

Angajat::Angajat(const std::string& nume, const std::string& functie, const std::string& oraStart, const std::string& oraEnd)
        : nume(nume), functie(functie), oraStart(oraStart), oraEnd(oraEnd) {}

Angajat::~Angajat() {}

// Getteri
std::string Angajat::getNume() const { return nume; }
std::string Angajat::getFunctie() const { return functie; }
std::string Angajat::getOraStart() const { return oraStart; }
std::string Angajat::getOraEnd() const { return oraEnd; }

// Setteri
void Angajat::setNume(const std::string& n) { this->nume = n; }
void Angajat::setFunctie(const std::string& f) { this->functie = f; }
void Angajat::setOraStart(const std::string& os) { this->oraStart = os; }
void Angajat::setOraEnd(const std::string& oe) { this->oraEnd = oe; }

// Metoda pentru afisarea detaliilor despre angajat
void Angajat::afisareDetalii() const {
    std::cout << "Nume: " << nume
              << ", Functie: " << functie
              << " (" << traduceFunctie(functie, true) << ")"  // Traducere in engleza
              << ", Tura: " << oraStart << " - " << oraEnd << std::endl;
}

