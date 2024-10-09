#ifndef INVENTORYBUTTON_H
#define INVENTORYBUTTON_H

// Local includes:
#include "button.h"

// Library includes:

// Forward Declarations:
class Renderer;
class Sprite;
class InputSystem;

// class declaration:
class InventoryButton : public Button
{
	// Member methods:
public:
	InventoryButton(Renderer& renderer, float x, float y, float scale);
	~InventoryButton();
	bool Initialise(Renderer& renderer) override;
	void Update(float deltaTime, InputSystem& inputSystem) override;
	void Draw(Renderer& renderer) override;
protected:

private:

	// Member data:
public:

protected:

private:
	Sprite* m_pInventoryButtonSprite;
};

#endif // !INVENTORYBUTTON_H

