// This include:
#include "player.h"

// Local includes: 
#include "renderer.h"
#include "vector2.h"

// Library includes: 
#include <cassert>

Player::Player()
	: m_pStaticDeath(nullptr)
	, m_bDoubleJump(false)
	, m_bAnimateDeath(false)
	, m_bGodmode(false)
{

}

Player::~Player()
{
}

void Player::setPlayerJump()
{
	m_bDoubleJump = !m_bDoubleJump;
	m_bJumping = !m_bJumping;
}
