#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

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
                    if (g1.velocity.x > 0) { // Kolizja z lewą ścianą bloku
                        g1.setPosition(r.getPosition().x - g1.getGlobalBounds().width, g1.getPosition().y);
                    }
                    else if (g1.velocity.x < 0) { // Kolizja z prawą ścianą bloku
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
                    if (g1.velocity.y > 0) { // Spadanie w dół
                        g1.setPosition(g1.getPosition().x, r.getPosition().y - g1.getGlobalBounds().height);
                        g1.tG = true;
                    }
                    else if (g1.velocity.y < 0) { // Skok w górę
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

void aktualizuj_hak(Hak& hak, gracz& g1, vector<vector<blok>>& t, float dt, skrzynka& s) {
    if (hak.stan == StanHaka::Nieaktywny) return;

    // Lot haka
    if (hak.stan == StanHaka::Wystrzelony) {
        hak.pozycja += hak.kierunek * hak.predkosc * dt;
        sf::FloatRect boundsHaka(hak.pozycja.x - 3, hak.pozycja.y - 3, 8, 8); // Większy punkt kolizji liny

        // Sprawdzamy skrzynkę
        if (boundsHaka.intersects(s.getGlobalBounds())) {
            hak.stan = StanHaka::PrzyciagaSkrzynke;
            hak.trafionaSkrzynka = &s;
            return;
        }

        // Sprawdzamy stałe bloki
        for (auto &pom : t) {
            for (auto &r : pom) {
                if (!r.fispfp() || !r.fispfb()) {
                    if (boundsHaka.intersects(r.getGlobalBounds())) {
                        hak.stan = StanHaka::PrzyciagaGracza;
                        return;
                    }
                }
            }
        }

        // Jeśli hak poleci za daleko znika
        float dx = hak.pozycja.x - g1.getPosition().x;
        float dy = hak.pozycja.y - g1.getPosition().y;
        if (std::hypot(dx, dy) > 700.f) {
            hak.stan = StanHaka::Nieaktywny;
        }
    }

    // Hak przykleja się do środka skrzynki
    if (hak.stan == StanHaka::PrzyciagaSkrzynke && hak.trafionaSkrzynka != nullptr) {

        hak.pozycja = hak.trafionaSkrzynka->getPosition() + (hak.trafionaSkrzynka->getSize() / 2.0f);
    }
}

void rysuj_hak(sf::RenderWindow& window, const gracz& g1, const Hak& hak) {
    if (hak.stan != StanHaka::Nieaktywny) {
        sf::VertexArray lina(sf::Lines, 2);
        lina[0].position = g1.getPosition() + sf::Vector2f(5, 10); // Środek gracza
        lina[0].color = sf::Color(200, 200, 200);
        lina[1].position = hak.pozycja;
        lina[1].color = sf::Color(100, 100, 100);
        window.draw(lina);
    }
}


bool kolizje_z_tlem(const sf::FloatRect& bounds, vector<vector<blok>>& t) {
    for (auto& rzad : t) {
        for (auto& b : rzad) {
            if (!b.fispfp() && bounds.intersects(b.getGlobalBounds())) {
                return true;
            }
        }
    }
    return false;
}


void ruch(sf::Time elapsed, gracz& g1, skrzynka& s, vector<vector<blok>>& t, const float gravity, const sf::RenderWindow& window, Hak& hak) {
    float dt = elapsed.asSeconds();

    aktualizuj_hak(hak, g1, t, dt, s);

    // Sprawdzenie czy gracz stoi na skrzynce
    bool naSkrzynce = false;
    sf::FloatRect plBounds = g1.getGlobalBounds();
    sf::FloatRect skBounds = s.getGlobalBounds();

    if (plBounds.left + plBounds.width > skBounds.left &&
        plBounds.left < skBounds.left + skBounds.width &&
        std::abs((plBounds.top + plBounds.height) - skBounds.top) < 5.0f) {
        naSkrzynce = true;
    }

    // Hak z graczem
    if (hak.stan == StanHaka::PrzyciagaGracza) {
        sf::Vector2f kierunek = hak.pozycja - g1.getPosition();
        float odleglosc = std::hypot(kierunek.x, kierunek.y);

        if (odleglosc > 10.f) {
            g1.velocity = (kierunek / odleglosc) * hak.silaPrzyciagania*0.8f;
        } else {
            hak.stan = StanHaka::Nieaktywny;
            g1.velocity = {0, 0};
        }
    } else {
        g1.velocity.y += gravity * dt;
    }

    g1.move(g1.velocity.x * dt, 0);
    kolizja_bloki_X(g1, t);

    // Hak ze skrzynką
    s.getVelocity().y += gravity * dt;

    if (hak.stan == StanHaka::PrzyciagaSkrzynke) {
        sf::Vector2f doGracza = g1.getPosition() - s.getPosition();
        float odleglosc = std::hypot(doGracza.x, doGracza.y);

        if (odleglosc > 60.f) {
            sf::Vector2f dir = doGracza / odleglosc;

            float sila = hak.silaPrzyciagania * (odleglosc / 100.f);
            if (sila > 480.f) sila = 480.f;

            s.getVelocity().x = dir.x * sila;

            if (dir.y < 0 && !naSkrzynce) {
                s.getVelocity().y = dir.y * sila;
            }
        } else {
            s.getVelocity().x *= 0.7f;
            if (std::abs(s.getVelocity().x) < 10.f) s.getVelocity().x = 0;

            if (s.getVelocity().y < 0) {
                s.getVelocity().y = 0;
            }
        }
    } else {
        s.getVelocity().x = 0;
    }

    plBounds = g1.getGlobalBounds();
    skBounds = s.getGlobalBounds();
    // Pchanie skrzynki
    if (plBounds.intersects(skBounds)) {



        if (plBounds.top + plBounds.height - 4.0f > skBounds.top &&
            plBounds.top + 4.0f < skBounds.top + skBounds.height) {
            if (g1.velocity.x > 0) {
                s.setPosition(plBounds.left + plBounds.width, s.getPosition().y);
                kolizja_skrzynka_bloki_X(s, t);
                if (plBounds.intersects(skBounds)) {
                    g1.setPosition(skBounds.left - plBounds.width, g1.getPosition().y);
                    g1.velocity.x = 0;
                }
            } else if (g1.velocity.x < 0) {
                s.setPosition(plBounds.left - skBounds.width, s.getPosition().y);
                kolizja_skrzynka_bloki_X(s, t);
                if (plBounds.intersects(skBounds)) {
                    g1.setPosition(skBounds.left + skBounds.width, g1.getPosition().y);
                    g1.velocity.x = 0;
                }
            }
        }
    }

    // Ruch skrzynki
    s.move(s.getVelocity().x * dt, 0);
    kolizja_skrzynka_bloki_X(s, t);

    s.move(0, s.getVelocity().y * dt);
    kolizja_skrzynka_bloki_Y(s, t);

    g1.move(0, g1.velocity.y * dt);
    g1.tG = false;

    plBounds = g1.getGlobalBounds();
    skBounds = s.getGlobalBounds();

    // Kolizje z mapą
    if (plBounds.intersects(skBounds)) {

        if (plBounds.top + plBounds.height / 2.0f < skBounds.top + skBounds.height / 2.0f) {

            sf::FloatRect docelowaPozycja = plBounds;
            docelowaPozycja.top = skBounds.top - plBounds.height;

            if (kolizje_z_tlem(docelowaPozycja, t)) {
                s.setPosition(s.getPosition().x, plBounds.top + plBounds.height);
                s.getVelocity().y = 0;
                if (hak.stan == StanHaka::PrzyciagaSkrzynke) {
                    hak.stan = StanHaka::Nieaktywny;
                    hak.trafionaSkrzynka = nullptr;
                }
            }
            else {
                g1.setPosition(g1.getPosition().x, skBounds.top - plBounds.height);
                g1.tG = true;
            }
        }
        else{
            sf::FloatRect docelowaDol = plBounds;
            docelowaDol.top = skBounds.top + skBounds.height;

            if (kolizje_z_tlem(docelowaDol, t)) {

                sf::FloatRect pozycjaNaSkrzynce = plBounds;
                pozycjaNaSkrzynce.top = skBounds.top - plBounds.height;

                if (!kolizje_z_tlem(pozycjaNaSkrzynce, t)) {
                    g1.setPosition(g1.getPosition().x, skBounds.top - plBounds.height);
                    g1.tG = true;
                }
                else{
                    s.setPosition(s.getPosition().x, plBounds.top - skBounds.height);
                    s.getVelocity().y = 0;
                    if (hak.stan == StanHaka::PrzyciagaSkrzynke) {
                        hak.stan = StanHaka::Nieaktywny;
                        hak.trafionaSkrzynka = nullptr;
                    }
                }
            }
            else{
                g1.setPosition(g1.getPosition().x, skBounds.top + skBounds.height);
            }
        }
        g1.velocity.y = 0;
    }

    kolizja_bloki_Y(g1, t);
    kolizja_b_gra(g1, window);
}

