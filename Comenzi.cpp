#include "Comenzi.h"
#include "Produse.h"
#include <algorithm>
#include <iomanip>

// Implementari pentru clasa Comanda
Comanda::Comanda() : numeClient(""), total(0.0) {}

Comanda::Comanda(const std::string& numeClient) : numeClient(numeClient), total(0.0) {}

// Destructor pentru Comanda
Comanda::~Comanda() {
    // Curata produsele sau alte resurse daca este cazul
}

void Comanda::adaugaProdus(const std::string& numeProdus, int cantitate, double pretUnitar) {
    produse.emplace_back(numeProdus, cantitate);
    total += cantitate * pretUnitar;
}

double Comanda::calculeazaTotal() const {
    return total;
}

void Comanda::aplicaReducere(double procent) {
    total -= total * procent;
}

double Comanda::getTotal() const {
    return total;
}

std::string Comanda::getNumeClient() const {
    return numeClient;
}

const std::vector<std::pair<std::string, int>>& Comanda::getProduse() const {
    return produse;
}

void Comanda::setTotal(double t) {
    total = t;
}

// Implementari pentru clasa SistemComenzi
SistemComenzi::SistemComenzi(double reducereClientFidel) : reducereClientFidel(reducereClientFidel) {}

// Destructor pentru SistemComenzi
SistemComenzi::~SistemComenzi() {
    // Curata comenzile sau alte resurse daca este cazul
}


void SistemComenzi::adaugaComanda(const Comanda& comanda) {
    try {
        comenzi.push_back(comanda);
    } catch (const std::exception& e) {
        std::cerr << "Eroare la adaugarea comenzii: " << e.what() << "\n";
    }
}


void SistemComenzi::aplicaReducereClientFidel(const std::string& numeClient) {
    for (auto& comanda : comenzi) {
        if (comanda.getNumeClient() == numeClient) {
            double reducere = comanda.calculeazaTotal() * reducereClientFidel / 100.0;
            comanda.aplicaReducere(reducereClientFidel / 100.0);
            std::cout << "Reducere aplicata pentru " << numeClient << ": " << reducere << " lei." << std::endl;
        }
    }
}

void SistemComenzi::afiseazaToateComenzile(bool inEngleza, const std::vector<Produs>& produse) const {
    for (const auto& comanda : comenzi) {
        std::cout << "Comanda pentru clientul: " << comanda.getNumeClient() << std::endl;

        for (const auto& produs : comanda.getProduse()) {
            std::string numeProdus = produs.first;
            if (inEngleza) {
                auto it = std::find_if(produse.begin(), produse.end(), [&produs](const Produs& p) {
                    return p.getNume() == produs.first;
                });
                if (it != produse.end()) {
                    numeProdus = it->getTraducereEngleza();
                }
            }
            std::cout << " - " << numeProdus << ": " << produs.second << " buc." << std::endl;
        }
        std::cout << "Total: " << std::fixed << std::setprecision(2) << comanda.getTotal() << " lei" << std::endl;
        std::cout << "========================" << std::endl;
    }
}

double SistemComenzi::calculeazaVenituriTotale() const {
    double venituri = 0.0;
    for (const auto& comanda : comenzi) {
        venituri += comanda.getTotal();
    }
    return venituri;
}

const std::vector<Comanda>& SistemComenzi::getComenzi() const {
    return comenzi;
}
