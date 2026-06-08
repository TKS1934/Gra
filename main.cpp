#include <bits/stdc++.h>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "blok.h"
#include "gracz.h"
#include "funkcje.h"


int main(){
    sf::RenderWindow window(sf::VideoMode(2800, 1600), "My window");
    const float grawitacja=500;
    //const sf::Vector2f size1={20,20};
    int w_p_x=20; //wymiar planszy x
    int w_p_y=20; //analogicznie y

    //blok *obj;
   // obj = new blok(size1,{200,550},{0,-10},window,grawitacja,1);

   // auto t=generowanie_planszy(w_p_x,w_p_y,window,grawitacja);
    auto t=generowanie_planszy_z_pliku("mapa2.txt",window,grawitacja);


    gracz *gracz1;
    gracz1= new gracz({10,20},{250,250},{0,0},window,grawitacja,0);



    sf::Clock clock;
    bool stop = false;
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
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space) {
                    std::cout << "Space" << std::endl;
                    stop = !stop;
                }
            }
        }


        sf::Time elapsed = clock.restart();

        gracz1->ruchom(event);
        ruch(elapsed,*gracz1,t,grawitacja,window);
//a
        // clear the window with black color
        window.clear(sf::Color(128,128,128));

        // draw everything here...
        for(auto pom:t){
            for(auto r:pom){
                window.draw(r);
            }
        }

    window.draw(*gracz1);
        // end the current frame
        window.display();
    }
}