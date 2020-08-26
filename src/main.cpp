#include "pseudo_unorderd_map.h"
#include <iostream>
#include <string>


int main(int argc, char** argv)
{
  	/// A simple test. 
    PseudoUnorderedMap<std::string, std::string> p("/var/run/redis/redis.sock", SHARED_MEMORY);

    p.insert("Hossein","Nazari");
    std::cout << p.at("Hossein") << std::endl;
   
   return 0;
}
