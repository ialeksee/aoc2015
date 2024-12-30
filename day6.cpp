#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <algorithm>
#include <memory>
#include <sstream>
#include <regex>

std::string file_contents;
std::stringstream ss;
void read_input(std::string filename)
{
    std::ifstream file(filename);
    std::string str;
   
    while (std::getline(file, str))
    {
        file_contents += str;
        file_contents.push_back('\n');
    } 
}

class Day6 : public olc::PixelGameEngine
{ 
    //Part1
    // std::array<std::array<bool,1000>,1000> grid{};
    
    //Part2
    std::array<std::array<uint32_t,1000>,1000> grid{};
    
    void update_grid()
    {
        std::string line;
        std::array<std::string,4> instructions;
        std::size_t idx = 0;
        if(std::getline(ss, line, '\n'))
        {
            auto c = line.begin();
            while(c != line.end())
            {
                if((' ' != *c) && ('\n' != *c))
                    instructions[idx] += *c;
                else if(' ' == *c)
                {
                    if((*(c+1) < 0x61) || (*(c-1) <= 0x39))
                    {
                        c++;
                        idx++;
                        continue;
                    }
                }
                c++;
            }
           // for(auto instr : instructions)
             //   std::cout << instr << std::endl;

            int start_x{0}, start_y{0};
            int end_x{0}, end_y{0};
            std::string temp;
              
            for(auto c : instructions[1])
            {
                if(','!=c)
                    temp += c;
                else
                {
                    start_x = stoi(temp);
                    temp = "";
                }
            }
            start_y = stoi(temp);
            temp = "";
              
            for(auto c : instructions[3])
            {
                if(','!=c)
                    temp += c;
                else
                {
                    end_x = stoi(temp);
                    temp = "";
                }
            }
            end_y = stoi(temp);

            if(instructions[0] == "turnon")
            {
              //std::cout << "start: "<< start_x << " " << start_y << std::endl; 
              //std::cout << "end: "<< end_x << " " << end_y << std::endl;
              for(std::size_t x = start_x; x <= end_x; x++)
                 for(std::size_t y = start_y; y <= end_y; y++)
                   // grid[y][x] = true; 
                   grid[y][x]++; 
            }
            
            if(instructions[0] == "turnoff")
            {
              //std::cout << "start: "<< start_x << " " << start_y << std::endl; 
              //std::cout << "end: "<< end_x << " " << end_y << std::endl;
              for(std::size_t x = start_x; x <= end_x; x++)
                 for(std::size_t y = start_y; y <= end_y; y++)
                    //grid[y][x] = false; 
                  if(grid[y][x] != 0) grid[y][x]--; 
            }
             
            if(instructions[0] == "toggle")
            {
              //std::cout << "start: "<< start_x << " " << start_y << std::endl; 
              //std::cout << "end: "<< end_x << " " << end_y << std::endl;
              for(std::size_t x = start_x; x <= end_x; x++)
                 for(std::size_t y = start_y; y <= end_y; y++)
                   // grid[y][x] = !grid[y][x]; 
                    grid[y][x] += 2; 
            }
        }
        else
        {
            int count{0};
            static bool once = false;
            if(!once)
            {
                for(auto row : grid)
                    for(auto flag : row)
                       // if(flag) count++;
                       count += flag;

                std::cout << count << std::endl;
                once = true;
            }
        }
    }

    public:
        Day6()
        {
            sAppName = "Christmas Lights";
        }
    public:
        bool OnUserCreate() override
        {
          //  for(auto& row : grid)
            //    for(auto& pixel : row)
              //      pixel = false;
            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override
        {
            update_grid();
            for(int x = 0; x < ScreenWidth(); x++)
                for(int y = 0; y < ScreenHeight(); y++)
                   //if(grid[y][x])
                   //     Draw(x, y, olc::YELLOW);
                   // else
                   //     Draw(x, y, olc::DARK_BLUE);
                   Draw(x, y, olc::Pixel(0, 0, (grid[y][x]*5)%255));

            return true;
        }
};

int main()
{
    read_input("day6.txt");
    ss << file_contents;
    Day6 lights;
    if(lights.Construct(1000, 1000, 1, 1))
        lights.Start();
}
