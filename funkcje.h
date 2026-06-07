#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;

vector<vector<int>> generowanie_pol(int x,int y){
    int i,j;
    vector<vector<int>> t;
    srand(time(NULL));
    for(i=0;i<y;i++){
        vector<int> pom;
        for(j=0;j<x;j++){
            int z=rand()%4;
            pom.push_back(z);
        }
        t.push_back(pom);
    }
    return t;

}
vector<vector<int>> wczytaj_mape_z_pliku(const string& nazwa_pliku) {
    vector<vector<int>> wczytana_mapa;
    ifstream plik(nazwa_pliku);

    if (!plik.is_open()) {
        cout << "Blad: Nie mozna otworzyc pliku " << nazwa_pliku << "!" << endl;
        return wczytana_mapa; // Zwraca puste, jeśli plik nie istnieje
    }

    string linia;
    // Czytamy plik linijka po linijce (to będą nasze rzędy Y)
    while (getline(plik, linia)) {
        vector<int> rzad;
        stringstream ss(linia);
        int typ_bloku;

        // Czytamy kolejne liczby z danej linijki oddzielone spacją (nasze X)
        while (ss >> typ_bloku) {
            rzad.push_back(typ_bloku);
        }
        wczytana_mapa.push_back(rzad);
    }

    plik.close();
    return wczytana_mapa;
}

vector<vector<blok>> generowanie_planszy_z_pliku(const std::string& nazwa_pliku, const sf::RenderWindow& window, float graw) {

    // 1. Zmienne statyczne - żyją przez cały czas działania gry
    static std::vector<sf::Texture> teksturyBlokow(4);
    static bool czyZaladowanoTekstury = false;

    // 2. Ładujemy tekstury tylko RAZ, przy pierwszym generowaniu mapy
    if (!czyZaladowanoTekstury) {
        teksturyBlokow[1].loadFromFile("cegla3.png");
        teksturyBlokow[2].loadFromFile("stop sign.png");
        teksturyBlokow[3].loadFromFile("liscie.png");
        czyZaladowanoTekstury = true; // Zapamiętujemy, żeby nie ładować ponownie
    }

    // 3. Wczytywanie pliku tekstowego
    vector<vector<int>> t = wczytaj_mape_z_pliku(nazwa_pliku);
    vector<vector<blok>> tk;

    if (t.empty() || t[0].empty()) return tk;

    int w_p_y = t.size();
    int w_p_x = t[0].size();
    sf::Vector2f size1 = {float(window.getSize().x) / w_p_x, float(window.getSize().y) / w_p_y};

    int j = 0;
    for (auto w : t) {
        vector<blok> pom;
        int i = 0;
        for (auto z : w) {
            sf::Vector2f pozycja = {float(i * size1.x), float(j * size1.y)};

            // Przypisanie wskaźnika do STATYCZNEJ tekstury
            const sf::Texture* tex = (z > 0 && z < teksturyBlokow.size()) ? &teksturyBlokow[z] : nullptr;

            blok pom1(size1, pozycja, {0,0}, window, graw, z, tex);
            pom.push_back(pom1);
            i++;
        }
        tk.push_back(pom);
        j++;
    }
    return tk;
}
vector<vector<blok>> generowanie_planszy(int x,int y, const sf::RenderWindow& window,float graw){
    vector<vector<int>> t=generowanie_pol(x,y);
    sf::Vector2f size1={float(window.getSize().x)/x,float(window.getSize().y)/y};
    vector<vector<blok>> tk;
    int i=0,j=0;
    for(auto w:t){
        vector<blok> pom;
        for(auto z:w){
            blok pom1(size1,{float(i*size1.x),float(j*size1.y)},{0,0},window,graw,z);
            pom.push_back(pom1);
            i++;
        }
        tk.push_back(pom);
        j++;
        i=0;
    }
    return tk;
    //blok *obj;
    // obj = new blok(size1,{200,550},{0,-10},window,grawitacja,1);
    //(const sf::Vector2f& size,const sf::Vector2f& position, const sf::Vector2f& velocity, const sf::RenderWindow& window,float grawitacja, int ID)
}

