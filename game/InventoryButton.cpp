// This include:
#include "InventoryButton.h"

// Local includes:
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"
#include "button.h"
#include "inputsystem.h"

// Library includes:

InventoryButton::InventoryButton(Renderer& renderer, float x, float y, float scale)
	: Button(x, y)
{
	// Create Inventory Button
	m_pInventoryButtonSprite = renderer.CreateSprite("..\\source\\Sprites\\inventory\\inventorybutton.png");
	m_pInventoryButtonSprite->SetX(x);
	m_pInventoryButtonSprite->SetY(y);
	m_pInventoryButtonSprite->SetScale(scale);

	SetWidth(m_pInventoryButtonSprite->GetWidth());
	SetHeight(m_pInventoryButtonSprite->GetHeight());
}

InventoryButton::~InventoryButton()
{

}

bool InventoryButton::Initialise(Renderer& renderer)
{
	return true;
}

void InventoryButton::Update(float deltaTime, InputSystem& inputSystem)
{
	Button::Update(deltaTime, inputSystem);
	
	if (IsClicked())
	{
		LogManager::GetInstance().Log("Inventory Button Pressed");
	}

	if (IsReleased())
	{
		LogManager::GetInstance().Log("Inventory Button Released");
	}

}

void InventoryButton::Draw(Renderer& renderer)
{
	m_pInventoryButtonSprite->Draw(renderer, false, true);
}
