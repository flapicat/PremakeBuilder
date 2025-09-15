#include "App.h"

int main() 
{
    Log::init();
    App* app = new App;
    app->Start();
    app->Update();
    app->ShutDown();
    delete app;
}
