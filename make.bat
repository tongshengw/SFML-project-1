g++ -c main.cpp Card.cpp -I"C:\Users\tongs\Documents\code\libraries\SFML-2.6.2-64\include" -DSFML_STATIC 
g++ main.o Card.o -o main -L"C:\Users\tongs\Documents\code\libraries\SFML-2.6.2-64\lib" -lsfml-graphics -lsfml-window -lsfml-system -std=c++17
.\main.exe