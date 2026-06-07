#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "gracz.h"
//#include "blok.h"
#include "funkcje.h"


using namespace std;




int main(){
    sf::RenderWindow window(sf::VideoMode(800, 800), "My window");
    const float grawitacja=1000;
    int l=0;
    //const sf::Vector2f size1={20,20};
    /*
    int w_p_x=20; //wymiar planszy x
    int w_p_y=20; //analogicznie y
    */
    //blok *obj;
   // obj = new blok(size1,{200,550},{0,-10},window,grawitacja,1);

    auto t=generowanie_planszy_z_pliku("mapa1.txt",window,grawitacja);
    gracz *gracz1;
    gracz1= new gracz({10,20},{250,250},{0,0},window,grawitacja,0);

    sf::Clock clock;
   // bool stop = false;
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
        gracz1->ruchom(event);
        ruch(elapsed,*gracz1,t,grawitacja,window);





        // clear the window with black color
        window.clear(sf::Color::White);

        // draw everything here...
        for(const auto &pom:t){
            for(const auto &r:pom){
                window.draw(r);
            }
        }
        window.draw(*gracz1);


        // end the current frame
        window.display();
    }
}