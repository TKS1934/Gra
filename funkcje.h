#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//using namespace std;

vector<vector<int>> wczytaj_mape_z_pliku(const string& nazwa_pliku) {
    vector<vector<int>> wczytana_mapa;
    ifstream plik(nazwa_pliku);

    if (!plik.is_open()) {
        cout << "Blad: Nie mozna otworzyc pliku " << nazwa_pliku << "!" << endl;
        return wczytana_mapa;
    }

    string linia;

    while (getline(plik, linia)) {
        vector<int> rzad;
        stringstream ss(linia);
        int typ_bloku;


        while (ss >> typ_bloku) {
            rzad.push_back(typ_bloku);
        }
        wczytana_mapa.push_back(rzad);
    }

    plik.close();
    return wczytana_mapa;
}

vector<vector<blok>> generowanie_planszy_z_pliku(const std::string& nazwa_pliku, const sf::RenderWindow& window, float graw) {


    static std::vector<sf::Texture> teksturyBlokow(4);
    static bool czyZaladowanoTekstury = false;


    if (!czyZaladowanoTekstury) {
        teksturyBlokow[1].loadFromFile("cegla4.png");
        teksturyBlokow[2].loadFromFile("stop sign.png");
        teksturyBlokow[3].loadFromFile("liscie.png");
        czyZaladowanoTekstury = true;
    }


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




void kolizja_b_gra(gracz& bi, const sf::RenderWindow& window) {
    sf::FloatRect b = bi.getGlobalBounds();

    // Kolizja z lewą krawędzią okna
    if (b.left < 0) {
        bi.setPosition(0, bi.getPosition().y);
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

void kolizja_skrzynka_bloki_X(skrzynka& s, vector<vector<blok>>& t) {
    for (auto &pom : t) {
        for (auto &r : pom) {
            if (!r.fispfb()) {
                if (s.getGlobalBounds().intersects(r.getGlobalBounds())) {
                    if (s.getPosition().x < r.getPosition().x) { // Ruch w prawo
                        s.setPosition(r.getPosition().x - s.getGlobalBounds().width, s.getPosition().y);
                    } else { // Ruch w lewo
                        s.setPosition(r.getPosition().x + r.getGlobalBounds().width, s.getPosition().y);
                    }
                }
            }
        }
    }
}

void kolizja_skrzynka_bloki_Y(skrzynka& s, vector<vector<blok>>& t) {
    for (auto &pom : t) {
        for (auto &r : pom) {
            if (!r.fispfb()) {
                if (s.getGlobalBounds().intersects(r.getGlobalBounds())) {
                    if (s.getVelocity().y > 0) { // Spadanie na blok
                        s.setPosition(s.getPosition().x, r.getPosition().y - s.getGlobalBounds().height);
                    } else if (s.getVelocity().y < 0) {
                        s.setPosition(s.getPosition().x, r.getPosition().y + r.getGlobalBounds().height);
                    }
                    s.getVelocity().y = 0;
                }
            }
        }
    }
}


void ruch(sf::Time elapsed, gracz& g1, skrzynka& s, vector<vector<blok>>& t, const float gravity, const sf::RenderWindow& window) {
    float dt = elapsed.asSeconds();

    // Grawitacja
    g1.velocity.y += gravity * dt;
    s.getVelocity().y += gravity * dt;

    // Najpierw fizycznie przesuwamy gracza w poziomie
    g1.move(g1.velocity.x * dt, 0);

    //  kolizja gracza z mapą w osi X
    kolizja_bloki_X(g1, t);

    //  Wykrywanie interakcji ze skrzynką w osi X
    if (g1.getGlobalBounds().intersects(s.getGlobalBounds())) {
        if (g1.velocity.x > 0) { // Gracz idzie w prawo -> pcha skrzynkę w prawo
            s.setPosition(g1.getGlobalBounds().left + g1.getGlobalBounds().width, s.getPosition().y);
            kolizja_skrzynka_bloki_X(s, t);

            // Jeśli skrzynka natrafiła na ścianę i cofnęła się na gracza:
            if (g1.getGlobalBounds().intersects(s.getGlobalBounds())) {
                g1.setPosition(s.getGlobalBounds().left - g1.getGlobalBounds().width, g1.getPosition().y);
                g1.velocity.x = 0;
            }
        }
        else if (g1.velocity.x < 0) { // Gracz idzie w lewo -> pcha skrzynkę w lewo
            s.setPosition(g1.getGlobalBounds().left - s.getGlobalBounds().width, s.getPosition().y);
            kolizja_skrzynka_bloki_X(s, t);

            // Jeśli skrzynka natrafiła na ścianę i cofnęła się na gracza:
            if (g1.getGlobalBounds().intersects(s.getGlobalBounds())) {
                g1.setPosition(s.getGlobalBounds().left + s.getGlobalBounds().width, g1.getPosition().y);
                g1.velocity.x = 0;
            }
        }
    }


    // Ruch pionowy skrzynki kolizja z podłogą mapy
    s.move(0, s.getVelocity().y * dt);
    kolizja_skrzynka_bloki_Y(s, t);

    // Ruch pionowy gracza
    g1.move(0, g1.velocity.y * dt);


    // Resetujemy stan stanięcia na ziemi
    g1.tG = false;

    kolizja_bloki_Y(g1, t); // Kolizja pionowa z kafelkami mapy

    // Kolizja pionowa gracza ze skrzynką
    if (g1.getGlobalBounds().intersects(s.getGlobalBounds())) {
        if (g1.velocity.y > 0) { // Gracz spada na skrzynkę
            g1.setPosition(g1.getPosition().x, s.getGlobalBounds().top - g1.getGlobalBounds().height);
            g1.tG = true; // Gracz traktuje skrzynkę jak podłogę
        }
        else if (g1.velocity.y < 0) { // Gracz uderza głową w skrzynkę od dołu
            g1.setPosition(g1.getPosition().x, s.getGlobalBounds().top + s.getGlobalBounds().height);
        }
        g1.velocity.y = 0; // Zerujemy prędkość pionową
    }

    kolizja_b_gra(g1, window);
}