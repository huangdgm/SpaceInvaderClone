// COMP710 GP 2D Framework 2017

// This include:
#include "playership.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"

// Library includes:
#include <cassert>
#include <Windows.h>
#include <XInput.h>
#include <iostream>

// Ensure the constructor chains-up to the super class constructor.
PlayerShip::PlayerShip()
: Entity()
{

}

PlayerShip::~PlayerShip()
{

}

void
PlayerShip::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);
	this->SetPosition(384, 568);
}

//void
//PlayerShip::SetHorizontalVelocity(float x)
//{
//
//}

void
PlayerShip::Process(float deltaTime)
{
	// Ensure the process method chains-up to the super class process method.
	Entity::Process(deltaTime);

	// W03.1: Generic position update, based upon velocity (and time).
	m_x += m_velocityX * deltaTime;
	m_y += m_velocityY * deltaTime;

	//DWORD dwResult;
	//int controllerID = -1;	// To store the controller id that is connected. Assuming only one controller connected.
	//XINPUT_STATE state;		// Store the state of the gamepad.

	//for (DWORD i = 0; i < XUSER_MAX_COUNT && controllerID == -1; i++)
	//{
	//	// Fills a block of memory with zeros.
	//	ZeroMemory(&state, sizeof(XINPUT_STATE));

	//	// Simply get the state of the controller from XInput.
	//	dwResult = XInputGetState(i, &state);

	//	if (dwResult == ERROR_SUCCESS)
	//	{
	//		// Controller is connected
	//		controllerID = i;
	//		std::cout << "The controller is connected on port " << i << std::endl;
	//	}
	//	else
	//	{
	//		// Controller is not connected 
	//	}
	//}

	//bool DPAD_LEFT_PRESSED = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0);
	//bool DPAD_RIGHT_PRESSED = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);
	//bool A_BUTTON_PRESSED = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);

	// W03.1: Boundary checking and position capping.
	if (m_x <= 0)
	{
		m_x = 0;
	}
	else if (m_x >= 800 - 32)
	{
		m_x = 800 - 32;
	}

	if (m_y <= 0)
	{
		m_y = 0;
	}
	else if (m_y >= 600 - 32)
	{
		m_y = 600 - 32;
	}
}