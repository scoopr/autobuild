#ifdef __APPLE__

#include "autobuild.h"
#include <CoreServices/CoreServices.h>
#include <iostream>

class Autobuild::Impl {
public:
    const char* command;
    FSEventStreamRef stream;
    double ignore;
    double last;
};



void fseventerCallback(
    ConstFSEventStreamRef streamRef,
    void *clientCallBackInfo,
    size_t numEvents,
    void *eventPaths,
    const FSEventStreamEventFlags eventFlags[],
    const FSEventStreamEventId eventIds[])
{
    streamRef = streamRef; // unused
    numEvents = numEvents; // unused
    eventPaths = eventPaths; // unused
    eventFlags = eventFlags; // unused
    eventIds = eventIds; // unused

    Autobuild::Impl *mPimpl = static_cast<Autobuild::Impl*>(clientCallBackInfo);


    CFAbsoluteTime now = CFAbsoluteTimeGetCurrent();
    
    // Ignore when events are under ignoretime from last executed event
    if(now - mPimpl->last < mPimpl->ignore) {
        return;
    }

    std::cout << "[autobuild] " << mPimpl->command << std::endl; 
    system(mPimpl->command);

    // set the last updated timestamp *after* command
    mPimpl->last = CFAbsoluteTimeGetCurrent();

}




Autobuild::Autobuild(const AutobuildConfiguration& conf) : mPimpl(new Autobuild::Impl) {
    
    
    CFStringRef path = CFStringCreateWithCString(NULL, conf.directory.c_str(), kCFStringEncodingUTF8 );
    CFArrayRef pathsToWatch = CFArrayCreate(NULL, (const void **)&path, 1, NULL);

    FSEventStreamContext context;
    memset(&context,0,sizeof(context));
    context.version = 0;
    context.info = mPimpl;

    mPimpl->ignore = conf.ignore;
    mPimpl->last = 0;
    mPimpl->command = conf.command.c_str();

    mPimpl->stream = FSEventStreamCreate(NULL,
       &fseventerCallback,
       &context,
       pathsToWatch,
       kFSEventStreamEventIdSinceNow,
       conf.latency,
       kFSEventStreamCreateFlagNone
    );


    FSEventStreamScheduleWithRunLoop(mPimpl->stream, CFRunLoopGetMain(), kCFRunLoopDefaultMode);
    FSEventStreamStart(mPimpl->stream);

    CFRelease(path);
    CFRelease(pathsToWatch);
    
}

Autobuild::~Autobuild() {

    FSEventStreamRelease(mPimpl->stream);

    delete mPimpl;
}


void Autobuild::run() {
    CFRunLoopRun();    
}

#endif
