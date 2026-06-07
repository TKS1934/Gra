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

// Zmieniamy argumenty - zamiast x i y, podajemy nazwę pliku
vector<vector<blok>> generowanie_planszy_z_pliku(const std::string& nazwa_pliku, const sf::RenderWindow& window, float graw) {
    vector<vector<int>> t = wczytaj_mape_z_pliku(nazwa_pliku);
    vector<vector<blok>> tk;

    if (t.empty() || t[0].empty()) {
        return tk; // Zabezpieczenie przed pustym plikiem
    }

    // Dynamicznie obliczamy wymiary wczytanej mapy
    int w_p_y = t.size();       // ilość rzędów
    int w_p_x = t[0].size();    // ilość kolumn

    // Obliczamy wielkość pojedynczego bloku tak, by wypełnił okno
    sf::Vector2f size1 = {float(window.getSize().x) / w_p_x, float(window.getSize().y) / w_p_y};

    int j = 0; // Oś Y
    for (auto w : t) {
        vector<blok> pom;
        int i = 0; // Oś X
        for (auto z : w) {
            // Obliczamy fizyczną pozycję bloku w pikselach
            sf::Vector2f pozycja = {float(i * size1.x), float(j * size1.y)};

            blok pom1(size1, pozycja, {0,0}, window, graw, z);
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
void kolizja_b_gra(gracz& bi, const sf::RenderWindow& window) {
    sf::FloatRect b = bi.getGlobalBounds();

    // Kolizja z lewą krawędzią okna
    if (b.left < 0) {
        bi.setPosition(0, bi.getPosition().y); // Zmieniono z 20 na 0 dla precyzji, dopasuj jeśli potrzebujesz marginesu
        bi.velocity.x = 0;
    }
    // Kolizja z prawą krawędzią okna
    if (b.left + b.width > window.getSize().x) {
        bi.setPosition(window.getSize().x - b.width, bi.getPosition().y);
        bi.velocity.x = 0;
    }
    // Kolizja z sufitem okna
    if (b.top < 0) {
        bi.setPosition(bi.getPosition().x, 0);
        bi.velocity.y = 0;
    }
    // Kolizja z podłogą okna
    if (b.top + b.height > window.getSize().y) {
        bi.setPosition(bi.getPosition().x, window.getSize().y - b.height);
        bi.velocity.y = 0;
        bi.tG = true; // Gracz stoi na dole okna
    }
}

// Rozbiliśmy funkcję kolizji na dwie niezależne osie
void kolizja_bloki_X(gracz& g1, vector<vector<blok>>& t) {
    for (auto &pom : t) {
        for (auto &r : pom) {
            if (!r.fispfp()) {
                if (g1.getGlobalBounds().intersects(r.getGlobalBounds())) {
                    if (g1.velocity.x > 0) { // Ruch w prawo -> kolizja z lewą ścianą bloku
                        g1.setPosition(r.getPosition().x - g1.getGlobalBounds().width, g1.getPosition().y);
                    }
                    else if (g1.velocity.x < 0) { // Ruch w lewo -> kolizja z prawą ścianą bloku
                        g1.setPosition(r.getPosition().x + r.getGlobalBounds().width, g1.getPosition().y);
                    }
                    g1.velocity.x = 0;
                }
            }
        }
    }
}

void kolizja_bloki_Y(gracz& g1, vector<vector<blok>>& t) {
    g1.tG = false; // Resetujemy stan "na ziemi" przed sprawdzeniem kolizji

    for (auto &pom : t) {
        for (auto &r : pom) {
            if (!r.fispfp()){
                if (g1.getGlobalBounds().intersects(r.getGlobalBounds())) {
                    if (g1.velocity.y > 0) { // Spadanie w dół -> lądowanie na bloku
                        g1.setPosition(g1.getPosition().x, r.getPosition().y - g1.getGlobalBounds().height);
                        g1.tG = true;
                    }
                    else if (g1.velocity.y < 0) { // Skok w górę -> uderzenie w sufit bloku
                        g1.setPosition(g1.getPosition().x, r.getPosition().y + r.getGlobalBounds().height);
                    }
                    g1.velocity.y = 0;
                }
            }
        }
    }
}

void ruch(sf::Time elapsed, gracz& g1, vector<vector<blok>>& t, const float gravity, const sf::RenderWindow& window) {
    float dt = elapsed.asSeconds();

    // 1. Grawitacja (zastosowanie delta time)
    g1.velocity.y += gravity * dt;

    // 2. Krok Osi X: Ruch -> Kolizje z blokami
    g1.move(g1.velocity.x * dt, 0);
    kolizja_bloki_X(g1, t);

    // 3. Krok Osi Y: Ruch -> Kolizje z blokami
    g1.move(0, g1.velocity.y * dt);
    kolizja_bloki_Y(g1, t);

    // 4. Marginesy mapy / okna na samym końcu
    kolizja_b_gra(g1, window);
}





