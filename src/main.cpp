#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
using namespace sf;

// ! Need to be in the game:
// TODO: Colors
// TODO: Save shapes
// TODO: Predict next shape
// TODO: Losing

// ? Should be in the game:
// New graphics
// Improved UI
// Main menu
// Replace "cleared lines" with "actual scores"

// ? Would be in the game:
// Multiplayer

// ? Stuff that will never be in the game:
// Block dating sim
// Battle royale

const int height = 20;
const int width = 10;

static int field[height][width] = {0};

// a = current position, b = last postion.
struct Point
{
    int x, y;
} a[4], b[4];

// 8 fileds with 4-4 columns,
// making 2 colums for the shapes to fill.
int shapes[7][4] = {
    1, 3, 5, 7, // I
    2, 4, 5, 7, // Z
    3, 5, 4, 6, // S
    3, 5, 4, 7, // T
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5  // O
};

bool check()
{
    for (int i = 0; i < 4; i++)
    {
        // Check for the shape being in the playfield.
        if (a[i].x < 0 || a[i].x >= width || a[i].y >= height)
            return false;

        // The more I look at it, the more I want to puke.
        if (field[a[i].y][a[i].x])
            return false;
    }

    return true;
};

// Main function for running the game.
int main()
{
    // Generates a random seed for the session.
    srand(time(0));

    // The main render for the window.
    RenderWindow window(VideoMode(320, 480), "Tetris Clone");

    // This is the tile for the tetris block.
    RectangleShape shape(Vector2f(18.f, 18.f));
    shape.setFillColor(Color::Green);

    // Shape modifiers.
    int dx = 0;
    bool rotate = false;
    int color = 1;

    // Game clock variables.
    float timer = 0.f, delay = 0.3f;
    static Clock clock;

    int score = 0.f;

    static Font font;
    if (!font.loadFromFile("src/hemi.ttf"))
        std::cout << "Error loading font file." << std::endl;

    static Text scoreTextLabel;
    scoreTextLabel.setFont(font);
    scoreTextLabel.setString("Cleared:");
    scoreTextLabel.setCharacterSize(24);
    scoreTextLabel.setFillColor(Color::White);

    static Text scoreLabel;
    scoreLabel.setFont(font);
    scoreLabel.setCharacterSize(24);
    scoreLabel.setFillColor(Color::White);

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;

            case Event::KeyPressed:

                switch (event.key.code)
                {
                case Keyboard::Up:
                    rotate = true;
                    break;
                case Keyboard::Left:
                    dx = -1;
                    break;
                case Keyboard::Right:
                    dx = 1;
                    break;

                default:
                    break;
                }
                break;

            default:
                break;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
            delay = 0.05;

        // * Movement
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].x += dx;
        }

        if (!check())
        {
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
        }

        // * Rotation
        if (rotate)
        {
            // Center of the rotation.
            Point point = a[1];

            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - point.y;
                int y = a[i].x - point.x;
                a[i].x = point.x - x;
                a[i].y = point.y + y;
            }
        }

        if (!check())
        {
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
        }

        // * Tick system (falling)
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++)
            {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!check())
            {
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = color;

                int n = rand() % 7;
                color = 1 + n;

                for (int i = 0; i < 4; i++)
                {
                    a[i].x = shapes[n][i] % 2;
                    a[i].y = shapes[n][i] / 2;
                }
            }

            timer = 0;
        }

        dx = 0;
        rotate = false;
        delay = 0.3f;

        // * Line clear
        int k = height - 1;

        for (int i = height - 1; i > 0; i--)
        {
            int count = 0;

            for (int j = 0; j < width; j++)
            {
                if (field[i][j])
                    count++;

                field[k][j] = field[i][j];
            }

            if (count < width)
                k--;
        }

        // * Scores
        score += k;
        scoreLabel.setString(std::to_string(score));

        // * Draw to window
        window.clear(Color::Black);

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (field[i][j] == 0)
                    continue;

                shape.setPosition(j * 18, i * 18);
                window.draw(shape);
            }
        }

        for (int i = 0; i < 4; i++)
        {
            shape.setPosition(a[i].x * 18, a[i].y * 18);
            window.draw(shape);
        }

        scoreTextLabel.setPosition(200, 240);
        window.draw(scoreTextLabel);

        scoreLabel.setPosition(235, 270);
        window.draw(scoreLabel);

        window.display();
    }

    return 0;
}