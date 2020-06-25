// SFMLTutorial.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Program.h"
#include "Game.h"

int main(int argc, const char* argv[])
{
    // SFMLTutorial::Program app;
    // app.Start();

    SFMLTutorial::Game game;
    while (!game.GetWindow().IsClose())
    {
        // game.HandleInput();
        game.Update();
        game.Render();
        game.LateUpdate();
        // sf::sleep(sf::seconds(0.2f)); // Sleep 0.2s
        // game.RestartClock();
    }

    return EXIT_SUCCESS;
}
