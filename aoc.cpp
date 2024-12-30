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

#ifdef DAY4
#include "md5.h"
#endif

std::string file_contents;

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

void day1()
{
    //count the floors
    int count = 0;
    //count instruction position
    int position = 0;
    read_input("day1.txt");
    char first_el = file_contents[0];
    for(auto c:file_contents)
    {
        //second part of the condition is for the second part of the puzzle
        if((c == '\n')||(count == -1))
            break;
        if(first_el == c)
            count++;
        else
            count--;

        position++;
    }
    std::cout << count << "\n";
    std::cout << position << "\n";
}

void day2()
{
    std::vector<std::array<int,3>> surface_vector;
    read_input("day2.txt");
    std::size_t idx = 0;
    std::string temp_str;
    std::array<int,3> dimension_array;
    std::array<int,3> multiplication_array;
    int ribbon_length{0};
    for(auto c:file_contents)
    {
        if(c == '\n')
        {
            dimension_array[idx] = std::stoi(temp_str);
            multiplication_array[0] = dimension_array[0]*dimension_array[1];
            multiplication_array[1] = dimension_array[1]*dimension_array[2];
            multiplication_array[2] = dimension_array[2]*dimension_array[0];
            surface_vector.push_back(multiplication_array);
            std::sort(dimension_array.begin(),dimension_array.end());
            ribbon_length += 2*dimension_array[0] + 2*dimension_array[1];
            ribbon_length += dimension_array[0]*dimension_array[1]*dimension_array[2];
            temp_str = "";
            idx = 0;
            continue;
        }
        if (c == 'x')
        {
            dimension_array[idx] = std::stoi(temp_str);
            temp_str = "";
            idx++;
        }
        else
        {
            temp_str += c;
        }
    }

    int total_area{0};
    for(auto tmp:surface_vector)
    {    //for(auto dim:tmp)
        std::array<int,3>::iterator result = std::min_element(tmp.begin(), tmp.end());
         total_area += 2*(tmp[0] + tmp[1] + tmp[2]) + *result;
        // std::cout << (tmp[0] + tmp[1] + tmp[2]);
        //std::cout << "\n";
    }
    std::cout << total_area << std::endl;
    std::cout << ribbon_length << std::endl;

}

void day3()
{
    struct House{
        int x, y, visit_count;
        bool is_visited(int x, int y){return ((x == this->x)&&(y == this->y))?true:false;}
    };
    std::vector<House> houses;
    houses.emplace_back(House{0,0,1});
    read_input("day3.txt");
    int santa_x{0}, santa_y{0};
    int robo_x{0}, robo_y{0};
    bool b_santa = true;
    
    for(auto c:file_contents){
        int* x = &santa_x;
        int* y = &santa_y;
        if(!b_santa)
        {
            x = &robo_x;
            y = &robo_y;
        }
        b_santa = !b_santa;

        if(c == '>')
            *x += 1;
        else if(c == '<')
            *x -= 1;
        else if(c == '^')
            *y += 1;
        else if(c == 'v')
            *y -= 1;
        else
            continue;
        bool b_found{false};
        for(auto& house:houses)
        {
            if(house.is_visited(*x,*y))
            {
                b_found = true;
                house.visit_count++;
                break;
            }
        }
        if(!b_found)
        {
            houses.emplace_back(House{*x, *y, 1});
        }
    }

    std::cout << houses.size() << std::endl; 
}

#ifdef DAY4
void day4()
{
    int number{0};
    std::string input{"yzbqklnj"};
    std::string output{"00000123"};
    while(1)
    {
        std::string tmp = input + std::to_string(number);
        output = md5(tmp);
        if(0== number % 100){
            std::cout << output << std::endl;
            std::cout << tmp  << std::endl;
        }
        if(0 == output.compare(0,6,"000000"))
            break;
        number++;
    }
    std::cout << number << std::endl;
}
#else
void day4()
{}
#endif

void day5()
{
    read_input("day5.txt");
    int count{0};
    auto ss = std::stringstream(file_contents);
    for (std::string line; std::getline(ss, line, '\n');)
    {
       //Part 1
       //std::regex self_regex(".*(ab|cd|pq|xy).*");
       //Part2
       std::regex self_regex("[a-z]*([a-z][a-z])[a-z]*\\1");

        if (std::regex_search(line, self_regex))
        {
            //Part 1
            //std::regex vowel_regex("[aeiou]");
            //auto vowels_begin =  std::sregex_iterator(line.begin(), line.end(), vowel_regex);
            //auto vowels_end = std::sregex_iterator();
            //auto vowel_count = std::distance(vowels_begin, vowels_end);

           // if(vowel_count >= 3)
           // {
           // 
           //   std::regex self_regex2("(.)\\1");
           // Part2
           std::regex self_regex2("([a-z])[a-z]{1}\\1");


            if (std::regex_search(line, self_regex2))
                    count++;
           // }

        }
    }
    std::cout << count << std::endl;
}

void day7()
{
    read_input("day7.txt");
    auto ss = std::stringstream(file_contents);
    enum class OperationType
    {
        OR,
        NOT,
        AND,
        RSHIFT,
        LSHIFT,
        SET,
        END,
        Unknown
    };
    
    class Operation
    {
        private:
            std::string lhi;
            std::string rhi;
            std::string output;
        
            OperationType operation{OperationType::Unknown};

        public:
            Operation(std::string input1, std::string input2, std::string result, OperationType op)
                 : lhi{input1}, rhi{input2}, output{result}, operation{op} {}

            OperationType get_operation(){return operation;}
            std::string get_left() {return lhi;}
            std::string get_right() {return rhi;}
            std::string get_output() {return output;}
            void clear(){operation = OperationType::Unknown;}
          
    };
 
    class Node
    {
        uint16_t value{0};
        bool dirty{false};
       
        
        public:
        void set_value(uint16_t input)
        {
            std::cout << "value set: " << input << std::endl;
            value = input;
            dirty = true;
        }

        void clear() {dirty = false;}
   
        uint16_t get_value() { //dirty = false;
            return value; }
        bool is_dirty() { return dirty; }
    };

   std::map<std::string, Node> circuit;
   std::vector<Operation> v_ops; 
   uint32_t count = 0;
   for (std::string line; std::getline(ss, line, '\n');)
    {
        std::string temp;
        Node* node = nullptr;
        std::vector<std::string> tokens;
        
        for(auto c : line)
        {
            if(' ' != c)
            {
                temp += c;
            } 
            else
            {
                //if(temp != "->")
                    tokens.emplace_back(temp);
                temp = "";
            }
        }
                    tokens.emplace_back(temp);

     // std::cout << "Line: " << count++ <<  std::endl; 
     // for(auto token : tokens)
     //   std::cout << "Token: " << token <<  std::endl; 

      if(tokens[0] == "NOT")
        {
            circuit.try_emplace(tokens[1],Node{});
            circuit.try_emplace(tokens[3],Node{});
            v_ops.emplace_back(Operation{"", tokens[1], tokens[3], OperationType::NOT});
        }
        else if(tokens[1] == "->")
        {
            circuit.try_emplace(tokens[0],Node{});
            if(tokens[0] == "44430")
                circuit[tokens[0]].set_value(44430);
             if(tokens[0] == "0")
                circuit[tokens[0]].set_value(0);
            
            circuit.try_emplace(tokens[2],Node{});
            if(tokens[2] != "a")
                v_ops.emplace_back(Operation{tokens[0], "", tokens[2], OperationType::SET});
            else
                v_ops.emplace_back(Operation{tokens[0], "", tokens[2], OperationType::END});
        }
        else
        {
            circuit.try_emplace(tokens[0],Node{});
            circuit.try_emplace(tokens[2],Node{});
            circuit.try_emplace(tokens[4],Node{});
            OperationType op{OperationType::Unknown};
            if(tokens[1] == "AND")
                op = OperationType::AND;
            if(tokens[1] == "OR")
                op = OperationType::OR;
             if(tokens[1] == "RSHIFT")
                op = OperationType::RSHIFT;
            if(tokens[1] == "LSHIFT")
                op = OperationType::LSHIFT;

            v_ops.emplace_back(Operation{tokens[0], tokens[2], tokens[4], op});
        }
    }

     int pass_counter = 0;
      std::vector<Operation>::iterator it = v_ops.begin();
  while(1)
  {
     // std::cout << "Enter the loop" << std::endl; 
      if(it->get_operation() == OperationType::END)
        if(circuit[it->get_left()].is_dirty())
        {
            std::cout << "The value is: " <<  circuit[it->get_left()].get_value() << std::endl;   
     //       pass_counter++;
      //     if(10 == pass_counter)
            break;
        }
       // else
         //   firstPass = true;
    
     if(it->get_operation() == OperationType::AND)
     {
         if(it->get_left() == "1")
         {
             if(circuit[it->get_right()].is_dirty())
             {
                std::cout << "AND: Left: " << it->get_left() << ", Right: " << it->get_right() << ", Output: " << it->get_output() << std::endl;
                 circuit[it->get_output()].set_value(circuit[it->get_right()].get_value());
         
             }
         }
         else
         {
            if(circuit[it->get_left()].is_dirty() && circuit[it->get_right()].is_dirty())
            {
                std::cout << "AND: Left: " << it->get_left() << ", Right: " << it->get_right() << ", Output: " << it->get_output() << std::endl;
                circuit[it->get_output()].set_value(circuit[it->get_left()].get_value() & circuit[it->get_right()].get_value());
        
            }
         }
     }
     else if(it->get_operation() == OperationType::OR)
     {
            if(circuit[it->get_left()].is_dirty() && circuit[it->get_right()].is_dirty())
            {
                std::cout << "OR: Left: " << it->get_left() << ", Right: " << it->get_right() << ", Output: " << it->get_output() << std::endl;
                circuit[it->get_output()].set_value(circuit[it->get_left()].get_value() | circuit[it->get_right()].get_value());
            }
     }
     else if(it->get_operation() == OperationType::LSHIFT)
     {
        if(circuit[it->get_left()].is_dirty())
        {
           std::cout << "LSHIFT: Left: " << it->get_left() << ", Right: " << it->get_right() << ", Output: " << it->get_output() << std::endl;
            if(it->get_right() == "1")
                circuit[it->get_output()].set_value(circuit[it->get_left()].get_value()<< 1);
            else
                circuit[it->get_output()].set_value(circuit[it->get_left()].get_value()<< 15);
        }
     }
     else if(it->get_operation() == OperationType::RSHIFT)
     {
        if(circuit[it->get_left()].is_dirty())
        {
            std::cout << "RSHIFT: Left: " << it->get_left() << ", Right: " << it->get_right() << ", Output: " << it->get_output() << std::endl;
            int to_shift = stoi(it->get_right());
            circuit[it->get_output()].set_value(circuit[it->get_left()].get_value() >> to_shift);
        }
     }
     else if(it->get_operation() == OperationType::NOT)
     {
        if(circuit[it->get_right()].is_dirty())
        {
            std::cout << "NOT: Left: " << it->get_left() << ", Right: " << it->get_right() << ", Output: " << it->get_output() << std::endl;
            uint16_t value =  circuit[it->get_right()].get_value();
          //  std::cout << "Operation NOT. Input: " << value << " Invert: " << ~value << std::endl; 
            circuit[it->get_output()].set_value(~value);
        }
     }
     else if(it->get_operation() == OperationType::SET)
     {
        if(circuit[it->get_left()].is_dirty())
        {
            std::cout << "SET: Left: " << it->get_left() << ", Right: " << it->get_right() << ", Output: " << it->get_output() << std::endl;
            circuit[it->get_output()].set_value(circuit[it->get_left()].get_value());

        }
     }    
     it++;
    if(it == v_ops.end())
       it = v_ops.begin(); 

  } 
}


int main()
{
    day7();
}
