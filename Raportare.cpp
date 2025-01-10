#include "Raportare.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm> // Pentru std::find_if

// Functie pentru incarcarea salariilor din CSV
std::map<std::string, double> Raportare::incarcaSalariiDinCSV(const std::string& fileName) {
    std::map<std::string, double> salarii;
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului " << fileName << " pentru citire!\n";
        return salarii;
    }

    std::string linie;
    while (std::getline(file, linie)) {
        if (linie.empty()) continue;

        std::istringstream ss(linie);
        std::string rol;
        double salariu;

        std::getline(ss, rol, ',');
        ss >> salariu;

        salarii[rol] = salariu;
    }

    file.close();
    return salarii;
}

// Functie pentru calcularea costurilor produselor utilizate in comenzi
double Raportare::calculeazaCosturiProduse(const SistemComenzi& sistemComenzi, const std::vector<Produs>& produse) {
    double costTotal = 0.0;

    for (const auto& comanda : sistemComenzi.getComenzi()) {
        for (const auto& produsComandat : comanda.getProduse()) {
            auto it = std::find_if(produse.begin(), produse.end(), [&produsComandat](const Produs& produs) {
                return produs.getNume() == produsComandat.first;
            });

            if (it != produse.end()) {
                costTotal += it->getPret() * produsComandat.second; // Cantitate * Pret unitar
            }
        }
    }

    return costTotal;
}

// Functie pentru calcularea salariilor angajatilor
double Raportare::calculeazaSalarii(const std::vector<std::unique_ptr<Angajat>>& angajati, const std::map<std::string, double>& salarii) {
    double salariiTotale = 0.0;

    for (const auto& angajat : angajati) {
        std::string functie = angajat->getFunctie(); // Obtine functia direct din atribut
        auto it = salarii.find(functie);

        if (it != salarii.end()) {
            double salariuLunar = it->second;
            double salariuZilnic = salariuLunar / 22; // Presupunem 22 zile lucratoare pe luna
            salariiTotale += salariuZilnic;

            std::cout << "Angajat: " << angajat->getNume()
                      << ", Functie: " << functie
                      << ", Salariu zilnic: " << salariuZilnic << " lei\n";
        } else {
            std::cerr << "Functia \"" << functie << "\" nu a fost gasita in salarii.csv!\n";
        }
    }

    return salariiTotale;
}

// Functie pentru generarea raportului financiar
void Raportare::genereazaRaport(const SistemComenzi& sistemComenzi,
                                const GestionareEvenimente& sistemEvenimente,
                                const std::vector<std::unique_ptr<Angajat>>& angajati,
                                const std::vector<Produs>& produse,
                                const std::map<std::string, double>& salarii,
                                const std::string& fileName) {
    std::ofstream raport(fileName);

    if (!raport.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului " << fileName << "\n";
        return;
    }

    raport << "Tip,Sursa,Costuri,Venituri,Profit\n";

    // Venituri din comenzi
    double venituriComenzi = sistemComenzi.calculeazaVenituriTotale();
    raport << "Comenzi,Total,," << venituriComenzi << "," << venituriComenzi << "\n";

    // Evenimente
    double profitTotalEvenimente = 0.0;
    for (const auto& eveniment : sistemEvenimente.getEvenimente()) {
        double profit = eveniment.calculeazaProfit();
        raport << "Evenimente," << eveniment.getNume() << "," << eveniment.getCosturi()
               << "," << eveniment.getVenituri() << "," << profit << "\n";
        profitTotalEvenimente += profit;
    }

    // Costuri produse
    double costuriProduse = calculeazaCosturiProduse(sistemComenzi, produse);
    raport << "Costuri Produse,Total," << costuriProduse << ",," << "\n";

    // Salarii angajati
    double salariiAngajati = calculeazaSalarii(angajati, salarii);
    raport << "Salarii Angajati,Total," << salariiAngajati << ",," << "\n";

    // Rezumat
    double totalVenituri = venituriComenzi + profitTotalEvenimente;
    double totalCosturi = costuriProduse + salariiAngajati;
    double profitNet = totalVenituri - totalCosturi;

    raport << "Rezumat,Total," << totalCosturi << "," << totalVenituri << "," << profitNet << "\n";

    raport.close();
    std::cout << "Raport generat cu succes in " << fileName << "\n";
}
void Raportare::genereazaRaportDual(const SistemComenzi& sistemComenzi,
                                    const GestionareEvenimente& sistemEvenimente,
                                    const std::vector<std::unique_ptr<Angajat>>& angajati,
                                    const std::vector<Produs>& produse,
                                    const std::map<std::string, double>& salarii,
                                    const std::string& fileNameRo,
                                    const std::string& fileNameEn) {
    // Generare raport in limba romana
    genereazaRaport(sistemComenzi, sistemEvenimente, angajati, produse, salarii, fileNameRo);

    // Generare raport in limba engleza
    std::ofstream raport(fileNameEn);

    if (!raport.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului " << fileNameEn << " pentru scriere!\n";
        return;
    }

    raport << "Type,Source,Costs,Revenue,Profit\n";

    // Venituri din comenzi
    double venituriComenzi = sistemComenzi.calculeazaVenituriTotale();
    raport << "Orders,Total,," << venituriComenzi << "," << venituriComenzi << "\n";

    // Evenimente
    double profitTotalEvenimente = 0.0;
    for (const auto& eveniment : sistemEvenimente.getEvenimente()) {
        double profit = eveniment.calculeazaProfit();
        raport << "Events," << eveniment.getNume() << "," << eveniment.getCosturi()
               << "," << eveniment.getVenituri() << "," << profit << "\n";
        profitTotalEvenimente += profit;
    }

    // Costuri produse
    double costuriProduse = calculeazaCosturiProduse(sistemComenzi, produse);
    raport << "Product Costs,Total," << costuriProduse << ",," << "\n";

    // Salarii angajati
    double salariiAngajati = calculeazaSalarii(angajati, salarii);
    raport << "Employee Salaries,Total," << salariiAngajati << ",," << "\n";

    // Rezumat
    double totalVenituri = venituriComenzi + profitTotalEvenimente;
    double totalCosturi = costuriProduse + salariiAngajati;
    double profitNet = totalVenituri - totalCosturi;

    raport << "Summary,Total," << totalCosturi << "," << totalVenituri << "," << profitNet << "\n";

    raport.close();
    std::cout << "Raport generat cu succes in " << fileNameEn << "\n";
}


