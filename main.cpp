#include <bits/stdc++.h>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "blok.h"
#include "skrzynka1.h"
#include "hak.h"
#include "gracz.h"
#include "funkcje.h"


int main(){
    sf::RenderWindow window(sf::VideoMode(2800, 1600), "My window",sf::Style::Fullscreen);
    const float grawitacja=400;

    auto t=generowanie_planszy_z_pliku("mapa.txt",window,grawitacja);


    gracz *gracz1;
    gracz1= new gracz({10,20},{250,250},{0,0},window,grawitacja,0);

    skrzynka *skrzynka1;
    skrzynka1 = new skrzynka({40, 40}, {2000, 250}, {0,0}, window, grawitacja, 4);

    Hak hak;
    hak.silaPrzyciagania=grawitacja*(10/3);

    sf::Clock clock;

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
                        gracz1->getVelocity().y = -400.0f;
                    }

                    hak.stan = StanHaka::Nieaktywny;
                    hak.trafionaSkrzynka = nullptr;
                }
            }
        }



        sf::Time elapsed = clock.restart();

        gracz1->ruchom(event);
        gracz1->Sterowanie();
        ruch(elapsed, *gracz1, *skrzynka1, t, grawitacja, window,hak);

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
        // end the current frame
        window.display();
    }

}