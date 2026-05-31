#include<bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "byt.h"

class blok: public byt
{
protected:
    bool isPFP; //is possible to go trough for player
    bool isPFB; //is possible to go trough for block
public:

    blok(const sf::Vector2f& size1,const sf::Vector2f& position1, const sf::Vector2f& velocity1, const sf::RenderWindow& window,float graw,int ID) :
        byt(size1, position1, velocity1, window,graw,ID){
        isPFP=(ID==2);
        isPFB=(ID==3);
        isStatic=1;
        switch(ID){
            case 1:
                this->setFillColor(sf::Color::Black);
                break;
            case 2:
                this->setFillColor(sf::Color(60,255,60,100));
                break;
            case 3:
                this->setFillColor(sf::Color::Magenta);
                break;
            default:
                this->setFillColor(sf::Color::Transparent);
                isPFP=1;
                isPFB=1;
            }

    }

};



