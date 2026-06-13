#include<bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
enum class StanHaka { Nieaktywny, Wystrzelony, PrzyciagaGracza, PrzyciagaSkrzynke };

struct Hak {
    sf::Vector2f pozycja;
    sf::Vector2f kierunek;
    float predkosc = 1200.f; // Prędkość lotu samego haka
    float silaPrzyciagania = 200.f; // Prędkość przyciągania
    StanHaka stan = StanHaka::Nieaktywny;

    // Wskaźnik na skrzynkę jeśli została trafiona
    skrzynka* trafionaSkrzynka = nullptr;
};

