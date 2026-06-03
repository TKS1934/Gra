#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>

class CustomShape : public sf::RectangleShape {
public:
    CustomShape(const sf::Vector2f& size, const sf::Vector3f& velocity, const sf::RenderWindow& window) :
        sf::RectangleShape(size), velocity(velocity), window(&window) {}
    bool isSelected() const {
        return selected;
    }
    void setSelected(bool selected) {
        this->selected = selected;
    }
    void animate(sf::Time elapsed) {
        if (isSelected())
            return;

        move(velocity.x*elapsed.asSeconds(), velocity.y*elapsed.asSeconds());
        rotate(velocity.z*elapsed.asSeconds());

        sf::FloatRect rectangle_bounds = getGlobalBounds();

        if (rectangle_bounds.left < 0)
            velocity.x = +std::abs(velocity.x);
        if (rectangle_bounds.left + rectangle_bounds.width > window->getSize().x)
            velocity.x = -std::abs(velocity.x);
        if (rectangle_bounds.top < 0)
            velocity.y = +std::abs(velocity.y);
        if (rectangle_bounds.top + rectangle_bounds.height > window->getSize().y)
            velocity.y = -std::abs(velocity.y);
    }
    void handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Up) {
                //std::cout << "Up" << std::endl;
            }
            if (event.key.code == sf::Keyboard::Down) {
                //std::cout << "Down" << std::endl;
            }
            if (event.key.code == sf::Keyboard::Left) {
                // std::cout << "Left" << std::endl;
            }
            if (event.key.code == sf::Keyboard::Right) {
                //std::cout << "Right" << std::endl;
            }
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            if(event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
                if (getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
                    setSelected(!isSelected());
            }
        }
    }

private:
    sf::Vector3f velocity;
    const sf::RenderWindow* window;
    bool selected = false;
};

int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // create some shapes
    sf::CircleShape circle(100.0);
    circle.setPosition(100.0, 300.0);
    circle.setFillColor(sf::Color(100, 250, 50));

    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(0.0, 0.0));
    triangle.setPoint(1, sf::Vector2f(0.0, 100.0));
    triangle.setPoint(2, sf::Vector2f(140.0, 40.0));
    triangle.setOutlineColor(sf::Color::Red);
    triangle.setOutlineThickness(5);
    triangle.setPosition(600.0, 100.0);

    std::vector<sf::Shape*> shapes;
    shapes.push_back(&circle);
    shapes.push_back(&triangle);

    std::vector<std::unique_ptr<CustomShape>> rectangles;
    for (int i = 0; i < 10; ++i) {
        rectangles.emplace_back(std::make_unique<CustomShape>(
            sf::Vector2f(120.0, 60.0), sf::Vector3f(-200+rand()%400, -200+rand()%400, -20+rand()%40), window));
        rectangles.back()->setPosition(500.0, 400.0);
        rectangles.back()->setFillColor(sf::Color(rand()%255, rand()%250, rand()%250));
        shapes.push_back(rectangles.back().get());
    }

    sf::Clock clock;
    bool stop = false;
    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space) {
                    std::cout << "Space" << std::endl;
                    stop = !stop;
                }
            }
            for (auto& r: rectangles)
                r->handleEvent(event);
        }

        sf::Time elapsed = clock.restart();

        if (!stop)
            for (auto& r: rectangles)
                r->animate(elapsed);

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        for (auto& s: shapes)
            window.draw(*s);

        // end the current frame
        window.display();
    }

    return 0;
}
