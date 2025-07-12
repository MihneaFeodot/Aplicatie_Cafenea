#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <limits>
#include <ctime>

using namespace std;

// Funcție pentru obținerea datei curente
string obtineDataCurenta() {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    int an = 1900 + ltm->tm_year;
    int luna = 1 + ltm->tm_mon;
    int zi = ltm->tm_mday;

    char buffer[11];
    sprintf(buffer, "%04d-%02d-%02d", an, luna, zi); // Format: YYYY-MM-DD
    return string(buffer);
}

//inceput abstract factory pentru angajati
class Angajat{
    protected: 
        string nume, functie;
        string ora_inceput, ora_sfarsit;

    public:
        virtual ~Angajat() = default;
        virtual void setNume(string nume) = 0;
        virtual void setFunctie(string functie) = 0;
        virtual void setOra_inceput(string ora_inceput) = 0;
        virtual void setOra_sfarsit(string ora_sfarsit) = 0; 

        string getNume(){
            return nume;
        }

        string getFunctie(){
            return functie;
        }               

        string getOra_inceput(){
            return ora_inceput;
        }

        string getOra_sfarsit(){
            return ora_sfarsit;
        }
};

class Barista : public Angajat{
    public:
        Barista(){
            cout << "Barista!!" << endl;
        }
        void setNume(string nume) override{
            this->nume = nume;
        }

        void setFunctie(string functie) override{
            this->functie = functie;
        }

        void setOra_inceput(string ora_inceput) override{
            this->ora_inceput = ora_inceput;
        }

        void setOra_sfarsit(string ora_sfarsit) override{
            this->ora_sfarsit = ora_sfarsit;
        }
};

class Manager : public Angajat{
    public:
        Manager(){
            cout << "Manager!!" << endl;
        }
        void setNume(string nume) override{
            this->nume = nume;
        }

        void setFunctie(string functie) override{    
            this->functie = functie;
        }

        void setOra_inceput(string ora_inceput) override{    
            this->ora_inceput = ora_inceput;
        }

        void setOra_sfarsit(string ora_sfarsit) override{    
            this->ora_sfarsit = ora_sfarsit;
        }
};

class Ospatar : public Angajat{
    public:
        Ospatar(){
            cout << "Ospatar!!" << endl;
        }
        void setNume(string nume) override{
            this->nume = nume;
        }

        void setFunctie(string functie) override{    
            this->functie = functie;
        }

        void setOra_inceput(string ora_inceput) override{    
            this->ora_inceput = ora_inceput;
        }

        void setOra_sfarsit(string ora_sfarsit) override{    
            this->ora_sfarsit = ora_sfarsit;
        }
};

class AngajatFactory{
    public:
        virtual Angajat* createBarista() = 0;
        virtual Angajat* createManager() = 0;
        virtual Angajat* createOspatar() = 0;
        virtual ~AngajatFactory() = default;
};

class BaristaFactory : public AngajatFactory{
    public:
        Angajat* createBarista() override{
            cout << "S-a creat un nou Barista" << endl;
            return new Barista();
        }

        Angajat* createManager() override{
            return nullptr;
        }

        Angajat* createOspatar() override{
            return nullptr;
        }
};

class ManagerFactory : public AngajatFactory{
    public:
        Angajat* createBarista() override{
            return nullptr;
        }

        Angajat* createManager() override{
            cout << "S-a creat un nou Manager" << endl;
            return new Manager();
        }

        Angajat* createOspatar() override{    
            return nullptr;
        }
};

class OspatarFactory : public AngajatFactory{
    public:
        Angajat* createBarista() override{
            return nullptr;
        }

        Angajat* createManager() override{
            return nullptr;
        }

        Angajat* createOspatar() override{
            cout << "S-a creat un nou Ospatar" << endl;
            return new Ospatar();
        }
};
//clasa de manipulat date din csv pentru functiile de angajati
class CSVWriter{
    public:
        void WriteToCSV(string filename, Angajat* angajat){
            ofstream file;
            file.open(filename, ios::app); // Deschide fișierul în modul append
            if (file.is_open()) {
                file << angajat->getNume() << ","
                    << angajat->getFunctie() << ","
                    << angajat->getOra_inceput() << ","
                    << angajat->getOra_sfarsit() << "\n";
                file.close();
            } else {
                cerr << "Nu s-a putut deschide fișierul " << filename << endl;
            }
        }

        void EraseFromCSV(string filename, string numeAngajat) {
            ifstream inputFile(filename);
            ofstream outputFile("temp.csv");
            string line;

            if (!inputFile.is_open() || !outputFile.is_open()) {
                cerr << "Nu s-a putut deschide fișierul " << filename << " sau fișierul temporar." << endl;
                return;
            }

            while (getline(inputFile, line)) {
                stringstream ss(line);
                string value;
                vector<string> row;

                while (getline(ss, value, ',')) {
                    row.push_back(value);
                }

                // Verifică dacă numele din primul câmp este diferit de cel specificat
                if (row.size() > 0 && row[0] != numeAngajat) {
                    outputFile << line << "\n";
                }
            }

            inputFile.close();
            outputFile.close();

            remove(filename.c_str());
            rename("temp.csv", filename.c_str());
        }

};
//singleton pentru a folosi o singura instanta de ActiuniAngajati, pentru toate functiile legate de angajati
class ActiuniAngajati{
    private:
        static ActiuniAngajati* instance;

        ActiuniAngajati(){}
    public:

        static ActiuniAngajati* getInstance() {
            if(instance == nullptr) {
                instance = new ActiuniAngajati();
            }
            return instance;
        }

        void adaugareAngajat(string nume, string functie, string ora_inceput, string ora_sfarsit){
            CSVWriter writer;
            string filename = "angajati.csv";
            if(functie == "barista")
            {
                AngajatFactory* angajatFactory = new BaristaFactory();
                Angajat* barista = angajatFactory->createBarista();
                barista->setNume(nume);
                barista->setFunctie(functie);
                barista->setOra_inceput(ora_inceput);
                barista->setOra_sfarsit(ora_sfarsit);

                writer.WriteToCSV(filename, barista);
                cout << barista->getNume() << endl;
                cout << barista->getFunctie() << endl;
                cout << barista->getOra_inceput() << endl;
                cout << barista->getOra_sfarsit() << endl; 
                delete angajatFactory;
            }

            else if(functie == "manager")
            {
                AngajatFactory* angajatFactory = new ManagerFactory();
                Angajat* manager = angajatFactory->createManager();
                manager->setNume(nume);
                manager->setFunctie(functie);
                manager->setOra_inceput(ora_inceput);
                manager->setOra_sfarsit(ora_sfarsit);
                writer.WriteToCSV(filename, manager);
                cout << manager->getNume() << endl;
                cout << manager->getFunctie() << endl;
                cout << manager->getOra_inceput() << endl;
                cout << manager->getOra_sfarsit() << endl;
                delete angajatFactory;
            }

            else if(functie == "ospatar")
            {
                AngajatFactory* angajatFactory = new OspatarFactory();
                Angajat* ospatar = angajatFactory->createOspatar();
                ospatar->setNume(nume);
                ospatar->setFunctie(functie);
                ospatar->setOra_inceput(ora_inceput);
                ospatar->setOra_sfarsit(ora_sfarsit);
                writer.WriteToCSV(filename, ospatar);
                cout << ospatar->getNume() << endl;
                cout << ospatar->getFunctie() << endl;
                cout << ospatar->getOra_inceput() << endl;
                cout << ospatar->getOra_sfarsit() << endl;
                delete angajatFactory;
            }

            else{
                cerr << "Funcția specificată (" << functie << ") nu este validă." << endl;
            }
        }

        void scoatereAngajat(string nume){
            CSVWriter writer;
            string filename = "angajati.csv";
            writer.EraseFromCSV(filename, nume);
            cout << "A fost sters angajatul cu numele " << nume << endl;
        }

        void afisareAngajati() {
            string filename = "angajati.csv";
            ifstream file(filename);
            string line;

            if (!file.is_open()) {
                cerr << "Nu s-a putut deschide fisierul " << filename << endl;
                return;
            }

            cout << "\n=== Lista Angajatilor Existenti ===" << endl;
            while (getline(file, line)) {
                cout << line << endl; // Afișează fiecare linie din fișier
            }
            file.close();
        }

        void afisareSalarii() {
            int ok = 0;
            string filename1 = "angajati.csv";
            string filename2 = "salarii.csv";
            ifstream file1(filename1);
            ifstream file2(filename2);

            if (!file1.is_open() || !file2.is_open()) {
                cerr << "Nu s-au putut deschide fisierele necesare" << endl;
                return;
            }

            cout << "\n=== Lista Angajatilor si Salariilor Lor ===" << endl;

            string line1, line2;
            double totalSalarii = 0; // Variabilă pentru a ține evidența salariilor totale

            // Citim fiecare linie din angajati.csv
            while (getline(file1, line1)) {
                vector<string> rowAngajat;
                stringstream ss1(line1);
                string value;

                // Împărțim linia din angajati.csv în coloane
                while (getline(ss1, value, ',')) {
                    rowAngajat.push_back(value);
                }

                // Verificăm dacă linia are structura corectă
                if (rowAngajat.size() < 2) {
                    cerr << "Linie invalida in fisierul angajati.csv: " << line1 << endl;
                    continue;
                }

                string numeAngajat = rowAngajat[0]; // Numele angajatului
                string functieAngajat = rowAngajat[1]; // Funcția angajatului
                double salariu = 0;

                // Căutăm salariul corespunzător funcției în fisierul salarii.csv
                ifstream salariiFile(filename2);
                while (getline(salariiFile, line2)) {
                    vector<string> rowSalariu;
                    stringstream ss2(line2);

                    while (getline(ss2, value, ',')) {
                        rowSalariu.push_back(value);
                    }

                    // Verificăm dacă linia are structura corectă
                    if (rowSalariu.size() < 2) {
                        cerr << "Linie invalida in fisierul salarii.csv: " << line2 << endl;
                        continue;
                    }

                    // Dacă funcția se potrivește, obținem salariul
                    if (rowSalariu[0] == functieAngajat) {
                        try {
                            salariu = stod(rowSalariu[1]); // Conversie din string în double
                        } catch (invalid_argument& e) {
                            cerr << "Eroare la conversia salariului pentru functia " << functieAngajat << endl;
                        }
                        break;
                    }
                }
                salariiFile.close();
                ok++;
                if(ok > 1){
                    // Afișăm numele angajatului și salariul
                cout << numeAngajat << " -- " << salariu << " lei" << endl;
                }
                // Adăugăm salariul la totalul general
                totalSalarii += salariu;
            }

            file1.close();
            file2.close();

            // Afișăm totalul salariilor
            cout << "\nTotalul salariilor: " << totalSalarii << " lei" << endl;
        }

};
//functia pentru verificarea inputului de la utilizator
bool esteOraValida(string ora) {
    // Verificare format folosind expresie regulată (HH:MM)
    regex oraPattern("^(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9])$");
    return regex_match(ora, oraPattern);
}
/// Singleton pentru a folosi o singura instanta de Produse, pentru toate functiie legate de produse
class Produse{
    private:
        static Produse* instance;

        Produse(){}
    public:

        static Produse* getInstance() {
            if(instance == nullptr) {
                instance = new Produse();
            }
            return instance;
        }

        void stergereProdus(string nume){
            ifstream inputFile("produse.csv");
            ofstream outputFile("temp.csv");
            string line;

            if (!inputFile.is_open() || !outputFile.is_open()) {
                cerr << "Nu s-a putut deschide fișierul produse.csv sau fișierul temporar." << endl;
                return;
            }
            while (getline(inputFile, line)) {
                if (!line.empty() && line.find(nume) == string::npos) {
                    outputFile << line << "\n";
                }
            }

            inputFile.close();
            outputFile.close();

            remove("produse.csv");
            rename("temp.csv", "produse.csv");
            cout << "A fost sters cu succes produsul " << nume << " din lista de produse" << endl;
        }

        void adaugareComanda(string nume, string produse) {
            ofstream filec("comenzi.csv", ios::app);
            ifstream filep("produse.csv");
            ifstream fidelitateFile("fidelitate.csv");
            ofstream tempFile("temp.csv", ios::app);
            string line1;
            double pret_total = 0, cheltuieli_total = 0;
            double reducere = 0.0; // Valoarea reducerii
            int nrVizitari = 0;
            int procentDiscount = 0;
            bool gasit = false;

            if (!filep.is_open()) {
                cerr << "Nu s-a putut deschide fisierul produse.csv" << endl;
                return;
            }

            // Calculăm prețul total și cheltuielile din produsele comandate
            while (getline(filep, line1)) {
                vector<string> row;
                stringstream ss(line1);
                string value;

                while (getline(ss, value, ',')) {
                    row.push_back(value);
                }

                if (produse.find(";") != string::npos) {
                    vector<string> result;
                    stringstream ss(produse);
                    string token;

                    while (getline(ss, token, ';')) {
                        result.push_back(token);
                    }

                    for (auto produs : result) {
                        if (produs == row[0]) {
                            try {
                                pret_total += stod(row[1]);
                                cheltuieli_total += stod(row[2]);
                            } catch (invalid_argument& e) {
                                cout << "Eroare: Pret invalid" << endl;
                            } catch (out_of_range& e) {
                                cout << "Eroare: Pret invalid" << endl;
                            }
                        }
                    }
                } else {
                    if (produse == row[0]) {
                        try {
                            pret_total += stod(row[1]);
                            cheltuieli_total += stod(row[2]);
                        } catch (invalid_argument& e) {
                            cout << "Eroare: Pret invalid" << endl;
                        } catch (out_of_range& e) {
                            cout << "Eroare: Pret invalid" << endl;
                        }
                    }
                }
            }
            filep.close();

            // Gestiunea fidelității
            if (!fidelitateFile.is_open()) {
                cerr << "Nu s-a putut deschide fisierul fidelitate.csv" << endl;
                return;
            }

            while (getline(fidelitateFile, line1)) {
                vector<string> row;
                stringstream ss(line1);
                string value;

                while (getline(ss, value, ',')) {
                    row.push_back(value);
                }

                if (!row.empty() && row[0] == nume) {
                    gasit = true;
                    nrVizitari = stoi(row[1]) + 1; // Crește nr_vizitari cu 1
                    procentDiscount = (nrVizitari % 6 == 0) ? 20 : 0; // Discount de 10% la fiecare 6 vizite

                    // Aplicăm reducerea la prețul total
                    reducere = (procentDiscount / 100.0) * pret_total;
                    pret_total -= reducere;

                    // Scriem linia actualizată în fișierul temporar
                    tempFile << nume << "," << nrVizitari << "," << procentDiscount << "\n";

                    // Afișăm mesajul despre vizite
                    if (procentDiscount == 0) {
                        cout << "Mai ai " << (6 - (nrVizitari % 6)) << " vizite pana la un discount." << endl;
                    } else {
                        cout << "Ai primit un discount de 20% pentru comanda curenta!" << endl;
                    }
                } else {
                    // Scriem linia nemodificată în fișierul temporar
                    tempFile << line1 << "\n";
                }
            }

            fidelitateFile.close();

            // Dacă persoana nu a fost găsită, o adăugăm cu valori implicite
            if (!gasit) {
                tempFile << nume << ",1,0\n";
                cout << "Mai ai 5 vizite pana la un discount." << endl;
            }
            tempFile.close();

            // Înlocuim fișierul original cu cel temporar
            remove("fidelitate.csv");
            rename("temp.csv", "fidelitate.csv");

            // Adăugăm comanda în comenzi.csv
            string dataCurenta = obtineDataCurenta();
            if (filec.is_open()) {
                filec << dataCurenta << "," << nume << "," << produse << "," << pret_total << "," << cheltuieli_total << "\n";
                cout << "Comanda a fost adaugata cu succes" << endl;
            } else {
                cerr << "Nu s-a putut deschide fisierul comenzi.csv" << endl;
            }
            filec.close();
        }

        void adaugareProdus(string nume, double pret){
            ofstream file;
            file.open("produse.csv", ios::app);
            if(file.is_open()){
                file <<  nume << "," << pret << "\n";
                file.close();
                cout << "A fost adaugat cu succes produsul " << nume << " avand un pret de " << pret << " lei" << endl;
            }
            else
                cerr << "Nu s-a putut deschide fisierul produse.csv" << endl;
        }

        void afisareProduse(const string& filename) {
            ifstream file(filename);
            string line, token;

            if (!file.is_open()) {
                cerr << "Nu s-a putut deschide fisierul " << filename << endl;
                return;
            }

            cout << "\n=== Stocul de produse ===" << endl;

            while (getline(file, line)) {
                vector<string> row;
                stringstream ss(line);
                row.clear();
                while(getline(ss, token, ',')){
                row.push_back(token);
                }   
                cout << row[0] << " -- " << row[1] << " lei" << endl;
            }
            file.close();
        }

        void genereazaRaportZilnic(string dataSpecificata) {
            ifstream comenziFile("comenzi.csv");
            ifstream rapoarteFile("rapoarte_financiare.csv");
            ofstream tempFile("temp.csv");
            string line;
            double totalVenituri = 0, totalCheltuieli = 0;
            bool dataExista = false;

            if (!comenziFile.is_open()) {
                cerr << "Nu s-a putut deschide fisierul comenzi.csv" << endl;
                return;
            }

            if (!rapoarteFile.is_open() || !tempFile.is_open()) {
                cerr << "Nu s-au putut deschide fisierele necesare" << endl;
                return;
            }

            // Parcurgem comenzi.csv și calculăm totalurile pentru data specificată
            while (getline(comenziFile, line)) {
                vector<string> row;
                stringstream ss(line);
                string value;

                while (getline(ss, value, ',')) {
                    row.push_back(value);
                }

                if (!row.empty() && row[0] == dataSpecificata) {
                    try {
                        totalVenituri += stod(row[3]);
                        totalCheltuieli += stod(row[4]);
                    } catch (invalid_argument& e) {
                        cerr << "Eroare la conversia datelor în comenzi.csv" << endl;
                    }
                }
            }
            comenziFile.close();

            // Rescriem rapoarte_financiare.csv, actualizând data specificată sau adăugând-o
            while (getline(rapoarteFile, line)) {
                vector<string> row;
                stringstream ss(line);
                string value;

                while (getline(ss, value, ',')) {
                    row.push_back(value);
                }

                if (!row.empty() && row[0] == dataSpecificata) {
                    dataExista = true;
                    tempFile << dataSpecificata << "," << totalVenituri << "," << totalCheltuieli << "," << (totalVenituri - totalCheltuieli) << "\n";
                } else {
                    tempFile << line << "\n";
                }
            }

            if (!dataExista) {
                tempFile << dataSpecificata << "," << totalVenituri << "," << totalCheltuieli << "," << (totalVenituri - totalCheltuieli) << "\n";
            }

            rapoarteFile.close();
            tempFile.close();

            // Înlocuim fișierul original cu cel temporar
            remove("rapoarte_financiare.csv");
            rename("temp.csv", "rapoarte_financiare.csv");

            cout << "Raportul pentru data " << dataSpecificata << " a fost procesat." << endl;
        }

    
};

class EvenimenteSpeciale {
private:
    static EvenimenteSpeciale* instance;

    EvenimenteSpeciale() {}

public:
    static EvenimenteSpeciale* getInstance() {
        if (instance == nullptr) {
            instance = new EvenimenteSpeciale();
        }
        return instance;
    }

    void adaugaEvenimentSpecial() {
        ofstream file("evenimente_speciale.csv", ios::app);
        if (!file.is_open()) {
            cerr << "Nu s-a putut deschide fisierul evenimente_speciale.csv" << endl;
            return;
        }

        string nume_eveniment, data, descriere;
        double costuri;

        cout << "Introduceti numele evenimentului: ";
        cin.ignore();
        getline(cin, nume_eveniment);

        cout << "Introduceti data evenimentului (YYYY-MM-DD): ";
        cin >> data;

        cout << "Introduceti costurile totale ale evenimentului: ";
        cin >> costuri;

        cout << "Introduceti o scurta descriere a evenimentului: ";
        cin.ignore();
        getline(cin, descriere);

        // Scriem evenimentul în fișierul CSV cu delimitatori corecți
        file << nume_eveniment << "," << data << "," << costuri << "," << descriere << "\n";
        file.close();

        cout << "Evenimentul a fost adaugat cu succes!" << endl;
    }

    void afiseazaEvenimenteSpeciale() {
        ifstream file("evenimente_speciale.csv");
        if (!file.is_open()) {
            cerr << "Nu s-a putut deschide fisierul evenimente_speciale.csv" << endl;
            return;
        }

        cout << "\n=== Lista Evenimentelor Speciale ===" << endl;
        string line;

        while (getline(file, line)) {
            vector<string> row;
            stringstream ss(line);
            string value;

            while (getline(ss, value, ',')) {
                row.push_back(value);
            }

            if (row.size() >= 4) {
                cout << "Nume: " << row[0] << ", Data: " << row[1]
                     << ", Costuri: " << row[2] << " lei, Descriere: " << row[3] << endl;
            }
        }

        file.close();
    }

    void calculeazaCosturiEvenimente(const string& dataCautata) {
        ifstream file("evenimente_speciale.csv");
        if (!file.is_open()) {
            cerr << "Nu s-a putut deschide fisierul evenimente_speciale.csv" << endl;
            return;
        }

        double totalCosturi = 0;
        string line;

        while (getline(file, line)) {
            vector<string> row;
            stringstream ss(line);
            string value;

            while (getline(ss, value, ',')) {
                row.push_back(value);
            }

            if (row.size() >= 3 && row[1] == dataCautata) {
                try {
                    totalCosturi += stod(row[2]);
                } catch (invalid_argument& e) {
                    cerr << "Eroare la conversia costurilor pentru eveniment: " << row[0] << endl;
                }
            }
        }

        file.close();

        cout << "Costurile totale pentru evenimentele din data " << dataCautata
             << " sunt: " << totalCosturi << " lei" << endl;
    }
};
/// Declarare a variabilelor statice
EvenimenteSpeciale* EvenimenteSpeciale::instance = nullptr;
Produse* Produse::instance = nullptr;
ActiuniAngajati* ActiuniAngajati::instance = nullptr;
/// Functie pentru meniul interactiv
void meniuInteractiv() {
    int optiunePrincipala = 0;
    Produse* produseManager = Produse::getInstance();
    ActiuniAngajati* controlAngajati = ActiuniAngajati::getInstance();

    while (true) {
        cout << "\n=== Meniu Principal ===" << endl;
        cout << "1. Client" << endl;
        cout << "2. Staff" << endl;
        cout << "3. Genereaza raport zilnic" << endl;
        cout << "4. Evenimente speciale" << endl;
        cout << "5. Iesire" << endl;
        cout << "Introduceti optiunea: ";
        cin >> optiunePrincipala;

        if (cin.fail() || optiunePrincipala < 1 || optiunePrincipala > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Optiunea introdusa nu este valida. Introduceti un numar intre 1 si 5." << endl;
            continue;
        }

        switch (optiunePrincipala) {
            case 1: { // Client
                int optiuneClient = 0;

                while (true) {
                    cout << "\n=== Meniu Client ===" << endl;
                    cout << "1. Vezi stocul de produse" << endl;
                    cout << "2. Plaseaza o comanda" << endl;
                    cout << "3. Inapoi la meniul principal" << endl;
                    cout << "Introduceti optiunea: ";
                    cin >> optiuneClient;

                    if (cin.fail() || optiuneClient < 1 || optiuneClient > 3) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Optiunea introdusa nu este valida. Introduceti un numar intre 1 si 3." << endl;
                        continue;
                    }

                    if (optiuneClient == 1) {
                        produseManager->afisareProduse("produse.csv");
                    } else if (optiuneClient == 2) {
                        string nume, produse;
                        produseManager->afisareProduse("produse.csv");
                        cout << "Ce produse doriti sa comandati?(separate prin ;)" << endl;
                        cin >> produse;
                        cout << "Ce nume aveti?" << endl;
                        cin >> nume;
                        cout << "INFO: LA 6 COMENZI, URMATOAREA COMANDA VA AVEA O REDUCERE DE 20%" << endl;
                        produseManager->adaugareComanda(nume, produse);
                    } else {
                        break; // Inapoi la meniul principal
                    }
                }
                break;
            }
            case 2: { // Staff
                int optiuneStaff = 0;

                while (true) {
                    cout << "\n=== Meniu Staff ===" << endl;
                    cout << "1. Adauga angajat" << endl;
                    cout << "2. Sterge angajat" << endl;
                    cout << "3. Vezi si STERGE produse din stoc" << endl;
                    cout << "4. Vezi si ADAUGA produse in stoc" << endl;
                    cout << "5. Vezi angajatii si salariile lor" << endl;
                    cout << "6. Inapoi la meniul principal" << endl;
                    cout << "Introduceti optiunea: ";
                    cin >> optiuneStaff;

                    if (cin.fail() || optiuneStaff < 1 || optiuneStaff > 6) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Optiunea introdusa nu este valida. Introduceti un numar intre 1 si 6." << endl;
                        continue;
                    }

                    if (optiuneStaff == 1) {
                        string nume, functie, ora_inceput, ora_sfarsit;
                        cout << "Introduceti numele angajatului: ";
                        cin >> nume;
                        cout << "Introduceti functia (barista/manager/ospatar): ";
                        cin >> functie;

                        if (functie != "barista" && functie != "manager" && functie != "ospatar") {
                            cout << "Functia introdusa nu este valida. Incercati din nou." << endl;
                            continue;
                        }

                        do {
                            cout << "Introduceti ora de inceput (hh:mm): ";
                            cin >> ora_inceput;
                            if (!esteOraValida(ora_inceput)) {
                                cout << "Ora introdusa nu este valida. Formatul corect este HH:MM." << endl;
                            }
                        } while (!esteOraValida(ora_inceput));

                        do {
                            cout << "Introduceti ora de sfarsit (hh:mm): ";
                            cin >> ora_sfarsit;
                            if (!esteOraValida(ora_sfarsit)) {
                                cout << "Ora introdusa nu este valida. Formatul corect este HH:MM." << endl;
                            }
                        } while (!esteOraValida(ora_sfarsit));

                        controlAngajati->adaugareAngajat(nume, functie, ora_inceput, ora_sfarsit);
                    } else if (optiuneStaff == 2) {
                        controlAngajati->afisareAngajati();
                        string nume;
                        cout << "Introduceti numele angajatului de sters: ";
                        cin >> nume;
                        controlAngajati->scoatereAngajat(nume);
                    } else if (optiuneStaff == 3) {
                        produseManager->afisareProduse("produse.csv");
                        string numeProdus;
                        cout << "Introduceti numele produsului de sters: ";
                        cin >> numeProdus;
                       // stergereProdus(numeProdus);
                    } else if(optiuneStaff == 4){
                        produseManager->afisareProduse("produse.csv");
                        string numeProdus;
                        cout << "Introduceti numele produsului pe care vreti sa-l adaugati: ";
                        cin >> numeProdus;
                        double pretProdus;
                        cout << "Introduceti pretul produsului pe care vreti sa-l adaugati: ";
                        cin >> pretProdus;
                        produseManager->adaugareProdus(numeProdus, pretProdus);
                    } 
                    else if (optiuneStaff == 5) {
                        controlAngajati->afisareSalarii();
                    }
                    else {
                        break; // Inapoi la meniul principal
                    }
                }
                break;
            }
            case 3: {
                while(true){
                    string data;
                    int optiuneRaport = 0;
                    cout << "=== Raport zilnic ===" << endl;
                    cout << "1. Generare raport zilnic pentru ziua curenta" << endl;
                    cout << "2. Generare raport zilnic pentru o alta zi" << endl;
                    cout << "3. Inapoi la meniul principal" << endl;
                    cout << "Introduceti optiunea: ";
                    cin >> optiuneRaport;
                    if(optiuneRaport == 1){
                        produseManager->genereazaRaportZilnic(obtineDataCurenta());
                    }
                    else if(optiuneRaport == 2){
                        cout << "Introduceti data pentru care doriti sa generati raportul (YYYY-MM-DD): ";
                        cin >> data;
                        produseManager->genereazaRaportZilnic(data);
                    }
                    else{
                        break;
                    }
                }
                break;
            }
            case 4:{
                EvenimenteSpeciale* evenimenteManager = EvenimenteSpeciale::getInstance();
                int optiuneEvenimente = 0;

                while (true) {
                    cout << "\n=== Meniu Evenimente Speciale ===" << endl;
                    cout << "1. Adauga eveniment special" << endl;
                    cout << "2. Afiseaza evenimente speciale" << endl;
                    cout << "3. Calculeaza costurile pentru o data" << endl;
                    cout << "4. Inapoi la meniul principal" << endl;
                    cout << "Introduceti optiunea: ";
                    cin >> optiuneEvenimente;

                    if (cin.fail() || optiuneEvenimente < 1 || optiuneEvenimente > 4) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Optiunea introdusa nu este valida. Incercati din nou." << endl;
                        continue;
                    }

                    if (optiuneEvenimente == 1) {
                        evenimenteManager->adaugaEvenimentSpecial();
                    } else if (optiuneEvenimente == 2) {
                        evenimenteManager->afiseazaEvenimenteSpeciale();
                    } else if (optiuneEvenimente == 3) {
                        string dataCautata;
                        cout << "Introduceti data pentru care doriti sa calculati costurile (YYYY-MM-DD): ";
                        cin >> dataCautata;
                        evenimenteManager->calculeazaCosturiEvenimente(dataCautata);
                    } else {
                        break; // Inapoi la meniul principal
                    }
                }
                break;
            }
            case 5: {
                cout << "Aplicatia se inchide..." << endl;
                return;
            }
        }
    }
}

int main(){
    meniuInteractiv();
    return 0;
}

///Calcul cheltuiala per shot de espresso: 150 lei pt 1kg de boabe de specialitate --> 1.33 lei per shot
///Calcul cheltuiala per latte: 16 lei pt 1.5L lapte, iar pt un latte sau cappucino se folosesc 120mL --> 0.48 lei per latte sau cappucino(doar pt lapte)
/**
 espresso: 1.33 per shot
 doppio: 2.66 lei
 latte: 1.81 lei
 cappucino: 1.81 lei
 flat white: 3.14 lei
 */
