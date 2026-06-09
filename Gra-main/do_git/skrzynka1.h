#include<bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class skrzynka : public byt {
private:
    // 'inline static' gwarantuje, że tekstura żyje w bezpiecznym miejscu globalnym
    inline static sf::Texture teksturaSkrzynki;
    inline static bool czyZaladowano = false;

public:
    skrzynka(const sf::Vector2f& size1, const sf::Vector2f& position1, const sf::Vector2f& velocity1, const sf::RenderWindow& window, float graw, int ID) :
        byt(size1, position1, velocity1, window, graw, ID) {

        isStatic = 0; // Skrzynka podlega fizyce

        // Ładujemy plik z dysku tylko RAZ, przy pierwszej stworzonej skrzynce
        if (!czyZaladowano) {
            if (teksturaSkrzynki.loadFromFile("skrzynka.png")) {
                czyZaladowano = true;
            } else {
                std::cout << "Blad: Nie udalo sie wczytac tekstury skrzynka.png!" << std::endl;
            }
        }

        // Jeśli tekstura istnieje, przypisujemy jej bezpieczny adres
        if (czyZaladowano) {
            this->setTexture(&teksturaSkrzynki);
            this->setFillColor(sf::Color::White);
        }
    }
    void changeVelo (sf::Vector2f vel){
        this->velocity=vel;
    }
    //friend void ruch(sf::Time elapsed, gracz& g1, skrzynka& s, vector<vector<blok>>& t, const float gravity, const sf::RenderWindow& window,Hak& hak);
};