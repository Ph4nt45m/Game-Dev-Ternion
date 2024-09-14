// This include: 
#include "entity.h"

// Local includes: 
#include "renderer.h"
#include "vector2.h"

// Library includes: 
#include <cassert>

// Static members:
int Entity::sm_iNumSegments = 0;
float Entity::sm_fBoundaryWidth = 0.0f;
float Entity::sm_fBoundaryHeight = 0.0f;
float Entity::sm_fSegmentWidth = 0.0f;
bool Entity::sm_bCameraCentered = false;
bool Entity::sm_bTerrainMoving = false;

Entity::Entity()
	: m_sMotionKeyStates{ BS_NEUTRAL, BS_NEUTRAL, BS_NEUTRAL, BS_NEUTRAL, BS_NEUTRAL, BS_NEUTRAL }
    , m_sKeyboardMotions{ MOTION_NONE, MOTION_NONE, MOTION_NONE, MOTION_NONE }
    , m_pSprSpriteBody(0)
    , m_bFlipHorizontally(false)
    , m_bMovingX(false)
    , m_bMovingY(false)
    , m_bJumping(false)
    , m_bSlash(false)
    , m_bShoot(false)
    , m_bProjectile(false)
    , m_bAlive(false)
    , m_iFacingDirection(0)
    , m_fScale(1.0f)
    , m_fScaleChangeRate(0.0f)
    , m_fScaleMin(0.0f)
    , m_fScaleMax(0.0f)
{

}
Entity::~Entity()
{

}

bool
Entity::Initialise(Renderer& renderer)
{
	return true;
}
