#include "Engine.h"

int main() {
    try {
        Engine engine(1280, 720, "Briar Engine");
        engine.Init();
        engine.Run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
