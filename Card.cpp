#include "Card.hpp"
#include <SFML/Graphics.hpp>

Card::Card(const std::string &name, int cost, float xpos, float ypos)
    : name(name), cost(cost), backgroundShape(sf::Vector2f(100, 50)) {
        backgroundShape.setPosition(sf::Vector2f(xpos, ypos));
    }

void Card::render(sf::RenderWindow &window) const {
    window.draw(backgroundShape);
}

int Card::get_cost() const {
    return cost;
}

std::string Card::get_name() const {
    return name;
}