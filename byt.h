#include<bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class byt: public sf::RectangleShape
{
protected:
    int id;
    bool isStatic;
    sf::Vector2f size;
    sf::Vector2f velocity;
    sf::Vector2f position;
    float gravity;
    const sf::RenderWindow* window;
    //bool isPFP; //is possible to go trough for player
    //bool isPFB; //is possible to go trough for block
public:
    byt(const sf::Vector2f& size,const sf::Vector2f& position, const sf::Vector2f& velocity, const sf::RenderWindow& window,float grawitacja, int ID) :
        sf::RectangleShape(size), velocity(velocity), window(&window)
    {this->position=position;
        gravity=grawitacja;
        id=ID;
        this->setPosition(position.x,position.y);
        this->setFillColor(sf::Color(ID*10+50, 100-ID*5, 255-ID*40));
    };
    bool FisStatic ()const
    {
        return isStatic;
    }

};



