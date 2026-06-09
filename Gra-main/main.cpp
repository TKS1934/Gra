#include <bits/stdc++.h>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "byt.h"
#include "blok.h"
#include "skrzynka1.h"
#include "hak.h"
#include "gracz.h"
#include "funkcje.h"



int main(){
    sf::RenderWindow window(sf::VideoMode(1600, 900), "My window");
    const float grawitacja=2000;
    auto t=generowanie_planszy_z_pliku("mapa1.txt",window,grawitacja);
    gracz *gracz1;
    gracz1= new gracz({10,20},{250,250},{0,0},window,grawitacja,0);
    Hak hak;
    hak.silaPrzyciagania=grawitacja*(4/3);
    skrzynka *skrzynka1;
    skrzynka1 = new skrzynka({40, 40}, {250, 250}, {0,0}, window, grawitacja, 4);

    sf::Clock clock;
    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time elapsed = clock.restart();
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (hak.stan == StanHaka::Nieaktywny) {

                // Pobranie pozycji myszy względem okna
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(mousePos.x, mousePos.y);
                hak.pozycja = mousePosF; // Hak startuje od punktu zaczepienia
                // Obliczenie wektora kierunku
                sf::Vector2f kierunek = mousePosF - gracz1->getPosition();
                //cout<<"\n\n kierunek: "<<kierunek.x<<" "<<kierunek.y<<"\n";
                float dlugosc = std::hypot(kierunek.x, kierunek.y);

                hak.kierunek = kierunek / dlugosc; // Normalizacja
                hak.stan = StanHaka::Wystrzelony;
            }
            else{
                hak.stan=StanHaka::Nieaktywny;
            }
        }
        gracz1->ruchom(event);
        ruch(elapsed, *gracz1, *skrzynka1, t, grawitacja, window,hak);





        // clear the window with black color
        window.clear(sf::Color(128,128,128));
        // draw everything here...
        for(const auto &pom:t){
            for(const auto &r:pom){
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