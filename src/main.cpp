#include "pseudo_unorderd_map.h"
#include <iostream>
#include <string>

#define REDIS_IP "127.0.0.1"
#define REDIS_PORT 6379

int main(int argc, char** argv)
{
    /// A simple test. 
    PseudoUnorderedMap<std::string, std::string> p(REDIS_IP, REDIS_PORT);

    p.insert("Hossein","Nazari");
    std::cout << p.at("Hossein") << std::endl;


   return 0;
}
