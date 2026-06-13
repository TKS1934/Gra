#include<bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class skrzynka : public byt {
private:
    inline static sf::Texture teksturaSkrzynki;
    inline static bool czyZaladowano = false;

public:
    skrzynka(const sf::Vector2f& size1, const sf::Vector2f& position1, const sf::Vector2f& velocity1, const sf::RenderWindow& window, float graw, int ID) :
        byt(size1, position1, velocity1, window, graw, ID) {

        isStatic = 0;

        if (!czyZaladowano) {
            if (teksturaSkrzynki.loadFromFile("skrzynka.png")) {
                czyZaladowano = true;
            } else {
                std::cout << "Blad: Nie udalo sie wczytac tekstury skrzynka.png!" << std::endl;
            }
        }
        if (czyZaladowano) {
            this->setTexture(&teksturaSkrzynki);
            this->setFillColor(sf::Color::White);
        }
    }
};