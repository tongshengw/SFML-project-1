#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <array>

using namespace std;

float pi = 3.1415925;

class Matrix33 {
    public:
        /*
        |a 0, d 3, g 6|
        |b 1, e 4, h 7|
        |c 2, f 5, i 8|
        */
        Matrix33(array<float, 9> data)
            : data(data) {}

        Matrix33 right_multiply(const Matrix33 & other) const {
            array<float, 9> output;
            output[0] = data[0] * other.data[0] + data[3] * other.data[1] + data[6] * other.data[2];
            output[1] = data[1] * other.data[0] + data[4] * other.data[1] + data[7] * other.data[2];
            output[2] = data[2] * other.data[0] + data[5] * other.data[1] + data[8] * other.data[2];
            output[3] = data[0] * other.data[3] + data[3] * other.data[4] + data[6] * other.data[5];
            output[4] = data[1] * other.data[3] + data[4] * other.data[4] + data[7] * other.data[5];
            output[5] = data[2] * other.data[3] + data[5] * other.data[4] + data[8] * other.data[5];
            output[6] = data[0] * other.data[6] + data[3] * other.data[7] + data[6] * other.data[8];
            output[7] = data[1] * other.data[6] + data[4] * other.data[7] + data[7] * other.data[8];
            output[8] = data[2] * other.data[6] + data[5] * other.data[7] + data[8] * other.data[8];

            return Matrix33(output);
        }

        sf::Vector3f vector_right_multiply(const sf::Vector3f &other) const {
            sf::Vector3f result;
            result.x = data[0] * other.x + data[3] * other.y + data[6] * other.z;
            result.y = data[1] * other.x + data[4] * other.y + data[7] * other.z;
            result.z = data[2] * other.x + data[5] * other.y + data[8] * other.z;

            return result;
        }

        void print() const {
            for (float f : data) {
                cout << f << ", ";
            }
            cout << endl;
        }

    private:
        array<float, 9> data;
};

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
    
    float xD = cameraRotation.x;
    array<float, 9> RxArr = {1.f, 0.f, 0.f, 0.f, cos(xD), sin(xD), 0.f, -sin(xD), cos(xD)};
    Matrix33 Rx(RxArr);
    
    float yD = cameraRotation.y;
    array<float, 9> RyArr = {cos(yD), 0.f, -sin(yD), 0.f, 1.f, 0.f, sin(yD), 0, cos(yD)};
    Matrix33 Ry(RyArr);

    float zD = cameraRotation.z;
    array<float, 9> RzArr = {cos(zD), sin(zD), 0.f, -sin(zD), cos(zD), 0.f, 0.f, 0.f, 1.f};
    Matrix33 Rz(RzArr);

    Matrix33 RotationArr = Rx.right_multiply(Ry.right_multiply(Rz));
    // RotationArr.print();

    sf::Vector3f rotatedVector = RotationArr.vector_right_multiply(s);

    // rotatedVector.y is negative because the coordinate system of the screen is down increases y.
    projected.x = 2.5*rotatedVector.x + winSizeX/2;
    projected.y = -2.5*rotatedVector.y + winSizeY/2;    

    if (rotatedVector.z <= 10) {
        return -1;
    }

    return focalLength/rotatedVector.z;
}

sf::Clock deltaTimeClock;
sf::Time deltaTime;


int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "A below mediocre 3d Renderer");

    vector<Sphere> spheres;
    Sphere s1;
    Sphere s2;
    Sphere s3;
    Sphere s4;
    sphere_init(s1, 0, 0, 100);
    s1.color=sf::Color::White;
    spheres.push_back(s1);
    sphere_init(s2, 60, 60, 100);
    s2.color = sf::Color::Blue;
    spheres.push_back(s2);
    // sphere_init(s3, 0, 60, 100);
    // s3.color = sf::Color::Red;
    // spheres.push_back(s3);
    // sphere_init(s4, 0, 0, 100);
    // s4.color = sf::Color::Cyan;
    // spheres.push_back(s4);

    // for (int i = 0; i < 300; i++) {
    //     Sphere s;
    //     sphere_init(s, (rand() % 1000) - 150, (rand() % 1000) - 150, (rand() % 1000) - 150);
    //     spheres.push_back(s);
    // }

    float paramAngle = 0;
    float paramDistance = 500;
    for (int i = 0; i < 12; i++) {
        Sphere s;
        sphere_init(s, paramDistance*cos(paramAngle), 0, paramDistance*sin(paramAngle));
        spheres.push_back(s);
        Sphere s1;
        sphere_init(s1, 0, paramDistance*cos(paramAngle), paramDistance*sin(paramAngle));
        spheres.push_back(s1);
        Sphere s2;
        sphere_init(s2, paramDistance*sin(paramAngle), paramDistance*cos(paramAngle), 0);
        spheres.push_back(s2);
        paramAngle += (2*pi)/12;
    }

    
    // float i = -(3*3.14)/4;
    sf::Vector3f cameraPosition(0, 0, 0);
    sf::Vector3f cameraRotation(0, 0, 0);

    float focalLength = 100;
    float sphereRadius = 30;

    float dist = 0;

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

        cameraRotation.y += deltaTime.asSeconds();

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