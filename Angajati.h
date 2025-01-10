#ifndef ANGAJATI_H
#define ANGAJATI_H

#include <string>
#include <map>
#include <iostream>

// Dictionar pentru traduceri
extern const std::map<std::string, std::string> traduceriFunctii;

// Functie pentru traducerea functiilor
std::string traduceFunctie(const std::string& functie, bool inEngleza);

// Clasa Angajat
class Angajat {
protected:
    std::string nume;
    std::string functie;
    std::string oraStart;
    std::string oraEnd;

public:
    // Constructori si destructor
    Angajat();
    Angajat(const std::string& nume, const std::string& functie, const std::string& oraStart, const std::string& oraEnd);
    virtual ~Angajat();

    // Getteri
    std::string getNume() const;
    std::string getFunctie() const;
    std::string getOraStart() const;
    std::string getOraEnd() const;

    // Setteri
    void setNume(const std::string& n);
    void setFunctie(const std::string& f);
    void setOraStart(const std::string& os);
    void setOraEnd(const std::string& oe);

    // Metoda virtuala pentru afisare detalii
    virtual void afisareDetalii() const;
};

#endif
