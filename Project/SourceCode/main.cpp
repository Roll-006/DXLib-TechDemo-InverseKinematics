#include "Manager/game_manager.hpp"

int WINAPI WinMain(
    _In_        const HINSTANCE hInstance,
    _In_opt_    const HINSTANCE hPrevInstance,
    _In_        const LPSTR     lpCmdLine,
    _In_        const int       nShowCmd)
{
    std::unique_ptr<GameManager> game_manager(std::make_unique<GameManager>());
    game_manager->Run();
    
    return 0;
}
