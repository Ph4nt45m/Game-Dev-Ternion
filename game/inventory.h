#ifndef INVENTORY_H
#define INVENTORY_H

// Local includes:

// Library includes:

// Forward Declarations:
class Renderer;
class Sprite;
class Button;
class InputSystem;

// class declaration:
class Inventory
{
	// Member methods:
public:
	Inventory(Renderer& renderer);
	~Inventory();
	void Process(float deltaTime, InputSystem& inputSystem);
	void Draw(Renderer& renderer);
protected:

private:

	// Member data:
public:

protected:

private:
	Sprite* m_pInventoryButtonSprite;
	Button* m_pInventoryButton;
};

#endif // !INVENTORY_H

