#include "InputSystem.h"

namespace nc
{
	const Uint8* InputSystem::SDL_GetKeyboardState(int* numkeys)
	{
		return nullptr;
	}

	InputSystem::eKeyState InputSystem::GetKeyState(int id)
	{
		return eKeyState();
	}
	
	bool InputSystem::IsKeyDown(int id)
	{
		return false;
	}

	bool InputSystem::IsPreviousKeyDown(int id)
	{
		return false;
	}

	void InputSystem::Startup()
	{
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(<pointer to numKeys>);
		keyboardState.<resize(? ? ? )>;
		std::copy(< https://www.includehelp.com/stl/how-to-copy-array-elements-to-a-vector.aspx>);
		prevKeyboardState = keyboardState;
	}

	void InputSystem::Shutdown()
	{
		//empty
	}

	void InputSystem::Update(float dt)
	{
		prevKeyboardState = keyboardState;

		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(nullptr);
		std::copy(< https://www.includehelp.com/stl/how-to-copy-array-elements-to-a-vector.aspx>);

		bool keyDown = <get key down>;
		bool prevKeyDown = <get previous key down>;

		eKeyState state = eKeyState::Idle;

		if (keyDown)
		{
			state = (prevKeyDown) ? < ? ? ? > : < ? ? ? >;
		}
		else
		{
			state = (prevKeyDown) ? < ? ? ? > : < ? ? ? >;
		}

		return state;

	}
}
