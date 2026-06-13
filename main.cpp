#include <bits/stdc++.h>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "blok.h"
#include "skrzynka1.h"
#include "hak.h"
#include "gracz.h"
#include "funkcje.h"
void czy_wygrana(skrzynka& s, vector<vector<blok>>& t,bool& cW){
    for (auto &pom : t) {
        for (auto &r : pom) {
            if (s.getGlobalBounds().intersects(r.getGlobalBounds()) && r.Ret_ID()==5){
                cW=1;
                cout<<"gowno";
            }
        }
    }
}

enum class StanGry { Menu, Rozgrywka, Wygrana, Poziomy };


int main(){

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "My window",sf::Style::Fullscreen); //,sf::Style::Fullscreen
    const float grawitacja=400;
    bool czyWygrane=0;

    auto t=generowanie_planszy_z_pliku("mapa2.txt",window,grawitacja);


    gracz *gracz1;


    skrzynka *skrzynka1;


    Hak hak;
    hak.silaPrzyciagania=grawitacja*(10/3);

    sf::Clock clock;
    sf::Font font;

    if (!font.loadFromFile("arial.ttf")) {
        cout << "Nie udalo sie zaladowac czcionki!" << endl;
    }
    sf::Text tekstGratulacje;
    tekstGratulacje.setFont(font);

    tekstGratulacje.setString("GRATULACJE! WYGRALES!");
    tekstGratulacje.setCharacterSize(100);
    tekstGratulacje.setFillColor(sf::Color::Yellow);

    sf::FloatRect textRect = tekstGratulacje.getLocalBounds();
    tekstGratulacje.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    tekstGratulacje.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));


    StanGry aktualnyStan = StanGry::Menu;
    int wybranaOpcjaMenu = 0; // 0 = Start, 1 = Wyjście

    sf::Text tekstTytul;
    tekstTytul.setFont(font);
    tekstTytul.setString("Nazwa Gry");
    tekstTytul.setCharacterSize(120);
    tekstTytul.setFillColor(sf::Color::White);
    tekstTytul.setPosition(window.getSize().x / 2.f - tekstTytul.getGlobalBounds().width / 2.f, 300.f);

    sf::Text tekstStart;
    tekstStart.setFont(font);
    tekstStart.setString("1. Poziomy");
    tekstStart.setCharacterSize(80);
    tekstStart.setPosition(window.getSize().x / 2.f - tekstStart.getGlobalBounds().width / 2.f, 600.f);

    sf::Text tekstWyjscie;
    tekstWyjscie.setFont(font);
    tekstWyjscie.setString("2. Wyjscie");
    tekstWyjscie.setCharacterSize(80);
    tekstWyjscie.setPosition(window.getSize().x / 2.f - tekstWyjscie.getGlobalBounds().width / 2.f, 750.f);


    sf::Text tekstPoziom1("1. Poziom 1", font, 80);
    sf::Text tekstPoziom2("2. Poziom 2", font, 80);
    sf::Text tekstPoziom3("3. Poziom 3", font, 80);
    sf::Text tekstPowrot("Powrot", font, 60);

    tekstPoziom1.setPosition(window.getSize().x / 2.f - tekstPoziom1.getGlobalBounds().width / 2.f, 500.f);
    tekstPoziom2.setPosition(window.getSize().x / 2.f - tekstPoziom2.getGlobalBounds().width / 2.f, 650.f);
    tekstPoziom3.setPosition(window.getSize().x / 2.f - tekstPoziom3.getGlobalBounds().width / 2.f, 800.f);
    tekstPowrot.setPosition(window.getSize().x / 2.f - tekstPowrot.getGlobalBounds().width / 2.f, 1000.f);
    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyReleased)
                if (event.key.code == sf::Keyboard::Enter)
                    window.close();



            if (aktualnyStan == StanGry::Menu) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(mousePos.x, mousePos.y);
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                    if (tekstStart.getGlobalBounds().contains(mousePosF)) {
                        czyWygrane = false;
                        aktualnyStan = StanGry::Poziomy;
                    }
                    else if(tekstWyjscie.getGlobalBounds().contains(mousePosF)){
                        window.close();
                    }
                }
            }


            else if(aktualnyStan == StanGry::Poziomy){
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(mousePos.x, mousePos.y);
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                    if (tekstPowrot.getGlobalBounds().contains(mousePosF)) {
                        czyWygrane = false;
                        aktualnyStan = StanGry::Menu;
                    }
                    else if(tekstPoziom1.getGlobalBounds().contains(mousePosF)){
                        t=generowanie_planszy_z_pliku("mapa1.txt",window,grawitacja);
                        aktualnyStan = StanGry::Rozgrywka;
                        gracz1= new gracz({8,14},{180,900},{0,0},window,grawitacja,0);
                        skrzynka1 = new skrzynka({float(window.getSize().x)*4/3/97, float(window.getSize().y)*4/3/60}, {200, 900}, {0,0}, window, grawitacja, 4);
                    }
                    else if(tekstPoziom2.getGlobalBounds().contains(mousePosF)){
                        aktualnyStan = StanGry::Rozgrywka;
                        t=generowanie_planszy_z_pliku("mapa2.txt",window,grawitacja);
                        gracz1= new gracz({8,14},{80,20},{0,0},window,grawitacja,0);
                        skrzynka1 = new skrzynka({float(window.getSize().x)*4/3/97, float(window.getSize().y)*4/3/60}, {40, 150}, {0,0}, window, grawitacja, 4);
                    }
                    else if(tekstPoziom3.getGlobalBounds().contains(mousePosF)){
                        aktualnyStan = StanGry::Rozgrywka;
                        t=generowanie_planszy_z_pliku("mapa3.txt",window,grawitacja);
                          gracz1= new gracz({8,14},{1800,700},{0,0},window,grawitacja,0);
                        skrzynka1 = new skrzynka({float(window.getSize().x)*4/3/97, float(window.getSize().y)*4/3/60}, {1000, 1000}, {0,0}, window, grawitacja, 4);
                    }
                }

            }


            else if(aktualnyStan == StanGry::Rozgrywka){
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (hak.stan == StanHaka::Nieaktywny) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePosF(mousePos.x, mousePos.y);

                        hak.pozycja = gracz1->getPosition();
                        sf::Vector2f kierunek = mousePosF - gracz1->getPosition();
                        float dlugosc = std::hypot(kierunek.x, kierunek.y);

                        if (dlugosc > 0.f) {
                            hak.kierunek = kierunek / dlugosc;
                            hak.stan = StanHaka::Wystrzelony;
                        }
                    } else {
                        if (hak.stan == StanHaka::PrzyciagaGracza && gracz1->getVelocity().y < 0) {
                            gracz1->getVelocity().y = -220.0f;

                        }

                        hak.stan = StanHaka::Nieaktywny;
                        hak.trafionaSkrzynka = nullptr;
                    }
                }
            }

        }


        sf::Time elapsed = clock.restart();
        if (aktualnyStan == StanGry::Menu) {
            window.clear();
            window.draw(tekstTytul);
            window.draw(tekstStart);
            window.draw(tekstWyjscie);
        }
        else if(aktualnyStan == StanGry::Poziomy){
            window.clear();
            window.draw(tekstTytul);
            window.draw(tekstPoziom1);
            window.draw(tekstPoziom2);
            window.draw(tekstPoziom3);
            window.draw(tekstPowrot);
        }
        else if(aktualnyStan == StanGry::Rozgrywka){
            if(czyWygrane){
                window.clear();
                window.draw(tekstGratulacje);
                window.draw(tekstPowrot);
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePosF(mousePos.x, mousePos.y);
                    if (tekstPowrot.getGlobalBounds().contains(mousePosF)) {
                        czyWygrane = false;
                        aktualnyStan = StanGry::Menu;
                    }
                }
            }
            else{
                gracz1->Sterowanie();
                ruch(elapsed, *gracz1, *skrzynka1, t, grawitacja, window,hak);
                czy_wygrana(*skrzynka1,t,czyWygrane);
                // clear the window with black color
                window.clear(sf::Color(128,128,128));

                // draw everything here...
                for(auto pom:t){
                    for(auto r:pom){
                        window.draw(r);
                    }
                }
                window.draw(*skrzynka1);
                window.draw(*gracz1);
                rysuj_hak(window,*gracz1,hak);
            }
        }
    window.display();
    }

}