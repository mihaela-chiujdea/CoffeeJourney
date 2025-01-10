#ifndef EVENIMENTE_H
#define EVENIMENTE_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

// Clasa Eveniment
class Eveniment {
private:
    std::string nume;                // Numele evenimentului in romana
    std::string traducereEngleza;    // Traducerea numelui in engleza
    std::string data;                // Data evenimentului
    double costuri;                  // Costurile evenimentului
    double venituri;                 // Veniturile evenimentului

public:
    // Constructori
    Eveniment();
    Eveniment(const std::string& nume, const std::string& data, double costuri, double venituri);

    // Destructor
    ~Eveniment();

    // Getteri
    std::string getNume() const;
    std::string getTraducereEngleza() const;
    std::string getData() const;
    double getCosturi() const;
    double getVenituri() const;

    // Setteri
    void setNume(const std::string& nume);
    void setTraducereEngleza(const std::string& traducere);
    void setData(const std::string& data);
    void setCosturi(double costuri);
    void setVenituri(double venituri);

    // Calculare profit
    double calculeazaProfit() const;

    // Afisare detalii
    void afiseazaDetalii() const;
};

// Clasa GestionareEvenimente
class GestionareEvenimente {
private:
    std::vector<Eveniment> evenimente;  // Lista de evenimente

public:
    // Constructori si destructor
    GestionareEvenimente();
    ~GestionareEvenimente();

    // Gestionare evenimente
    void adaugaEveniment(const Eveniment& eveniment);
    void eliminaEveniment(const std::string& nume);
    void afiseazaToateEvenimentele() const;
    double calculeazaProfitTotal() const;

    // Getter pentru lista de evenimente
    const std::vector<Eveniment>& getEvenimente() const;

    // Setter pentru lista de evenimente (optional)
    void setEvenimente(const std::vector<Eveniment>& lista);
};

#endif
