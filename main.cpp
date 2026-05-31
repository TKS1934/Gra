#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "blok.h"
#include "funkcje.h"

using namespace std;




int main(){
    sf::RenderWindow window(sf::VideoMode(800, 800), "My window");
    const float grawitacja=9.81;
    //const sf::Vector2f size1={20,20};
    int w_p_x=20; //wymiar planszy x
    int w_p_y=20; //analogicznie y

    //blok *obj;
   // obj = new blok(size1,{200,550},{0,-10},window,grawitacja,1);

    auto t=generowanie_planszy(w_p_x,w_p_y,window,grawitacja);


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
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space) {
                    std::cout << "Space" << std::endl;
                    stop = !stop;
                }
            }
        }

        sf::Time elapsed = clock.restart();

        if (!stop)
            for(auto pom:t){
                for(auto r:pom){
                    r.ruch(elapsed);
                }
            }


        // clear the window with black color
        window.clear(sf::Color::White);

        // draw everything here...
        for(auto pom:t){
            for(auto r:pom){
                window.draw(r);
            }
        }


        // end the current frame
        window.display();
    }
}