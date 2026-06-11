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
void kolizja_skrzynka_X(gracz& g1, skrzynka& s) {
    if (g1.getGlobalBounds().intersects(s.getGlobalBounds())) {
        if (g1.getVelocity().x > 0) { // Ruch w prawo -> uderzenie w lewy bok skrzynki
            g1.setPosition(s.getPosition().x - g1.getGlobalBounds().width, g1.getPosition().y);
        }
        else if (g1.getVelocity().x < 0) { // Ruch w lewo -> uderzenie w prawy bok skrzynki
            g1.setPosition(s.getPosition().x + s.getGlobalBounds().width, g1.getPosition().y);
        }
        g1.getVelocity().x = 0; // Zatrzymujemy gracza w poziomie
    }
}

void kolizja_skrzynka_Y(gracz& g1, skrzynka& s) {
    if (g1.getGlobalBounds().intersects(s.getGlobalBounds())) {
        if (g1.getVelocity().y > 0) { //  lądowanie na skrzynce
            g1.setPosition(g1.getPosition().x, s.getPosition().y - g1.getGlobalBounds().height);
            g1.tG = true; //  może skakać
        }
        else if (g1.getVelocity().y < 0) { // Skok w górę -> uderzenie w skrzynkę od spodu
            g1.setPosition(g1.getPosition().x, s.getPosition().y + s.getGlobalBounds().height);
        }
        g1.getVelocity().y = 0; // Zerujemy prędkość pionową
    }
}


void aktualizuj_hak(Hak& hak, gracz& g1, vector<vector<blok>>& t, float dt,skrzynka& s) {
    if (hak.stan == StanHaka::Nieaktywny) return;

    // 1. LOT HAKA
    if (hak.stan == StanHaka::Wystrzelony) {
        hak.pozycja += hak.kierunek * hak.predkosc * dt;
        sf::FloatRect boundsHaka(hak.pozycja.x - 2, hak.pozycja.y - 2, 4, 4);
        // Sprawdzamy kolizję haka z blokami
        for (auto &pom : t) {
            for (auto &r : pom) {
                if (!r.fispfp()){
                     // Mały punkt kolizji haka

                    if (boundsHaka.intersects(r.getGlobalBounds())) {
                        if (!r.fispfp()) {
                            // Trafiliśmy w ścianę -> gracz będzie się przyciągał
                            hak.stan = StanHaka::PrzyciagaGracza;
                        }
                        /*else if (r.ids() == 4) {
                            // Trafiliśmy w skrzynkę -> skrzynka będzie przyciągana do gracza
                            hak.stan = StanHaka::PrzyciagaSkrzynke;
                            hak.trafionaSkrzynka = &r;
                        }*/
                        return;
                    }
                }
            }
        }
        if (boundsHaka.intersects(s.getGlobalBounds())){
            hak.stan = StanHaka::PrzyciagaSkrzynke;
            hak.trafionaSkrzynka = &s;
            std::cout<<"larwa1 kurwa1\n";
        }
        // Zabezpieczenie: jeśli hak poleci za daleko
        float dx = hak.pozycja.x - g1.getPosition().x;
        float dy = hak.pozycja.y - g1.getPosition().y;
        if (std::hypot(dx, dy) > 600.f) {
            hak.stan = StanHaka::Nieaktywny;
        }
    }

    // 2. TRYB A: Gracz przyciąga się do haka (ściany)
    if (hak.stan == StanHaka::PrzyciagaGracza) {
        sf::Vector2f cel = hak.pozycja;
        sf::Vector2f start = g1.getPosition();

        float dx = cel.x - start.x;
        float dy = cel.y - start.y;
        float odleglosc = std::hypot(dx, dy);

        if (odleglosc > 15.f) { // Jeśli gracz jeszcze nie dotarł do celu
            // Wyznaczamy wektor prędkości w stronę haka
            g1.velocity.x = (dx / odleglosc) * hak.silaPrzyciagania;
            g1.velocity.y = (dy / odleglosc) * hak.silaPrzyciagania;

            // Wyłączamy grawitację na czas przyciągania w funkcji ruch()!
        } else {
            // Gracz dotarł do miejsca zakotwiczenia haka
            hak.stan = StanHaka::Nieaktywny;
            g1.velocity = sf::Vector2f(0, 0);
        }
    }

    // 3. TRYB B: Skrzynka jest przyciągana do gracza
    if (hak.stan == StanHaka::PrzyciagaSkrzynke) {
        if (hak.trafionaSkrzynka == nullptr) {
            hak.stan = StanHaka::Nieaktywny;
            return;
        }

        sf::Vector2f cel = g1.getPosition(); // Skrzynka leci do gracza
        sf::Vector2f start = hak.trafionaSkrzynka->getPosition();

        float dx = cel.x - start.x;
        float dy = cel.y - start.y;
        float odleglosc = std::hypot(dx, dy);

        if (odleglosc > 30.f) {
            sf::Vector2f przesuniecieSkrzynki;
            przesuniecieSkrzynki.x = (dx / odleglosc) * hak.silaPrzyciagania * dt;
            przesuniecieSkrzynki.y = (dy / odleglosc) * hak.silaPrzyciagania * dt;

            // Przesuwamy skrzynkę (musisz mieć metodę move lub setPosition w klasie blok)
            hak.trafionaSkrzynka->move(przesuniecieSkrzynki.x, przesuniecieSkrzynki.y);

            // Aktualizujemy pozycję haka, żeby "jechał" razem ze skrzynką
            hak.pozycja = hak.trafionaSkrzynka->getPosition();
        } else {
            // Skrzynka doleciała do gracza
            hak.stan = StanHaka::Nieaktywny;
            hak.trafionaSkrzynka = nullptr;
        }
    }
}


void rysuj_hak(sf::RenderWindow& window, const gracz& g1, const Hak& hak) {
    if (hak.stan != StanHaka::Nieaktywny) {
        sf::VertexArray lina(sf::Lines, 2);

        // Punkt A - Gracz
        lina[0].position = g1.getPosition();
        lina[0].color = sf::Color(100, 100, 100); // Szary kolor liny

        // Punkt B - Hak / Skrzynka
        lina[1].position = hak.pozycja;
        lina[1].color = sf::Color(150, 150, 150);

        window.draw(lina);

        // Opcjonalnie: możesz narysować mały kwadracik w miejscu hak.pozycja
    }
}







void ruch(sf::Time elapsed, gracz& g1, skrzynka& s, vector<vector<blok>>& t, const float gravity, const sf::RenderWindow& window,Hak& hak) {
    float dt = elapsed.asSeconds();

    aktualizuj_hak(hak,g1,t,dt,s);
    if (hak.stan != StanHaka::PrzyciagaGracza && hak.stan != StanHaka::PrzyciagaSkrzynke) {
        g1.velocity.y += gravity * dt;
    } else {
        sf::Vector2f kierunek = hak.pozycja - g1.getPosition();

        // 2. Normalizujemy wektor (dzielimy przez aktualną odległość)
        // Dzięki temu wektor ma długość 1 i wskazuje tylko kierunek
        float dlugosc = std::hypot(kierunek.x, kierunek.y);//liczy odleglosc z pitagorasa

        if (hak.stan != StanHaka::PrzyciagaGracza){
            sf::Vector2f znormalizowanyKierunek = kierunek / (dlugosc);
            g1.velocity = znormalizowanyKierunek * hak.silaPrzyciagania;
        }
        else{
            sf::Vector2f znormalizowanyKierunek = kierunek / (-dlugosc);
            s.changeVelo( znormalizowanyKierunek * hak.silaPrzyciagania);
        }



    }

    // Grawitacja
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