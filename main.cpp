
#include "autobuild.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{

    if(argc <= 1) { 
        std::cout << "Usage: " << argv[0] << " {cmd}" << std::endl;
        return EXIT_FAILURE;
    }


    AutobuildConfiguration conf;
    conf.directory = ".";
    conf.command = argv[1];
    conf.latency = 0.5;
    conf.ignore = 1.0;

    Autobuild ab(conf);
    
    ab.run();
    
    return EXIT_SUCCESS;
}

