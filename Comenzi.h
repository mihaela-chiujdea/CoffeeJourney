#ifndef COMENZI_H
#define COMENZI_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include "Produse.h"

// Clasa Comanda
class Comanda {
private:
    std::string numeClient; // Numele clientului
    std::vector<std::pair<std::string, int>> produse; // Lista de produse (nume, cantitate)
    double total; // Totalul comenzii

public:
    // Constructori si destructor
    Comanda();
    Comanda(const std::string& numeClient);
    ~Comanda();

    // Functionalitati
    void adaugaProdus(const std::string& numeProdus, int cantitate, double pretUnitar);
    double calculeazaTotal() const;
    void aplicaReducere(double procent);

    // Getteri
    std::string getNumeClient() const;
    double getTotal() const;
    const std::vector<std::pair<std::string, int>>& getProduse() const;

    // Setteri
    void setNumeClient(const std::string& nume);
    void setTotal(double t);
    void setProduse(const std::vector<std::pair<std::string, int>>& produseNou);
};

// Clasa SistemComenzi
class SistemComenzi {
private:
    std::vector<Comanda> comenzi; // Lista comenzilor
    double reducereClientFidel; // Reducere aplicabila pentru clientii fideli
    std::map<std::string, int> clientiFideli; // Map pentru clientii fideli si numarul lor de comenzi

public:
    // Constructori si destructor
    SistemComenzi(double reducereClientFidel = 0.0);
    ~SistemComenzi();

    // Functionalitati
    void adaugaComanda(const Comanda& comanda);
    void aplicaReducereClientFidel(const std::string& numeClient);
    void afiseazaToateComenzile(bool inEngleza = false, const std::vector<Produs>& produse = {}) const;
    double calculeazaVenituriTotale() const;

    // Getteri
    const std::vector<Comanda>& getComenzi() const;
    double getReducereClientFidel() const;
    const std::map<std::string, int>& getClientiFideli() const;

    // Setteri
    void setReducereClientFidel(double reducere);
    void setComenzi(const std::vector<Comanda>& comenziNou);
    void setClientiFideli(const std::map<std::string, int>& clientiNou);
};

#endif
