#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	bricks.clear();
	for (int i = 0; i < 5; ++i)
	{	
		Box brick;
		brick.width = 10;
		brick.height = 2;
		brick.x_position = i * (brick.width + 6);
		brick.y_position = 5;
		brick.doubleThick = true;
		brick.color = ConsoleColor::DarkGreen;
		bricks.push_back(brick);
	}

	//game start instructions
	//commit
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	if (ball.moving)
	{
		ball.Update();
		CheckCollision();
	}

	if (bricks.empty())
	{
		ball.moving = false;
	}

	if (ball.y_position >= WINDOW_HEIGHT - 1)
	{
		ball.moving = false;
	}

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering should be done between the locks in this function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (int i = 0; i < bricks.size(); ++i)
	{
		bricks[i].Draw();
	}

	// TODO #6 - If no bricks remain, pause ball and display victory text with R to reset
	if (bricks.empty())
	{
		Console::SetCursorPosition(WINDOW_WIDTH / 2 - 15, WINDOW_HEIGHT / 2);
		std::cout << "You win! 'R' to play again.";
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display defeat text with R to reset
	if (ball.y_position >= WINDOW_HEIGHT - 1)
	{
		Console::SetCursorPosition(WINDOW_WIDTH / 2 - 15, WINDOW_HEIGHT / 2);
		std::cout << "You lose. 'R' to play again.";
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (int i = 0; i < bricks.size(); ++i)
	{
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			//reverse ball direction
			if (ball.x_position < bricks[i].x_position || ball.x_position > bricks[i].x_position + bricks[i].width - 1)
				ball.x_velocity *= -1; //had my x_velocity and y_velocity mixed up, this fixed where my ball was morphing through
			//the bricks instead of "hitting" them
			else
				ball.y_velocity *= -1;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (bricks[i].color == ConsoleColor::DarkGreen)
			{
				bricks[i].color = ConsoleColor::Green;
			}
			else if (bricks[i].color == ConsoleColor::Green)
			{
				bricks[i].color = ConsoleColor::Blue;
			}
			else if (bricks[i].color == ConsoleColor::Blue)
			{
				bricks.erase(bricks.begin() + i);
				i--;
			}
			break;
		}
			//reverse ball direction if it hits the paddle
			if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
			{
				ball.y_velocity *= -1;
			}
	}
}
