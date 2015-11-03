#include <iostream>
#include <stdio.h>

#include "glm/glm.hpp"
#include <SFML/Graphics.hpp>

using namespace std;

const int WindowWidth = 600, WindowHeight = 600;
const int gridRows = 30, gridColumns = 30;

float Lerp(float a0, float a1, float w) 
{
    return (1.0f - w) * a0 + w * a1;
}
 
float GetRand()
{
    return float(rand()%10000) / 10000;
}

glm::vec2 GetRandomNormalizedVector()
{
    return glm::normalize( glm::vec2(GetRand() * 2.0f - 1.0f, GetRand() * 2.0f - 1.0f) );
}

sf::Color Vec3ToColor(glm::vec3 color)
{
  unsigned char r = (unsigned char) glm::clamp(color.r * 255.0f, 0.0f, 255.0f);
  unsigned char g = (unsigned char) glm::clamp(color.g * 255.0f, 0.0f, 255.0f);
  unsigned char b = (unsigned char) glm::clamp(color.b * 255.0f, 0.0f, 255.0f);
  return sf::Color(r,g,b);
}

int main(int argc, char **args)
{ 
  srand(time(0));
  
  sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Perlin Noise", sf::Style::Titlebar | sf::Style::Close);
  
  glm::vec2 grid[gridRows+1][gridColumns+1];
  for(int i = 0; i < gridRows; ++i)
  {
    for(int j = 0; j < gridColumns; ++j)
    {
        grid[i][j] = GetRandomNormalizedVector();
    }
  }
  
  sf::Image image;
  image.create(WindowWidth, WindowHeight);
  
  int tileWidthPixels  = (WindowWidth  / gridColumns);
  int tileHeightPixels = (WindowHeight / gridRows);
  for(int x = 0; x < WindowWidth; ++x)
  {
    for(int y = 0; y < WindowHeight; ++y)
    {
        int i = x/tileWidthPixels;
        int j = y/tileHeightPixels;
        glm::vec2 pixelCoordInsideGrid(x % tileWidthPixels, y % tileHeightPixels);
        
        // [g0] | [g1]
        // -----------
        // [g2] | [g3]
        glm::vec2 g0 = grid[i][j],   g1 = grid[i+1][j],
                  g2 = grid[i][j+1], g3 = grid[i+1][j+1];
        
        glm::vec2 d0 = glm::vec2(x,y) - glm::vec2(tileWidthPixels*(i),   tileHeightPixels*(j));
        glm::vec2 d1 = glm::vec2(x,y) - glm::vec2(tileWidthPixels*(i+1), tileHeightPixels*(j));
        glm::vec2 d2 = glm::vec2(x,y) - glm::vec2(tileWidthPixels*(i),   tileHeightPixels*(j+1));
        glm::vec2 d3 = glm::vec2(x,y) - glm::vec2(tileWidthPixels*(i+1), tileHeightPixels*(j+1));
        
        d0.x /= WindowWidth; d0.y /= WindowHeight;
        d1.x /= WindowWidth; d1.y /= WindowHeight;
        d2.x /= WindowWidth; d2.y /= WindowHeight;
        d3.x /= WindowWidth; d3.y /= WindowHeight;
        
        float dot0 = glm::dot(g0, (d0)), dot1 = glm::dot(g1, (d1)),
              dot2 = glm::dot(g2, (d2)), dot3 = glm::dot(g3, (d3));
        
        float u = float(pixelCoordInsideGrid.x) / (tileWidthPixels);
        float v = float(pixelCoordInsideGrid.y) / (tileHeightPixels);
        float x0 = Lerp(dot0, dot1, u);
        float x1 = Lerp(dot2, dot3, u);
        float pixelValue = Lerp(x0, x1, v);
        
        //cout << i << ", " << j << endl;        
        //sf::Color color = Vec3ToColor(glm::vec3(u, v, 0.0));
        //cout << pixelValue << endl;
        sf::Color color = Vec3ToColor(glm::vec3(1.0f, 0.0f, 0.0f) * pow(pixelValue + 0.9f, 5.0f));
        image.setPixel(x, y, color);
    }
  }
  
  sf::Texture texture; texture.loadFromImage(image);
  sf::Sprite sprite; sprite.setTexture(texture);
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed) window.close();
      
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space)
            {
            }
        }
    }
    window.draw(sprite);
    window.display();
  }
  return 0;
}
