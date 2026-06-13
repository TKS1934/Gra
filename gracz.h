#include<bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class skrzynka;

class gracz: public byt{
protected:
    bool tG=1;
    bool dslw=0;//dotyka sciany lewa prawa
    bool dsgd=0;//dotyka sciany gora dol
    bool skok = false;
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

    void Sterowanie(){
        // Ruch poziomy A D
        velocity.x = 0.0f; // Reset prędkości

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            velocity.x = 230.0f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            velocity.x = -230.0f;
        }

        //  skok W
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){

            if (tG && !skok){
                velocity.y = (-100- gravity / 2);
                tG = false;
                skok = true;
            }
        }
        else {
            skok = false; // po puszczeniu W możemy znowu skakać

        }}


    friend void ruch(sf::Time elapsed, gracz& g1, skrzynka& s, std::vector<std::vector<blok>>& t, const float gravity, const sf::RenderWindow& window,Hak& hak);
    friend void kolizja_b_gra(gracz& bi, const sf::RenderWindow& window);
    friend void kolizja_bloki_Y(gracz& g1, std::vector<std::vector<blok>>& t);
    friend void kolizja_bloki_X(gracz& g1, std::vector<std::vector<blok>>& t);
    friend void kolizja_skrzynka_Y(gracz& g1, skrzynka& s);
    friend void aktualizuj_hak(Hak& hak, gracz& g1, std::vector<std::vector<blok>>& t, float dt,skrzynka& s);
};
