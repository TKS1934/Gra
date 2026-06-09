#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class AnimatedSprite : public sf::Sprite {
public:
    AnimatedSprite(const std::string& filename, int animation_fps = 7) : animation_fps(animation_fps) {
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Could not load:" << filename << std::endl;
            return;
        }
        setTexture(texture);
    }
    void animate(const sf::Time& elapsed) {
        if (rectangles.empty())
            return;

        time += elapsed.asSeconds();
        if (time > 1./animation_fps) {
            time = 0;

            setTextureRect(rectangles[n]);
            if (++n >= rectangles.size())
                n = 0;
        }
    }
    void add_animation_frame(const sf::IntRect& rect) {
        rectangles.push_back(rect);
    }

private:
    sf::Texture texture;
    int animation_fps;
    float time = 0;
    int n=0;
    std::vector<sf::IntRect> rectangles;
};


int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");


    sf::Sprite sprite;
    sf::Texture texture;
    if (!texture.loadFromFile("grass.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }
    sprite.setTexture(texture);

    sf::Sprite wall;
    sf::Texture texture_wall;
    if(!texture_wall.loadFromFile("wall.png")) { return 1; }
    texture_wall.setRepeated(true);
    wall.setTexture(texture_wall);
    wall.setScale(0.3, 0.3);
    wall.setTextureRect(sf::IntRect(0, 0, 500, 500));
    wall.setPosition(300, 300);

    sf::Texture texture_guy;
    if(!texture_guy.loadFromFile("guy.png")) { return 1; }

    sf::Sprite guy;
    guy.setTexture(texture_guy);
    guy.setTextureRect(sf::IntRect(10, 20, 20, 15)); //left, top, width, height

    AnimatedSprite hero("character.png", 7);
    hero.add_animation_frame(sf::IntRect(0, 0, 37, 37)); // 1 frame of animation
    hero.add_animation_frame(sf::IntRect(50, 0, 37, 37)); // 1 frame of animation
    hero.add_animation_frame(sf::IntRect(100, 0, 37, 37)); // 1 frame of animation
    //hero.add_animation_frame(sf::IntRect(200, 0, 37, 37)); // 1 frame of animation
    //hero.add_animation_frame(sf::IntRect(250, 0, 37, 37)); // 2 frame
    //hero.add_animation_frame(sf::IntRect(300, 0, 37, 37)); // 3 frame
    //hero.add_animation_frame(sf::IntRect(350, 0, 37, 37)); // 4 frame
    hero.setPosition(400, 400);
    hero.setScale(-3.0, 3.0);

    std::vector<const sf::Drawable*> objects = {&sprite, &wall, &guy, &hero};

    sf::Clock clock;
    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time elapsed = clock.restart();
        hero.animate(elapsed);

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        for (auto &s: objects)
            window.draw(*s);

        // end the current frame
        window.display();
    }

    return 0;
}
