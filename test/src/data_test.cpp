#include "data_test.h"
#include <iostream>
#include <string>
#include <random>



DataTest::DataTest()
{
}


template <>
std::string DataTest::get_data()
{
    return get_string();
}


template <>
int DataTest::get_data()
{
    return get_int();
}


template <>
double DataTest::get_data()
{
    return get_float();
}


template <>
bool DataTest::get_data()
{
    return get_bool();
}


template <>
char DataTest::get_data()
{
    return get_char();
}


std::string DataTest::get_string()
{
    std::random_device generator; 
    std::mt19937 engine(generator());
    size_t min_size = 1;
    size_t max_size = 20;
    std::uniform_int_distribution<int> distribution(min_size, max_size);
    size_t str_size = distribution(engine);
    return get_string(str_size);
}



std::string DataTest::get_string(size_t size)
{
    std::string new_str = "";
    for (int ptr = 0; ptr < size; ptr++)
    {
        char ch = get_char();
        if (ch == '"')
            new_str += '.';
        else
            new_str += ch;
    }
    return new_str;
}


int DataTest::get_int()
{
    int min = -100000;
    int max = 100000;
    return get_int(min, max); 
}

int DataTest::get_int(size_t min, size_t max)
{ 
    std::random_device generator; 
    std::mt19937 engine(generator());

    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(engine); 
}


double DataTest::get_float()
{ 
    double min = -100000.0;
    double max = 100000.0;
    return get_float(min, max); 
}


double DataTest::get_float(double min, double max)
{
    std::random_device generator; 
    std::mt19937 engine(generator());
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(engine);    
}


char DataTest::get_char()
{
    int first_char = 32;
    int last_char = 126;
    return get_int(first_char, last_char); 
}


bool DataTest::get_bool()
{
    return get_int(0, 1);
}

