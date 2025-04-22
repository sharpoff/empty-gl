#include <iostream>
#include <engine.h>

int main()
{
    Engine engine;

    engine.init("Application", 1280, 720);
    try {
        engine.run();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}
