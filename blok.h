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
    int fispfp(){
        return isPFP;}

    bool fispfb() {
        return isPFB;
    }
    int Ret_ID(){
        return this->id;
    }

    blok(const sf::Vector2f& size1,const sf::Vector2f& position1, const sf::Vector2f& velocity1, const sf::RenderWindow& window,float graw,int ID, const sf::Texture* tex=nullptr) :
        byt(size1, position1, velocity1, window,graw,ID,tex){
        isPFP=(ID==3 || ID==5);
        isPFB=(ID==2 || ID==5);
        isStatic=1;


        if(ID==0){
            this->setFillColor(sf::Color::Transparent);
            isPFP = 1;
            isPFB = 1;
        }
        if(ID==5){
            this->setFillColor(sf::Color::Magenta);
        }

    }

};



