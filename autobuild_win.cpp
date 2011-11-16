#ifdef _WIN32

#include "autobuild.h"
#include <windows.h>

#include <iostream>

class Autobuild::Impl {
public:    
    HANDLE dwChangeHandle;
    const AutobuildConfiguration *conf;
    double last;
};

Autobuild::Autobuild(const AutobuildConfiguration& conf) : mPimpl(new Autobuild::Impl) {
    mPimpl->conf = &conf;
    mPimpl->last = 0;
        
    DWORD notifyFilter = FILE_NOTIFY_CHANGE_FILE_NAME  |
                         FILE_NOTIFY_CHANGE_DIR_NAME   |
                         FILE_NOTIFY_CHANGE_ATTRIBUTES |
                         FILE_NOTIFY_CHANGE_SIZE       |
                         FILE_NOTIFY_CHANGE_LAST_WRITE |
                         FILE_NOTIFY_CHANGE_SECURITY;

    mPimpl->dwChangeHandle = FindFirstChangeNotificationA( 
       conf.directory.c_str(),
       TRUE,          // watch subtree 
       notifyFilter); 
       
    
}

Autobuild::~Autobuild() {
    FindCloseChangeNotification(mPimpl->dwChangeHandle);
}
  
void Autobuild::run() {      
   for(;;) {
       
       DWORD state = WaitForSingleObject(mPimpl->dwChangeHandle, INFINITE);
       if(state == WAIT_OBJECT_0) {

           double now = GetTickCount() / 1000.0;
           if( now - mPimpl->last < mPimpl->conf->ignore ) {
               // ignore ..
           } else {
               std::cout << "[autobuild] " << mPimpl->conf->command << std::endl; 
               system(mPimpl->conf->command.c_str());
               mPimpl->last = GetTickCount() / 1000.0;
           }
           
           
           FindNextChangeNotification(mPimpl->dwChangeHandle);
       }
   }
}

#endif
