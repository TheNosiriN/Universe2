#include "main.hpp"


Application app = {};


int main(int argc, char const *argv[]) {
    /// testing error app
    Graphics::SetErrorCallback([](HXRC_STATE state){
        if (state.Code == HXRC_FATAL){
            std::cout << "Graphics Engine: " << state.ErrorString << '\n';
            exit(0);
        }
    });

    Input::SetErrorCallback([](HXRC_STATE state){
        if (state.Code == HXRC_FATAL){
            std::cout << "Input Engine: " << state.ErrorString << '\n';
            exit(0);
        }
    });

    /// testing the error app
    HX_GRAPHICS_ERROR_PRINT("Error App is Working");
    HX_INPUT_ERROR_PRINT("Error App is Working");

    app.Init();
    while (app.Update());
    // for (int i=0; i<4; ++i){ app.Update(); }
    app.Destroy();

    HX_GRAPHICS_ERROR_PRINT("finished running safely");
    return 0;
}
