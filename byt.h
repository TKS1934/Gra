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

    void ruch(sf::Time elapsed){
        if(isStatic){
            return;
        }
        float dt = elapsed.asSeconds();
        velocity.y += gravity * dt;
        move(velocity.x * dt, velocity.y * dt);
        sf::FloatRect b = getGlobalBounds();
        if (b.left < 0)
        {
            setPosition(0, getPosition().y);
            velocity.x = 0;
        }
        if (b.left + b.width > window->getSize().x)
        {
            setPosition(window->getSize().x - b.width, getPosition().y);
            velocity.x = 0;
        }
        if (b.top < 0)
        {
            setPosition(getPosition().x, 0);
            velocity.y = 0;
        }
        if (b.top + b.height > window->getSize().y)
        {
            setPosition(getPosition().x,window->getSize().y - b.height);

            velocity.y = 0;
        }
    }

};



