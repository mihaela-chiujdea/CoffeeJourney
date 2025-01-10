#ifndef RAPORTARE_H
#define RAPORTARE_H

#include "Comenzi.h"
#include "Evenimente.h"
#include "Angajati.h"
#include "Produse.h"

#include <string>
#include <map>
#include <vector>
#include <memory>

class Raportare {
private:
    std::string fileNameRo; // Numele fișierului pentru raportul în română
    std::string fileNameEn; // Numele fișierului pentru raportul în engleză

public:
    // Constructori
    Raportare();
    Raportare(const std::string& fileNameRo, const std::string& fileNameEn);

    // Destructor
    ~Raportare();

    // Getteri
    std::string getFileNameRo() const;
    std::string getFileNameEn() const;

    // Setteri
    void setFileNameRo(const std::string& fileNameRo);
    void setFileNameEn(const std::string& fileNameEn);

    // Funcții statice pentru generare și încărcare rapoarte
    static void genereazaRaport(const SistemComenzi& sistemComenzi,
                                const GestionareEvenimente& sistemEvenimente,
                                const std::vector<std::unique_ptr<Angajat>>& angajati,
                                const std::vector<Produs>& produse,
                                const std::map<std::string, double>& salarii,
                                const std::string& fileName);

    static std::map<std::string, double> incarcaSalariiDinCSV(const std::string& fileName);

    static void genereazaRaportDual(const SistemComenzi& sistemComenzi,
                                    const GestionareEvenimente& sistemEvenimente,
                                    const std::vector<std::unique_ptr<Angajat>>& angajati,
                                    const std::vector<Produs>& produse,
                                    const std::map<std::string, double>& salarii,
                                    const std::string& fileNameRo,
                                    const std::string& fileNameEn);

private:
    // Funcții statice pentru calcularea costurilor
    static double calculeazaCosturiProduse(const SistemComenzi& sistemComenzi, const std::vector<Produs>& produse);
    static double calculeazaSalarii(const std::vector<std::unique_ptr<Angajat>>& angajati, const std::map<std::string, double>& salarii);
};

#endif
