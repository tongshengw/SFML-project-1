#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Sphere {
    float x;
    float y;
    float z;
    sf::Color color;
};

Sphere & sphere_init(Sphere &s, float x, float y, float z) {
    
    sf::Color color;
    color.r = round(rand()*255);
    color.g = round(rand()*255);
    color.b = round(rand()*255);
    s.color = color;

    s.x = x;
    s.y = y;
    s.z = z;
    return s;
}

// s is the input vector to be projected
sf::Vector2f project(sf::Vector3f cameraPosition, sf::Vector3f cameraRotation,
                     sf::Vector3f s, float focalLength, int winSizeX, int winSizeY) {
  sf::Vector3f cameraRelPos;

  float X = s.x - cameraPosition.x;
  float Y = s.y - cameraPosition.y;
  float Z = s.z - cameraPosition.z;

  cameraRelPos.x = cos(cameraRotation.y) *
                       (sin(cameraRotation.z) * Y + cos(cameraRotation.z) * X) -
                   sin(cameraRotation.y) * Z;
  cameraRelPos.y = sin(cameraRotation.x) *
                       (cos(cameraRotation.y) * Z +
                        sin(cameraRotation.y) * (sin(cameraRotation.z) * Y +
                                                 cos(cameraRotation.z) * X)) +
                   cos(cameraRotation.x) *
                       (cos(cameraRotation.z) * Y - sin(cameraRotation.z) * X);
  cameraRelPos.z = cos(cameraRotation.x) *
                       (cos(cameraRotation.y) * Z +
                        sin(cameraRotation.y) * (sin(cameraRotation.z) * Y +
                                                 cos(cameraRotation.z) * X)) -
                   sin(cameraRotation.x) *
                       (cos(cameraRotation.z) * Y - sin(cameraRotation.z) * X);

  sf::Vector2f projected;

  projected.x = (focalLength / cameraRelPos.z) * cameraRelPos.x + winSizeX / 2;
  projected.y = (focalLength / cameraRelPos.z) * cameraRelPos.y + winSizeY / 2;

  return projected;
}

sf::Clock deltaTimeClock;
sf::Time deltaTime;


int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");

    vector<Sphere> spheres;
    Sphere s1;
    Sphere s2;
    Sphere s3;
    sphere_init(s1, 0, 0, 0);
    s1.color=sf::Color::Green;
    spheres.push_back(s1);
    sphere_init(s2, 60, 0, 0);
    s2.color = sf::Color::Blue;
    spheres.push_back(s2);
    sphere_init(s3, 0, 60, 0);
    s3.color = sf::Color::Red;
    spheres.push_back(s3);
    

    sf::Vector3f cameraPosition(300, 0, 300);
    sf::Vector3f cameraRotation(0, 0, 0);

    float focalLength = 100;
    float sphereRadius = 30;

    float cameraPolarAngle = 0;
    float cameraPolarDisplacement = 50;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        deltaTime = deltaTimeClock.restart();

        // cameraPolarAngle += deltaTime.asSeconds();
        // cameraPosition.x = cameraPolarDisplacement * cos(cameraPolarAngle);
        // cameraPosition.y = cameraPolarDisplacement * sin(cameraPolarAngle);

        // cameraRotation.x = cameraPolarAngle;

        window.clear();

        for (Sphere s : spheres) {
            sf::Vector3f sphereEdgeVector;
            sf::Vector3f sphereVector(s.x, s.y, s.z);
            sphereEdgeVector = sf::Vector3f(s.x + sphereRadius, s.y, s.z);

            int winSizeX = window.getSize().x;
            int winSizeY = window.getSize().y;

            sf::Vector2f projectedSphereVector = project(cameraPosition, cameraRotation, sphereVector, focalLength, winSizeX, winSizeY);
            sf::Vector2f projectedSphereEdgeVector = project(cameraPosition, cameraRotation, sphereEdgeVector, focalLength, winSizeX, winSizeY);

            sf::Vector2f diff = projectedSphereEdgeVector - projectedSphereVector;

            float projectedRadius = sqrt(diff.x*diff.x + diff.y*diff.y);

            window.setView(sf::View(sf::FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y)));

            sf::CircleShape circle;
            circle.setRadius(projectedRadius);

            circle.setFillColor(s.color);
            circle.setOrigin(projectedRadius, projectedRadius);
            circle.setPosition(projectedSphereVector.x, projectedSphereVector.y);
            window.draw(circle);
        }
        
        window.display();
    }

    return 0;
}