#ifndef DATA_TEST_H
#define DATA_TEST_H

#include <string>

class DataTest
{
    public:

        DataTest();

        template <typename TYPE>
        TYPE get_data();        
 
        std::string get_string();
        std::string get_string(size_t size);
        std::string get_string(size_t min, size_t max);
        int get_int();
        int get_int(size_t min, size_t max);
        double get_float();
        double get_float(double min, double max);
        char get_char();
        bool get_bool(); 
            
        
};

#endif
