#include <SFML/Graphics.hpp>
#include <time.h>

const int RES_X = 640, RES_Y = 720, CUBE_RES = 32;

const int M = 21, N = 12;

int field[M][N] = {0};

struct Point {int x, y;} a[4], b[4];

int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,4,5,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

bool check()
{
    for (int i = 0; i < 4; i++)
        if(a[i].x < 0 || a[i].x>=N-1 || a[i].y>=M) return 0;
        else if(field[a[i].y][a[i].x]) return 0;
    return 1;
}

int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(RES_X, RES_Y), "TETRIS");

    sf::Texture tex;
    tex.loadFromFile("images/tiles.png");

    sf::Sprite s(tex), s2(tex);
    s.setTextureRect(sf::IntRect(0,0,CUBE_RES,CUBE_RES));
    s2 = s;
    int dx = 0; bool rotate = 0; int colorNum = 1;
    float timer = 0, delay = 0.3;
    sf::Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
                if(event.key.code == sf::Keyboard::Up) rotate = true;
                else if (event.key.code == sf::Keyboard::Left) dx=-1;
                else if (event.key.code == sf::Keyboard::Right) dx=+1;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delay = 0.05f;

        //// <-- Motion --> ////
        for (int i = 0; i < 4; i++) {b[i] = a[i]; a[i].x+=dx;}
        if(!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

        //// <-- Rotation --> ////
        if(rotate)
        {
            Point p = a[1]; // center of rotation
            for(int i = 0; i < 4; i++)
            {
                int x = a[i].y-p.y;
                int y = a[i].x-p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if(!check()) for(int i = 0; i < 4; i++) a[i] = b[i];
        }

        //// <-- Tick --> ////
        if(timer>delay)
        {
            for (int i = 0; i < 4; i++)
                {b[i] = a[i]; a[i].y+=1;}

            if(!check())
            {
                for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;

                colorNum = 1 + rand()%6;
                int n = rand()%6 +1;
                for (int i = 0; i < 4; i++)
                {
                    a[i].x = figures[n][i]%2;
                    a[i].y = figures[n][i]/2;
                }

            }
            timer = 0;
        }

        //// <-- Checking Lines --> ////
        int k = M - 1;
        for (int i = M-1; i > 0; i--)
        {
            int count = 0;
            for (int j = 0; j < N; j++)
            {
                if(field[i][j]) count++;
                field[k][j] = field[i][j];
            }
            if(count < N) k--;
        }

        dx = 0; rotate = 0; delay = 0.3;

        //// <-- Drawing --> ////
        window.clear(sf::Color::Black);


        // ->> Frame <<- //
        for(int i = 0; i <= M; i++)
        {
            s2.setPosition(0*CUBE_RES, i*CUBE_RES);
            window.draw(s2);
            s2.setPosition(N*CUBE_RES, i*CUBE_RES);
            window.draw(s2);
        }
        for(int i = 0; i <=N; i++)
        {
            s2.setPosition(i*CUBE_RES, M*CUBE_RES);
            window.draw(s2);
        }


        // ->> Cubes <<- //
        for(int i = 0; i < M; i++)
            for(int j = 0; j < N; j++)
            {
                if(field[i][j] == 0) continue;
                s.setTextureRect(sf::IntRect(field[i][j]*CUBE_RES, 0, CUBE_RES, CUBE_RES));
                s.setPosition((j+1)*CUBE_RES, i*CUBE_RES);
                //s.move(28,31); // offset
                window.draw(s);
            }

        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(sf::IntRect(colorNum*CUBE_RES, 0, CUBE_RES, CUBE_RES));
            s.setPosition((a[i].x+1)*CUBE_RES, a[i].y*CUBE_RES);
            window.draw(s);
        }
        window.display();
    }

    return 0;
}
