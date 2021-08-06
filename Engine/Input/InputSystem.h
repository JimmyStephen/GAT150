#pragma once
#include "Framework/System.h"
#include <iostream>
#include <vector>

namespace nc
{
	class InputSystem : public System
	{
	public:

		enum class eKeyState
		{
			Idle,
			Pressed,
			Held,
			Release
		};

		const Uint8* SDL_GetKeyboardState(int* numkeys);

		eKeyState GetKeyState(int id);
		bool IsKeyDown(int id);
		bool IsPreviousKeyDown(int id);

		// Inherited via System
		virtual void Startup() override;

		virtual void Shutdown() override;

		virtual void Update(float dt) override;


	private:
		std::vector<Uint8> keyboardState;
		std::vector<Uint8> prevKeyboardState;
		//std::vector<std::uint8_t> keyboardState;
		//std::vector<std::uint8_t> prevKeyboardState;
		int numKeys;
	};
}