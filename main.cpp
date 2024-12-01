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
    color.r = rand() % 100;
    color.g = rand() % 100;
    color.b = rand() % 100;
    s.color = color;

    s.x = x;
    s.y = y;
    s.z = z;
    return s;
}

// s is the input vector to be projected
float project(sf::Vector2f &projected, sf::Vector3f cameraPosition, sf::Vector3f cameraRotation,
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

  if (cameraRelPos.z <= 50) {
    return -1;  // Return -1 instead of 0 to indicate invalid projection
  }

  float scaleFactor = focalLength / cameraRelPos.z;
  projected.x = (scaleFactor * cameraRelPos.x) + winSizeX / 2;
  projected.y = (scaleFactor * cameraRelPos.y) + winSizeY / 2;

  return scaleFactor;
}

sf::Clock deltaTimeClock;
sf::Time deltaTime;


int main()
{
    srand(time(nullptr));  // Initialize random seed
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "A below mediocre 3d Renderer");

    vector<Sphere> spheres;
    Sphere s1;
    Sphere s2;
    Sphere s3;
    Sphere s4;
    sphere_init(s1, 0, 0, 0);
    s1.color=sf::Color::White;
    spheres.push_back(s1);
    sphere_init(s2, 60, 0, 0);
    s2.color = sf::Color::Blue;
    spheres.push_back(s2);
    sphere_init(s3, 0, 60, 0);
    s3.color = sf::Color::Red;
    spheres.push_back(s3);
    sphere_init(s4, 0, 0, 60);
    s4.color = sf::Color::Cyan;
    spheres.push_back(s4);

    for (int i = 0; i < 300; i++) {
        Sphere s;
        sphere_init(s, (rand() % 1000) - 150, (rand() % 1000) - 150, (rand() % 1000) - 150);
        spheres.push_back(s);
    }

    
    // float i = -(3*3.14)/4;
    sf::Vector3f cameraPosition(0, 0, 0);
    sf::Vector3f cameraRotation(0, 0, 0);

    float focalLength = 100;
    float sphereRadius = 30;

    float dist = 0;

    float paramAngle = 0;

    while (window.isOpen())
    {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            cameraPosition.y -= deltaTime.asSeconds() * 100;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            cameraPosition.y += deltaTime.asSeconds() * 100;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            cameraPosition.x -= deltaTime.asSeconds() * 100;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            cameraPosition.x += deltaTime.asSeconds() * 100;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            cameraPosition.z += deltaTime.asSeconds() * 100;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            cameraPosition.z -= deltaTime.asSeconds() * 100;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        deltaTime = deltaTimeClock.restart();

        // dist += deltaTime.asSeconds();

        // cameraPosition.z = 300*sin(dist);

        // cameraRotation.x = sin(dist);

        sort(spheres.begin(), spheres.end(), [cameraPosition](Sphere a, Sphere b) {
            float aDist = sqrt(pow(a.x - cameraPosition.x, 2) + pow(a.y - cameraPosition.y, 2) + pow(a.z - cameraPosition.z, 2));
            float bDist = sqrt(pow(b.x - cameraPosition.x, 2) + pow(b.y - cameraPosition.y, 2) + pow(b.z - cameraPosition.z, 2));
            return aDist > bDist;
        });

        window.clear();

        for (Sphere s : spheres) {
            sf::Vector3f sphereVector(s.x, s.y, s.z);

            int winSizeX = window.getSize().x;
            int winSizeY = window.getSize().y;

            sf::Vector2f projectedSphereVector;
            float scaleFactor = project(projectedSphereVector, cameraPosition, cameraRotation, sphereVector, focalLength, winSizeX, winSizeY);

            if (scaleFactor > 0) {
                window.setView(sf::View(sf::FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y)));

                sf::CircleShape circle;
                circle.setRadius(scaleFactor * sphereRadius);

                circle.setFillColor(s.color);
                circle.setOrigin(scaleFactor * sphereRadius, scaleFactor * sphereRadius);
                circle.setPosition(projectedSphereVector.x, projectedSphereVector.y);
                window.draw(circle);
            }
        }
        
        window.display();
    }

    return 0;
}