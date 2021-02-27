// main.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
using namespace sf;
const Vector2i win = Vector2i(1280, 720);
const int TileSize = 10;
const float PI = 3.14159265359;
const int countRay = 320;
const float FOV = PI / 4;
const float RS = 5 * PI / 180; //rotate speed
const float PS = float(TileSize) * 10; //player speed
int i;
const float WR = float(win.x) / float(countRay); //width rect ray
const float OR = FOV / float(countRay);
const unsigned char map[15][25] ={
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1 },
	{1,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1 },
	{1,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,1 },
	{1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1 },
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },
	{1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1 },
	{1,0,2,3,4,5,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },
	{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1 },
	{1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1 },
	{1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1 },
	{1,0,0,0,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,1 },
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
};

const int mapW = 25 * TileSize;
const int mapH = 15 * TileSize;
float X = TileSize * 1.5;
float Y = TileSize * 1.5;
float dir = 0;
float Rx, Ry;
float h;
int k;
int x1, dx;
float yor, depth_v;
float x2, depth_h;
int y2, dy;
int my = win.y / 2;

Vector2i mPos;

int InMap(float Xmap, float Ymap)
{
	if ((Xmap >= 0.0 & Xmap <= mapW) & (Ymap >= 0.0 & Ymap <= mapH))
	{
		return map[int(Ymap / TileSize)][int(Xmap / TileSize)]; 
	}
	else { return false; }
}
int Go(float dir, float move)
{
	float cos_a = cos(dir);
	X += cos_a * move;
	if (InMap(X, Y)) { X -= cos_a * move; }

	float sin_a = sin(dir);
	Y += sin_a * move;
	if (InMap(X, Y)) { Y -= sin_a * move; }

	return 0;
}
int main()
{
	RenderWindow window(VideoMode(win.x, win.y), "");
	//window.setFramerateLimit(100);
	Clock clock;
	float Time = 0;
	Event event;
	while (window.isOpen())
	{
		window.setTitle(std::to_string(int(1.f / Time)));
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape) window.close();
			}
		}
		if (event.type == Event::KeyPressed)
		{
			if (event.key.code == Keyboard::Up)
			{
				Go(dir, PS * Time);
			}
			if (event.key.code == Keyboard::Down)
			{
				Go(dir, 0 - PS * Time);
			}
			if (event.key.code == Keyboard::Right)
			{
				Go(dir + PI/2, PS * Time);
			}
			if (event.key.code == Keyboard::Left)
			{
				Go(dir - PI / 2, PS * Time);
			}
		}
		float ldir = dir;
		dir += FOV / 2;
		window.clear(Color(60,60,60));
		RectangleShape sky(Vector2f(win.x, my));
		sky.setFillColor(Color::Cyan);
		window.draw(sky);
		for (i = 0; i < countRay; i++)
		{
			float cos_a = cos(dir);
			float sin_a = sin(dir);
			// verticals
			if (cos_a >= 0)
			{
				x1 = int(X / TileSize) * TileSize + TileSize;
				dx = 1;
			}
			else
			{
				x1 = int(X/TileSize)*TileSize;
				dx = -1;
			}
			unsigned char c1 = 0;
			for (k = 0; k <= 50; k++)
			{
				depth_v = (x1 - X) / cos_a;
				yor = Y + depth_v * sin_a;
				if (InMap(float(x1)+float(dx), yor))
				{
					c1 = InMap(float(x1) + float(dx), yor);
					break;
				}
				x1 += dx * TileSize;
			}
			// horizontals
			if (sin_a >= 0)
			{
				y2 = int(Y / TileSize) * TileSize + TileSize;
				dy = 1;
			}
			else
			{
				y2 = int(Y / TileSize) * TileSize;
				dy = -1;
			}
			unsigned char c2 = 0;
			for (k = 0; k <= 50; k++)
			{
				depth_h = (y2 - Y) / sin_a;
				x2 = X + depth_h * cos_a;
				if (InMap(x2, float(y2)+float(dy)))
				{
					c2 = InMap(x2, float(y2) + float(dy));
					break;
				}
				y2 += dy * TileSize;
			}
			float dist;
			Vector2f pos;
			dist = (depth_h < depth_v) ? depth_h : depth_v;
			pos = (depth_h > depth_v) ? Vector2f(x1, yor) : Vector2f(x2, y2);
			unsigned char touchColor = (depth_h > depth_v) ? c1 : c2;
			float fy = dir - ldir;
			dist *=cos(fy);
			h = 1 / (dist  / float(TileSize)) * (win.x * 1.25) / (FOV / (PI/4));
			dist = dist / float(TileSize);
			unsigned char c = int(255 / (1 + dist * dist * 0.1));
			RectangleShape rect(Vector2f(WR, h));
			Color col = Color::Cyan;
			rect.setFillColor(Color(c, c, c));
			if (touchColor == 1) { rect.setFillColor(Color(c / 2, c / 2, c / 2)); }
			if(touchColor == 2) { rect.setFillColor(Color(c, c / 10, c / 10)); }
			if (touchColor == 3) { rect.setFillColor(Color(c / 10, c, c / 10)); }
			if (touchColor == 4) { rect.setFillColor(Color(c / 10, c / 10, c)); }
			if (touchColor == 5) { rect.setFillColor(Color(c, c, c / 10)); }
			rect.move(win.x - i * WR, my - (h / 2));
			window.draw(rect);
			dir -= OR;
		}
		dir = ldir;
		//draw map
		RectangleShape rect(Vector2f(mapW - TileSize * 5, mapH));
		rect.setFillColor(Color(255, 255, 255));
		window.draw(rect);
		rect.move(0, 0);
		rect.setFillColor(Color(255, 255, 255));
		window.draw(rect);
		Color cor;
		int x1;
		for (x1 = 0; x1 <= mapW / TileSize - 1; x1++)
		{
			int y1;
			for (y1 = 0; y1 <= mapH / TileSize - 1; y1++)
			{
				if (map[y1][x1] > 0)
				{
					RectangleShape box(Vector2f(TileSize, TileSize));
					if (map[y1][x1] == 1) { cor = Color::Black; }
					if (map[y1][x1] == 2) { cor = Color::Red; }
					if (map[y1][x1] == 3) { cor = Color::Green; }
					if (map[y1][x1] == 4) { cor = Color::Blue; }
					if (map[y1][x1] == 5) { cor = Color::Yellow; }
					box.setFillColor(cor);
					box.move(x1 * TileSize, y1 * TileSize);
					window.draw(box);
				}
			}
		}
		VertexArray line(Lines, 2);
		line[0].position = Vector2f(X, Y);
		line[0].color = Color::Red;
		line[1].position = Vector2f((X + cos(dir) * 20), (Y + sin(dir) * 20) );
		line[1].color = Color::Red;
		window.draw(line);
		//draw player
		RectangleShape box(Vector2f(5.f, 5.f));
		box.setFillColor(Color(255, 0, 0));
		box.move(X - 2.5, Y - 2.5);
		window.draw(box);
		mPos = Mouse::getPosition(window);
		dir += float(mPos.x - win.x/2) * 0.0174533;
		my -= (mPos.y - win.y / 2) * 10;
		Mouse::setPosition(Vector2i(win.x / 2, win.y / 2), window);
		window.display();
		Time = clock.getElapsedTime().asSeconds();
		clock.restart();
	}
	return 0;
}