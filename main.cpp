
#include "autobuild.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

enum ConfType {
    CONF_NONE = 0,
    CONF_DIR,
    CONF_CMD,
    CONF_LAT,
    CONF_IGN
};


inline double toDouble(const std::string& s) {
   std::istringstream i(s);
   double x;
   if (!(i >> x)) {
       std::cerr << "Could not parse '" << s << "' as a number";
       exit(EXIT_FAILURE);
   }

   return x;
}

int main(int argc, char **argv)
{


    AutobuildConfiguration conf;
    conf.directory = ".";
    conf.latency = 0.1;
    conf.ignore = 0.5;


    if(argc <= 1) { 
        std::cerr << "Usage: " << argv[0] << " [options] cmd" << std::endl;
        std::cerr << "    -d   Directory to watch. Default: " << conf.directory << std::endl;
        std::cerr << "    -l   Latency. Default: " << conf.latency << std::endl;
        std::cerr << "    -i   Ignore duration. Default: " << conf.ignore << std::endl;
        return EXIT_FAILURE;
    }




    ConfType type = CONF_NONE;
    for(int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        switch(type) {
        case CONF_DIR: conf.directory = arg; type = CONF_NONE; break; 
        case CONF_CMD: 
            conf.command += " ";
            conf.command += arg;  
            break; 
        case CONF_LAT: conf.latency = toDouble(arg); type = CONF_NONE; break;
        case CONF_IGN: conf.ignore  = toDouble(arg); type = CONF_NONE; break;
        default:
            if(arg == "-d") type = CONF_DIR;
            else if(arg == "-l") type = CONF_LAT;
            else if(arg == "-i") type = CONF_IGN;
            else {
                conf.command = arg;
                type = CONF_CMD;
            }
        }
    }

    if(conf.command.size() == 0) {
        std::cerr << "Warning: no command to run?" << std::endl;
    }


    Autobuild ab(conf);
    
    ab.run();
    
    return EXIT_SUCCESS;
}

