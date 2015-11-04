#include <iostream>
#include <vector>
#include <stdio.h>

#include "glm/glm.hpp"
#include <SFML/Graphics.hpp>

using namespace std;

const int WindowWidth = 700, WindowHeight = 700;
const int gridRowsMin = 10, gridColumnsMin = 10;
const int gridRowsMax = 10, gridColumnsMax = 10;
int gridStep = 10;


float Lerp(float a0, float a1, float w) 
{
    return (1.0f - w) * a0 + w * a1;
}
 
float GetRand()
{
    return float(rand()%1000) / 1000;
}

glm::vec2 GetRandomNormalizedVector()
{
    return glm::normalize( glm::vec2(GetRand() * 2.0f - 1.0f, GetRand() * 2.0f - 1.0f) );
}

sf::Color Vec3ToColor(glm::vec3 color)
{
  unsigned char r = (unsigned char) glm::clamp(color.x * 255.0f, 0.0f, 255.0f);
  unsigned char g = (unsigned char) glm::clamp(color.y * 255.0f, 0.0f, 255.0f);
  unsigned char b = (unsigned char) glm::clamp(color.z * 255.0f, 0.0f, 255.0f);
  return sf::Color(r,g,b);
}

int main(int argc, char **args)
{ 
  srand(time(0));
  
  sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Perlin Noise", sf::Style::Titlebar | sf::Style::Close);
  
  sf::Image image;
  image.create(WindowWidth, WindowHeight);
    
  vector<vector<glm::vec3>> imageAccum(WindowWidth, vector<glm::vec3>(WindowHeight, glm::vec3(0.0f)));
  
  int gridRows = gridRowsMin;
  int gridColumns = gridColumnsMin;
  int steps = 0;
  while(gridRows <= gridRowsMax && gridColumns <= gridColumnsMax)
  {
    vector<vector<glm::vec2>> grid(gridRows+1, vector<glm::vec2>(gridColumns+1));
    for(int i = 0; i < grid.size(); ++i)
    {
        for(int j = 0; j < grid[i].size(); ++j)
        {
            grid[i][j] = GetRandomNormalizedVector();
        }
    }
    
    int tileWidthPixels  = WindowWidth  / gridColumns;
    int tileHeightPixels = WindowHeight / gridRows;
    for(int x = 0; x < WindowWidth; ++x)
    {
        for(int y = 0; y < WindowHeight; ++y)
        {
            // [g0] | [g1]
            // -----------
            // [g2] | [g3]
            int i = y/tileHeightPixels;
            int j = x/tileWidthPixels;
            glm::vec2 g0 = grid[i][j],   g1 = grid[i][j+1],
                      g2 = grid[i+1][j], g3 = grid[i+1][j+1];
            
            glm::vec2 d0 = glm::vec2(x,y) - glm::vec2(tileHeightPixels * (i),   tileWidthPixels * (j)  ) + glm::vec2(0.000001);
            glm::vec2 d1 = glm::vec2(x,y) - glm::vec2(tileHeightPixels * (i),   tileWidthPixels * (j+1)) + glm::vec2(0.000001);
            glm::vec2 d2 = glm::vec2(x,y) - glm::vec2(tileHeightPixels * (i+1), tileWidthPixels * (j)  ) + glm::vec2(0.000001);
            glm::vec2 d3 = glm::vec2(x,y) - glm::vec2(tileHeightPixels * (i+1), tileWidthPixels * (j+1)) + glm::vec2(0.000001);
            
            float dot0 = glm::dot((d0), g0), dot1 = glm::dot((d1), g1),
                  dot2 = glm::dot((d2), g2), dot3 = glm::dot((d3), g3);
            
            glm::vec2 pixelCoordInsideGrid(x % tileWidthPixels, y % tileHeightPixels);
            float u = float(pixelCoordInsideGrid.x) / (tileWidthPixels);
            float v = float(pixelCoordInsideGrid.y) / (tileHeightPixels);
            float x0 = Lerp(dot0, dot1, u);
            float x1 = Lerp(dot2, dot3, u);
            float pixelValue = Lerp(x0, x1, v);
            
            glm::vec3 c = glm::vec3(1.0f, 1.0f, 1.0f) * (pixelValue * pixelValue);
            imageAccum[x][y] += c;
        }
    }
    
    gridRows += gridStep, 
    gridColumns += gridStep;
    ++steps;
  }
  
            
  for(int x = 0; x < WindowWidth; ++x)
  {
    for(int y = 0; y < WindowHeight; ++y)
    {
        imageAccum[x][y] /= steps;
        sf::Color color = Vec3ToColor(imageAccum[x][y]);
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
