# Gamepad and Event system Implementation

##Gamepad:

First of all for the Gamepad implementation with SDL we need to inicialize the SDL_INIT_GAMECONTROLLER.
After this we must search for the number of gamepads or joysticks connected to the PC using SDL_NumJoysticks() and SDL_IsGameController(int). If SDL_IsGameController return true the evice is a gamepad and we have to open it with SDL_GameControllerOpen()

For each gamepad opened we must close it using SDL_GameControllerClose() also the SDL_INIT_GAMECONTROLLER has to be removed using SDL_QuitSubSystem()



