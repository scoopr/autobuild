#ifndef AUTOBUILD_AUTOBUILD_H
#define AUTOBUILD_AUTOBUILD_H


#include <string>

struct AutobuildConfiguration {
    std::string directory;
    std::string command;
    double latency;
    double ignore;


};


class Autobuild {
public:
    
    Autobuild(const AutobuildConfiguration& conf);
    ~Autobuild();
    
    
    void run();

    class Impl;
private:
    Impl *mPimpl;
    
};


#endif

