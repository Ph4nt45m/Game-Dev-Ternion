// This include:
#include "inventory.h"

// Local includes:
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"
#include "inputsystem.h"
#include "inventorybutton.h"

// Library includes:

Inventory::Inventory(Renderer& renderer)
{
	// Create Inventory Button
	float inventoryButtonX = 100;
	float inventoryButtonY = 100;
	float inventoryButtonScale = 0.5f;

	m_pInventoryButton = new InventoryButton(renderer, inventoryButtonX, inventoryButtonY, inventoryButtonScale);
}

Inventory::~Inventory()
{

}

void Inventory::Process(float deltaTime, InputSystem& inputSystem)
{
	m_pInventoryButton->Update(deltaTime, inputSystem);
	
}

void Inventory::Draw(Renderer& renderer)
{
	m_pInventoryButton->Draw(renderer);
}
