// main.cpp - Functia principala pentru integrarea si testarea gestionarii angajatilor
#include "Angajati.h"
#include "Produse.h"
#include "Comenzi.h"
#include "Evenimente.h"
#include "Raportare.h"

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>

// Functie pentru salvarea angajatilor in CSV
void salveazaAngajatiDualCSV(const std::vector<std::unique_ptr<Angajat>>& angajati,
                             const std::string& fileNameRo, const std::string& fileNameEn) {
    // Salvare in romana
    std::ofstream fileRo(fileNameRo);
    if (!fileRo.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului " << fileNameRo << " pentru scriere!\n";
        return;
    }

    for (const auto& angajat : angajati) {
        fileRo << angajat->getNume() << "," << angajat->getFunctie() << ","
               << angajat->getOraStart() << "," << angajat->getOraEnd() << "\n";
    }
    fileRo.close();

    // Salvare in engleza
    std::ofstream fileEn(fileNameEn);
    if (!fileEn.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului " << fileNameEn << " pentru scriere!\n";
        return;
    }

    for (const auto& angajat : angajati) {
        fileEn << angajat->getNume() << "," << traduceFunctie(angajat->getFunctie(), true) << ","
               << angajat->getOraStart() << "," << angajat->getOraEnd() << "\n";
    }
    fileEn.close();

    std::cout << "Angajatii au fost salvati in " << fileNameRo << " si " << fileNameEn << "!\n";
}

// Functie pentru incarcarea angajatilor din CSV
void incarcaAngajatiDualCSV(std::vector<std::unique_ptr<Angajat>>& angajati, const std::string& fileNameRo, const std::string& fileNameEn) {
    // incarcare din fisierul in limba romana
    std::ifstream fileRo(fileNameRo);
    if (!fileRo.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului " << fileNameRo << " pentru citire!\n";
        return;
    }

    std::string linie;
    while (std::getline(fileRo, linie)) {
        std::istringstream ss(linie);
        std::string nume, functie, oraStart, oraEnd;

        std::getline(ss, nume, ',');
        std::getline(ss, functie, ',');
        std::getline(ss, oraStart, ',');
        std::getline(ss, oraEnd, ',');

        angajati.push_back(std::make_unique<Angajat>(nume, functie, oraStart, oraEnd));
    }
    fileRo.close();
    std::cout << "Angajatii au fost incarcati din " << fileNameRo << "!\n";

    // Incarcare din fisierul in limba engleza (pentru traduceri)
    std::ifstream fileEn(fileNameEn);
    if (!fileEn.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului " << fileNameEn << " pentru citire!\n";
        return;
    }

    int index = 0; // Index pentru alinierea angajatilor
    while (std::getline(fileEn, linie) && index < angajati.size()) {
        std::istringstream ss(linie);
        std::string nume, functie, oraStart, oraEnd;

        std::getline(ss, nume, ',');  // Ignoram numele, deja incarcat din fisierul romanesc
        std::getline(ss, functie, ',');
        std::getline(ss, oraStart, ',');
        std::getline(ss, oraEnd, ',');

        // Setam traducerea functiei pentru angajatul corespunzator
        angajati[index]->setFunctie(traduceFunctie(functie, false)); // False indica traducerea din engleza in romana
        ++index;
    }

    fileEn.close();
    std::cout << "Traducerile pentru angajati au fost incarcate din " << fileNameEn << "!\n";
}

void meniuAngajati(std::vector<std::unique_ptr<Angajat>>& angajati,
                   const std::string& fileNameRo, const std::string& fileNameEn) {
    int optiune = 0;
    do {
        std::cout << "\n--- Gestionare Angajati ---\n";
        std::cout << "1. Adauga angajat\n";
        std::cout << "2. Sterge angajat\n";
        std::cout << "3. Afiseaza toti angajatii\n";
        std::cout << "4. Modifica programul unui angajat\n";
        std::cout << "0. Iesire\n";
        std::cout << "Alege o optiune: ";
        std::cin >> optiune;

        switch (optiune) {
            case 1: {
                std::string nume, functie, oraStart, oraEnd;
                std::cout << "Introduceti numele angajatului: ";
                std::cin.ignore();
                std::getline(std::cin, nume);

                std::cout << "Introduceti functia (Barista/Manager/Ospatar): ";
                std::getline(std::cin, functie);

                std::cout << "Introduceti ora de inceput: ";
                std::cin >> oraStart;

                std::cout << "Introduceti ora de sfarsit: ";
                std::cin >> oraEnd;

                if (traduceriFunctii.find(functie) != traduceriFunctii.end()) {
                    angajati.push_back(std::make_unique<Angajat>(nume, functie, oraStart, oraEnd));
                } else {
                    std::cout << "Functie invalida!\n";
                }

                salveazaAngajatiDualCSV(angajati, fileNameRo, fileNameEn);
                break;
            }

            case 2: {
                std::string nume;
                std::cout << "Introduceti numele angajatului de sters: ";
                std::cin.ignore();
                std::getline(std::cin, nume);

                auto it = std::remove_if(angajati.begin(), angajati.end(),
                                         [&nume](const std::unique_ptr<Angajat>& angajat) {
                                             return angajat->getNume() == nume;
                                         });

                if (it != angajati.end()) {
                    angajati.erase(it, angajati.end());
                    std::cout << "Angajat sters cu succes!\n";
                } else {
                    std::cout << "Angajatul nu a fost gasit!\n";
                }

                salveazaAngajatiDualCSV(angajati, fileNameRo, fileNameEn);
                break;
            }

            case 3: {
                std::cout << "Lista angajatilor:\n";
                for (const auto& angajat : angajati) {
                    angajat->afisareDetalii();
                }
                break;
            }

            case 4: {
                std::string nume;
                std::cout << "Introduceti numele angajatului al carui program doriti sa il modificati: ";
                std::cin.ignore();
                std::getline(std::cin, nume);

                bool gasit = false;
                for (auto& angajat : angajati) {
                    if (angajat->getNume() == nume) {
                        std::string oraStart, oraEnd;
                        std::cout << "Introduceti noua ora de inceput: ";
                        std::cin >> oraStart;
                        std::cout << "Introduceti noua ora de sfarsit: ";
                        std::cin >> oraEnd;
                        angajat->setOraStart(oraStart);
                        angajat->setOraEnd(oraEnd);
                        std::cout << "Program modificat cu succes!\n";
                        gasit = true;
                        break;
                    }
                }

                if (!gasit) {
                    std::cout << "Angajatul nu a fost gasit!\n";
                }

                salveazaAngajatiDualCSV(angajati, fileNameRo, fileNameEn);
                break;
            }

            case 0:
                std::cout << "Iesire din meniul angajatilor.\n";
                break;

            default:
                std::cout << "Optiune invalida!\n";
                break;
        }
    } while (optiune != 0);
}
// Functie pentru salvarea produselor in CSV
void salveazaProduseDualCSV(const std::vector<Produs>& produse, const std::string& fileNameRo, const std::string& fileNameEn) {
    // Salvare in romana
    std::ofstream fileRo(fileNameRo);
    if (!fileRo.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului " << fileNameRo << " pentru scriere!\n";
        return;
    }

    for (const auto& produs : produse) {
        fileRo << produs.getNume() << "," << produs.getPret() << "," << produs.getCantitate() << "\n";
    }
    fileRo.close();

    // Salvare in engleza
    std::ofstream fileEn(fileNameEn);
    if (!fileEn.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului " << fileNameEn << " pentru scriere!\n";
        return;
    }

    for (const auto& produs : produse) {
        fileEn << produs.getTraducereEngleza() << "," << produs.getPret() << "," << produs.getCantitate() << "\n";
    }
    fileEn.close();

    std::cout << "Produsele au fost salvate in " << fileNameRo << " si " << fileNameEn << "!\n";
}

// Functie pentru incarcarea produselor din CSV
void incarcaProduseDualCSV(std::vector<Produs>& produse, const std::string& fileNameRo, const std::string& fileNameEn) {
    try {
        std::ifstream fileRo(fileNameRo);
        if (!fileRo.is_open()) throw std::runtime_error("Nu s-a putut deschide fisierul " + fileNameRo);

        std::ifstream fileEn(fileNameEn);
        if (!fileEn.is_open()) throw std::runtime_error("Nu s-a putut deschide fisierul " + fileNameEn);

        std::string linieRo, linieEn;

        // Golim vectorul de produse inainte de a incarca date noi
        produse.clear();

        while (std::getline(fileRo, linieRo) && std::getline(fileEn, linieEn)) {
            try {
                // Parsare fisier in romana
                std::istringstream ssRo(linieRo);
                std::string numeRo, pretStr, cantitateStr;
                std::getline(ssRo, numeRo, ',');
                std::getline(ssRo, pretStr, ',');
                std::getline(ssRo, cantitateStr);

                double pret = std::stod(pretStr);
                int cantitate = std::stoi(cantitateStr);

                // Parsare fisier in engleza
                std::istringstream ssEn(linieEn);
                std::string numeEn;
                std::getline(ssEn, numeEn, ',');

                // Creare obiect Produs
                Produs produs(numeRo, pret, cantitate);
                produs.setTraducereEngleza(numeEn);

                // Adaugare produs in vector
                produse.push_back(produs);

            } catch (const std::exception& e) {
                std::cerr << "Eroare la procesarea liniei: " << linieRo << " | " << linieEn << "\n";
                std::cerr << "Detalii eroare: " << e.what() << "\n";
            }
        }

        fileRo.close();
        fileEn.close();

        std::cout << "Produsele au fost incarcate din fisierele " << fileNameRo << " si " << fileNameEn << "!\n";

    } catch (const std::exception& e) {
        std::cerr << "Eroare la incarcarea produselor: " << e.what() << "\n";
    }
}

void meniuProduse(std::vector<Produs>& produse, const std::string& fileNameRo, const std::string& fileNameEn) {
    incarcaProduseDualCSV(produse, fileNameRo, fileNameEn);

    int optiune = 0;
    do {
        std::cout << "\n--- Gestionare Produse ---\n";
        std::cout << "1. Adauga produs nou\n";
        std::cout << "2. Adauga stoc la un produs existent\n";
        std::cout << "3. Sterge produs\n";
        std::cout << "4. Afiseaza toate produsele\n";
        std::cout << "0. Iesire\n";
        std::cout << "Alege o optiune: ";
        std::cin >> optiune;

        switch (optiune) {
            case 1: {
                std::string numeRo, numeEn;
                double pret;
                int cantitate;

                std::cout << "Introduceti numele produsului (romana): ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, numeRo);

                std::cout << "Introduceti numele produsului (engleza): ";
                std::getline(std::cin, numeEn);

                std::cout << "Introduceti pretul produsului: ";
                std::cin >> pret;

                std::cout << "Introduceti cantitatea produsului: ";
                std::cin >> cantitate;

                Produs produs(numeRo, pret, cantitate);
                produs.setTraducereEngleza(numeEn);

                produse.push_back(produs);
                salveazaProduseDualCSV(produse, fileNameRo, fileNameEn);
                break;
            }

            case 2: {
                std::string nume;
                int cantitateAdaugata;

                std::cout << "Introduceti numele produsului (romana): ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, nume);

                auto it = std::find_if(produse.begin(), produse.end(), [&nume](const Produs& produs) {
                    return produs.getNume() == nume;
                });

                if (it != produse.end()) {
                    std::cout << "Introduceti cantitatea de adaugat: ";
                    std::cin >> cantitateAdaugata;
                    it->setCantitate(it->getCantitate() + cantitateAdaugata);
                    std::cout << "Stoc actualizat cu succes!\n";
                    salveazaProduseDualCSV(produse, fileNameRo, fileNameEn);
                } else {
                    std::cout << "Produsul nu a fost gasit!\n";
                }

                break;
            }

            case 3: {
                std::string nume;
                std::cout << "Introduceti numele produsului de sters (romana): ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, nume);

                auto it = std::remove_if(produse.begin(), produse.end(), [&nume](const Produs& produs) {
                    return produs.getNume() == nume;
                });

                if (it != produse.end()) {
                    produse.erase(it, produse.end());
                    std::cout << "Produs sters cu succes!\n";
                    salveazaProduseDualCSV(produse, fileNameRo, fileNameEn);
                } else {
                    std::cout << "Produsul nu a fost gasit!\n";
                }

                break;
            }

            case 4: {
                std::cout << "Lista produselor:\n";
                for (const auto& produs : produse) {
                    std::cout << "- " << produs.getNume() << " (EN: " << produs.getTraducereEngleza()
                              << ") | Pret: " << produs.getPret() << " lei | Cantitate: " << produs.getCantitate() << "\n";
                }
                break;
            }

            case 0:
                std::cout << "Iesire din meniul produselor.\n";
                break;

            default:
                std::cout << "Optiune invalida!\n";
                break;
        }
    } while (optiune != 0);
}

// Functie pentru salvarea comenzilor in CSV
void salveazaComenziDualCSV(const SistemComenzi& sistemComenzi,
                            const std::vector<Produs>& produse,
                            const std::string& fileNameRo,
                            const std::string& fileNameEn) {
    try {
        // Salvare in romana
        std::ofstream fileRo(fileNameRo);
        if (!fileRo.is_open()) throw std::runtime_error("Eroare la deschiderea fisierului " + fileNameRo + " pentru scriere!");

        // Salvare in engleza
        std::ofstream fileEn(fileNameEn);
        if (!fileEn.is_open()) throw std::runtime_error("Eroare la deschiderea fisierului " + fileNameEn + " pentru scriere!");

        for (const auto& comanda : sistemComenzi.getComenzi()) {
            // Salvare comenzi in romana
            fileRo << comanda.getNumeClient() << "," << comanda.getTotal() << "\n";

            // Salvare comenzi in engleza
            fileEn << comanda.getNumeClient() << "," << comanda.getTotal() << "\n";

            for (const auto& produs : comanda.getProduse()) {
                auto it = std::find_if(produse.begin(), produse.end(), [&produs](const Produs& p) {
                    return p.getNume() == produs.first;
                });

                if (it != produse.end()) {
                    // Scriem in romana
                    fileRo << produs.first << "," << produs.second << "\n";
                    // Scriem in engleza
                    fileEn << it->getTraducereEngleza() << "," << produs.second << "\n";
                } else {
                    std::cerr << "Produsul " << produs.first << " nu a fost gasit in lista de produse.\n";
                }
            }
            fileRo << "\n";
            fileEn << "\n";
        }

        fileRo.close();
        fileEn.close();

        std::cout << "Comenzile au fost salvate in " << fileNameRo << " si " << fileNameEn << "!\n";

    } catch (const std::exception& e) {
        std::cerr << "Eroare la salvarea comenzilor: " << e.what() << "\n";
    }
}

// Functie pentru incarcarea comenzilor din CSV
void incarcaComenziDualCSV(SistemComenzi& sistemComenzi, const std::string& fileNameRo, const std::string& fileNameEn) {
    // Citirea din fisierul in romana
    std::ifstream fileRo(fileNameRo);
    if (!fileRo.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului " << fileNameRo << " pentru citire!\n";
        return;
    }

    std::string linie;
    while (std::getline(fileRo, linie)) {
        std::istringstream ss(linie);
        std::string numeClient, numeProdus;
        double total;
        int cantitate;

        // Citire nume client si total comanda
        std::getline(ss, numeClient, ',');
        ss >> total;
        ss.ignore();

        Comanda comanda(numeClient);
        comanda.setTotal(total); // Setam totalul din CSV

        // Citire produse asociate
        while (std::getline(fileRo, linie) && !linie.empty()) {
            std::istringstream produsStream(linie);
            std::getline(produsStream, numeProdus, ',');
            produsStream >> cantitate;

            comanda.adaugaProdus(numeProdus, cantitate, 0.0); // Pretul unitar nu este relevant aici
        }

        sistemComenzi.adaugaComanda(comanda);
    }
    fileRo.close();
}

void meniuComenzi(std::vector<Produs>& produse, SistemComenzi& sistemComenzi,
                   const std::string& comenziFileNameRo, const std::string& comenziFileNameEn,
                   const std::string& produseFileNameRo, const std::string& produseFileNameEn) {
    int optiune = 0;
    do {
        std::cout << "\n--- Gestionare Comenzi ---\n";
        std::cout << "1. Adauga comanda\n";
        std::cout << "2. Afiseaza toate comenzile\n";
        std::cout << "3. Calculeaza veniturile totale\n";
        std::cout << "0. Iesire\n";
        std::cout << "Alege o optiune: ";
        std::cin >> optiune;

        switch (optiune) {
            case 1: {
                std::string numeClient;
                std::cout << "Introduceti numele clientului: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, numeClient);

                char esteFidel;
                std::cout << "Client fidel? (y/n): ";
                std::cin >> esteFidel;
                bool clientFidel = (esteFidel == 'y' || esteFidel == 'Y');

                Comanda comanda(numeClient);
                while (true) {
                    std::string numeProdus;
                    int cantitate;

                    std::cout << "Introduceti numele produsului (sau 'stop' pentru a termina): ";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, numeProdus);
                    if (numeProdus == "stop") break;

                    // Gasirea produsului indiferent de limba
                    auto it = std::find_if(produse.begin(), produse.end(), [&numeProdus](const Produs& produs) {
                        return produs.getNume() == numeProdus || produs.getTraducereEngleza() == numeProdus;
                    });

                    if (it != produse.end()) {
                        std::cout << "Introduceti cantitatea: ";
                        std::cin >> cantitate;

                        if (cantitate <= it->getCantitate()) {
                            comanda.adaugaProdus(it->getNume(), cantitate, it->getPret());
                            it->setCantitate(it->getCantitate() - cantitate);
                        } else {
                            std::cout << "Stoc insuficient pentru " << numeProdus << "!\n";
                        }
                    } else {
                        std::cout << "Produsul nu a fost gasit!\n";
                    }
                }

                if (clientFidel) {
                    double totalInitial = comanda.calculeazaTotal();
                    comanda.aplicaReducere(0.10); // 10% reducere
                    std::cout << "Reducere aplicata: " << totalInitial - comanda.calculeazaTotal() << " lei\n";
                }
                std::cout << "Nume client: " << numeClient << " | " << "Total: " << comanda.calculeazaTotal() << " lei\n";
                sistemComenzi.adaugaComanda(comanda);

                // Salvare stocuri in ambele limbi
                salveazaProduseDualCSV(produse, produseFileNameRo, produseFileNameEn);

                // Salvare comenzi in ambele limbi
                salveazaComenziDualCSV(sistemComenzi, produse, comenziFileNameRo, comenziFileNameEn);
                break;
            }

            case 2: {
                char limba;
                std::cout << "Afisati comenzile in limba romana (r) sau engleza (e)? ";
                std::cin >> limba;

                bool inEngleza = (limba == 'e' || limba == 'E');
                sistemComenzi.afiseazaToateComenzile(inEngleza);
                break;
            }

            case 3: {
                double venituri = sistemComenzi.calculeazaVenituriTotale();
                std::cout << "Venituri totale: " << venituri << " lei\n";
                break;
            }

            case 0:
                std::cout << "Iesire din meniul comenzilor.\n";
                break;

            default:
                std::cout << "Optiune invalida!\n";
                break;
        }
    } while (optiune != 0);
}

// Functie pentru salvarea evenimentelor in CSV
void salveazaEvenimenteDualCSV(const GestionareEvenimente& sistemEvenimente,
                               const std::string& fileNameRo, const std::string& fileNameEn) {
    // Salvare in romana
    std::ofstream fileRo(fileNameRo);
    if (!fileRo.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului " << fileNameRo << " pentru scriere!\n";
    } else {
        for (const auto& eveniment : sistemEvenimente.getEvenimente()) {
            fileRo << eveniment.getNume() << "," << eveniment.getData()
                   << "," << eveniment.getCosturi() << "," << eveniment.getVenituri() << "\n";
        }
        fileRo.close();
        std::cout << "Evenimentele au fost salvate in " << fileNameRo << "!\n";
    }

    // Salvare in engleza
    std::ofstream fileEn(fileNameEn);
    if (!fileEn.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului " << fileNameEn << " pentru scriere!\n";
    } else {
        for (const auto& eveniment : sistemEvenimente.getEvenimente()) {
            fileEn << eveniment.getTraducereEngleza() << "," << eveniment.getData()
                   << "," << eveniment.getCosturi() << "," << eveniment.getVenituri() << "\n";
        }
        fileEn.close();
        std::cout << "Evenimentele au fost salvate in " << fileNameEn << "!\n";
    }
}

void incarcaEvenimenteDualCSV(GestionareEvenimente& sistemEvenimente,
                              const std::string& fileNameRo, const std::string& fileNameEn) {
    std::ifstream fileRo(fileNameRo);
    std::ifstream fileEn(fileNameEn);

    if (!fileRo.is_open() || !fileEn.is_open()) {
        if (!fileRo.is_open()) {
            std::cerr << "Eroare la deschiderea fisierului " << fileNameRo << " pentru citire!\n";
        }
        if (!fileEn.is_open()) {
            std::cerr << "Eroare la deschiderea fisierului " << fileNameEn << " pentru citire!\n";
        }
        return;
    }

    std::string linieRo, linieEn;
    while (std::getline(fileRo, linieRo) && std::getline(fileEn, linieEn)) {
        std::istringstream ssRo(linieRo);
        std::istringstream ssEn(linieEn);

        std::string numeRo, data, numeEn;
        double costuri, venituri;

        // Citire din fisierul in romana
        std::getline(ssRo, numeRo, ',');
        std::getline(ssRo, data, ',');
        ssRo >> costuri;
        ssRo.ignore();
        ssRo >> venituri;

        // Citire din fisierul in engleza
        std::getline(ssEn, numeEn, ',');

        // Creare si adaugare obiect Eveniment cu traducere
        Eveniment eveniment(numeRo, data, costuri, venituri);
        eveniment.setTraducereEngleza(numeEn);
        sistemEvenimente.adaugaEveniment(eveniment);
    }

    fileRo.close();
    fileEn.close();

    std::cout << "Evenimentele au fost incarcate din " << fileNameRo
              << " si " << fileNameEn << "!\n";
}

void meniuEvenimente(GestionareEvenimente& sistemEvenimente,
                     const std::string& fileNameRo, const std::string& fileNameEn) {
    int optiune = 0;
    do {
        std::cout << "\n--- Gestionare Evenimente ---\n";
        std::cout << "1. Adauga eveniment\n";
        std::cout << "2. Sterge eveniment\n";
        std::cout << "3. Afiseaza toate evenimentele\n";
        std::cout << "4. Calculeaza profitul total\n";
        std::cout << "0. Iesire\n";
        std::cout << "Alege o optiune: ";
        std::cin >> optiune;

        switch (optiune) {
            case 1: {
                std::string nume, traducereEn, data;
                double costuri, venituri;

                std::cout << "Introduceti numele evenimentului (romana): ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, nume);

                std::cout << "Introduceti traducerea numelui in engleza: ";
                std::getline(std::cin, traducereEn);

                std::cout << "Introduceti data evenimentului: ";
                std::getline(std::cin, data);

                std::cout << "Introduceti costurile evenimentului: ";
                std::cin >> costuri;

                std::cout << "Introduceti veniturile estimate: ";
                std::cin >> venituri;

                Eveniment eveniment(nume, data, costuri, venituri);
                eveniment.setTraducereEngleza(traducereEn);
                sistemEvenimente.adaugaEveniment(eveniment);

                salveazaEvenimenteDualCSV(sistemEvenimente, fileNameRo, fileNameEn);
                std::cout << "Eveniment adaugat cu succes!\n";
                break;
            }

            case 2: {
                std::string nume;
                std::cout << "Introduceti numele evenimentului de sters: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, nume);

                sistemEvenimente.eliminaEveniment(nume);
                salveazaEvenimenteDualCSV(sistemEvenimente, fileNameRo, fileNameEn);
                std::cout << "Eveniment sters cu succes!\n";
                break;
            }

            case 3: {
                std::cout << "Lista evenimentelor:\n";
                sistemEvenimente.afiseazaToateEvenimentele();
                break;
            }

            case 4: {
                double profitTotal = sistemEvenimente.calculeazaProfitTotal();
                std::cout << "Profitul total din evenimente: " << profitTotal << " lei\n";
                break;
            }

            case 0:
                std::cout << "Iesire din meniul evenimentelor.\n";
                break;

            default:
                std::cout << "Optiune invalida!\n";
                break;
        }
    } while (optiune != 0);
}

void meniuRaportare(const SistemComenzi& sistemComenzi,
                    const GestionareEvenimente& sistemEvenimente,
                    const std::vector<std::unique_ptr<Angajat>>& angajati,
                    const std::vector<Produs>& produse,
                    const std::string& salariiFileName,
                    const std::string& raportFileNameRo,
                    const std::string& raportFileNameEn) {
    std::map<std::string, double> salarii = Raportare::incarcaSalariiDinCSV(salariiFileName);

    // Generam raportul financiar in ambele limbi
    Raportare::genereazaRaportDual(sistemComenzi, sistemEvenimente, angajati, produse, salarii, raportFileNameRo, raportFileNameEn);
}


void meniuPrincipal(const std::string& oras,
                    std::vector<std::unique_ptr<Angajat>>& angajati,
                    std::vector<Produs>& produse,
                    SistemComenzi& sistemComenzi,
                    GestionareEvenimente& sistemEvenimente,
                    const std::string& angajatiFileNameRo, const std::string& angajatiFileNameEn,
                    const std::string& produseFileNameRo, const std::string& produseFileNameEn,
                    const std::string& comenziFileNameRo, const std::string& comenziFileNameEn,
                    const std::string& evenimenteFileNameRo, const std::string& evenimenteFileNameEn,
                    const std::string& salariiFileName,
                    const std::string& raportFileNameRo, const std::string& raportFileNameEn) {
    int optiune = 0;
    do {
        std::cout << "\n--- Meniu Principal pentru " << oras << " ---\n";
        std::cout << "1. Gestionare Angajati\n";
        std::cout << "2. Gestionare Produse\n";
        std::cout << "3. Gestionare Comenzi\n";
        std::cout << "4. Gestionare Evenimente\n";
        std::cout << "5. Generare Raport Financiar\n";
        std::cout << "0. Iesire\n";
        std::cout << "Alege o optiune: ";
        std::cin >> optiune;

        switch (optiune) {
            case 1:
                meniuAngajati(angajati, angajatiFileNameRo, angajatiFileNameEn);
                break;

            case 2:
                meniuProduse(produse, produseFileNameRo, produseFileNameEn);
                break;

            case 3:
                meniuComenzi(produse, sistemComenzi, comenziFileNameRo, comenziFileNameEn, produseFileNameRo, produseFileNameEn);
                break;

            case 4:
                meniuEvenimente(sistemEvenimente, evenimenteFileNameRo,evenimenteFileNameEn);
                break;

            case 5:
                meniuRaportare(sistemComenzi, sistemEvenimente, angajati, produse, salariiFileName, raportFileNameRo, raportFileNameEn);
                break;

            case 0:
                std::cout << "Iesire din program pentru " << oras << ".\n";
                break;

            default:
                std::cout << "Optiune invalida!\n";
                break;
        }
    } while (optiune != 0);
}


int main() {
    std::string oras;
    do {
        std::cout << "Selecteaza orasul:\n";
        std::cout << "1. Bucuresti\n";
        std::cout << "2. Cluj-Napoca\n";
        std::cout << "3. Constanta\n";
        std::cout << "4. Iasi\n";
        std::cout << "5. Brasov\n";
        std::cout << "0. Iesire\n";
        std::cout << "Alege o optiune: ";
        int optiune;
        std::cin >> optiune;

        switch (optiune) {
            case 1: oras = "bucuresti"; break;
            case 2: oras = "cluj"; break;
            case 3: oras = "constanta"; break;
            case 4: oras = "iasi"; break;
            case 5: oras = "brasov"; break;
            case 0:
                std::cout << "Iesire din program.\n";
                return 0;
            default:
                std::cout << "Optiune invalida!\n";
                continue;
        }

        // Fisiere pentru orasul selectat
        const std::string angajatiFileNameRo = "angajati_" + oras + "_ro.csv";
        const std::string angajatiFileNameEn = "angajati_" + oras + "_en.csv";
        const std::string produseFileNameRo = "produse_" + oras + "_ro.csv";
        const std::string produseFileNameEn = "produse_" + oras + "_en.csv";
        const std::string comenziFileNameRo = "comenzi_" + oras + "_ro.csv";
        const std::string comenziFileNameEn = "comenzi_" + oras + "_en.csv";
        const std::string evenimenteFileNameRo = "evenimente_" + oras + "_ro.csv";
        const std::string evenimenteFileNameEn = "evenimente_" + oras + "_en.csv";
        const std::string salariiFileName = "salarii.csv";
        const std::string raportFileNameRo = "raport_financiar_" + oras + "_ro.csv";
        const std::string raportFileNameEn = "raport_financiar_" + oras + "_en.csv";

        // Gestionare date pentru oras
        std::vector<std::unique_ptr<Angajat>> angajati;
        std::vector<Produs> produse;
        SistemComenzi sistemComenzi(10.0); // Reducere 10% pentru clienti fideli
        GestionareEvenimente sistemEvenimente;

        // incarcare date din CSV
        try {
            incarcaAngajatiDualCSV(angajati, angajatiFileNameRo, angajatiFileNameEn);
            incarcaProduseDualCSV(produse, produseFileNameRo, produseFileNameEn);
            incarcaEvenimenteDualCSV(sistemEvenimente, evenimenteFileNameRo, evenimenteFileNameEn);
            incarcaComenziDualCSV(sistemComenzi, comenziFileNameRo, comenziFileNameEn);
        } catch (const std::exception& e) {
            std::cerr << "Eroare la incarcarea datelor: " << e.what() << "\n";
        }
        // Meniu principal pentru orasul selectat
        meniuPrincipal(oras, angajati, produse, sistemComenzi, sistemEvenimente,
                       angajatiFileNameRo,angajatiFileNameEn,
                       produseFileNameRo, produseFileNameEn,
                       comenziFileNameRo, comenziFileNameEn,
                       evenimenteFileNameRo, evenimenteFileNameEn,
                       salariiFileName,
                       raportFileNameRo, raportFileNameEn);
    } while (true);

    return 0;
}
