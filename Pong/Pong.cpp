#include <raylib.h>
#include <ctime>
#include <iostream>
#include <string.h>

struct Ball 
{
	float x, y;
	float speedX, speedY;
	float r;


	void Draw() 
	{
		DrawCircle(x, y, r, WHITE);
	}
		
};

struct Paddle
{
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect()
	{
		return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
	}

	void Draw() 
	{
		DrawRectangleRec(GetRect(), WHITE);
	}
};

int main()
{
	InitWindow(800, 600, "Pong"); //Создаем окно, задаем разрешение и название окну
	SetWindowState(FLAG_VSYNC_HINT); //Вертикальную синхронизацию включает
	srand(time(NULL));

restart:
	int scoreL = 0;
	int scoreR = 0;
	const char* winner = nullptr;

stillgame:
	int plusminus[2] = {-1, 1};
	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.r = 5;

	ball.speedX = 300 * plusminus[(rand() % 2)];
	ball.speedY = 300 * plusminus[(rand() % 2)];

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 500; 

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 60;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;

	

	while (!WindowShouldClose()) //Луп для работы с окном
	{
		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		if (ball.y < 0)
		{
			ball.y = 0;
			ball.speedY *= -1;
		}
		if (ball.y > GetScreenHeight())
		{
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}

		if (IsKeyDown(KEY_W))
		{
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}else if (IsKeyDown(KEY_S))
		{
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_UP))
		{
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}else if (IsKeyDown(KEY_DOWN))
		{
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}

		if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.r,
			leftPaddle.GetRect()))
		{ 
			if (ball.speedX < 0) 
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.r,
			rightPaddle.GetRect()))
		{
			if (ball.speedX > 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * (-1 * ball.speedX);
			}
		}

		if (ball.x < 0 && scoreR < 5)
		{
			scoreR += 1;
			if(scoreR<5) goto stillgame;
		}
		if (ball.x > GetScreenWidth() && scoreL < 5)
		{ 
			scoreL += 1; 
			if (scoreL < 5)goto stillgame;
		}

		if (scoreL == 5)
		{
			winner = "Left Player Wins!\nSpace to restart";
		}
		else if (scoreR == 5) { winner = "Right Player Wins!\nSpace to restart"; }
		if (winner && IsKeyPressed(KEY_SPACE))
		{
			goto restart;
		}
		
		BeginDrawing();
			ClearBackground(BLACK);

			ball.Draw();
			leftPaddle.Draw();
			rightPaddle.Draw();
			//int textWidth = MeasureText(winnerText, 60);
			if (winner)
			{
				int textWidth = MeasureText(winner, 30);
				DrawText(winner, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 40, 30, WHITE);
			}

			DrawText(TextFormat("%i:%i", scoreL, scoreR), GetScreenWidth() / 2 - 15, 15, 30, WHITE);

			DrawFPS(0, 0);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
