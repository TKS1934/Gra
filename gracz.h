#include<bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
class skrzynka;

class gracz: public byt{
protected:
    bool tG=1;
    bool dslw=0;//dotyka sciany lewa prawa
    bool dsgd=0;//dotyka sciany gora dol
public:
    bool fdsgd(){
        return dsgd;
    }
    bool fdslw(){
        return dslw;
    }
    void f1dsgd(){
        dsgd=!dsgd;
    }
    void f1dslw(){
        dslw=!dslw;
    }
    gracz(const sf::Vector2f& size1,const sf::Vector2f& position1,const sf::Vector2f& velocity1, const sf::RenderWindow& window,float graw,int ID) :
        byt(size1, position1, velocity1, window,graw,ID){
        velocity.x=0;
        velocity.y=0;
    }

    void ruchom(sf::Event event){
        //std::cout<<tG<<"\n";
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up && tG!=0){//skok
                velocity.y=(-200-gravity/2);
                tG=0;
            }
            if (event.key.code == sf::Keyboard::Right){
                velocity.x=200.0f;
            }
            if (event.key.code == sf::Keyboard::Left){
                velocity.x=(-200.0f);
            }
        }
        else if(event.type == sf::Event::KeyReleased){
            if (event.key.code == sf::Keyboard::Right){
                velocity.x=0.0f;
            }
            if (event.key.code == sf::Keyboard::Left){
                velocity.x=0.0f;
            }
        }
        std::cout<<getPosition().x<<" "<<getPosition().y<<"\n \n";




    }


    friend void ruch(sf::Time elapsed, gracz& g1, skrzynka& s, std::vector<std::vector<blok>>& t, const float gravity, const sf::RenderWindow& window);
    friend void kolizja_b_gra(gracz& bi, const sf::RenderWindow& window);
    friend void kolizja_bloki_Y(gracz& g1, std::vector<std::vector<blok>>& t);
    friend void kolizja_bloki_X(gracz& g1, std::vector<std::vector<blok>>& t);
    friend void kolizja_skrzynka_Y(gracz& g1, skrzynka& s);
};
