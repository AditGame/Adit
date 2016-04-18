#include "SyncedMotionState.h"

#include <osgbCollision\Utils.h>

#include "Entity.h"

SyncedMotionState::SyncedMotionState(const btTransform &initialPosition, Entity* ent, bool syncRotation) : _syncRotation(syncRotation)
{
	_entity = ent;
	_initialPosition = initialPosition;
}


SyncedMotionState::~SyncedMotionState()
{
}

void SyncedMotionState::setWorldTransform(const btTransform & worldTrans)
{
	if (_entity == nullptr)
		return; // silently return before we set a node

	if (_syncRotation)
	{
		btQuaternion rot = worldTrans.getRotation();
		_entity->getBaseNode()->setAttitude(osg::Quat(rot.w(), rot.x(), rot.y(), rot.z()));
	}
	
	osg::Vec3f pos = osgbCollision::asOsgVec3(worldTrans.getOrigin());
	_entity->setPosition(pos, false);

}
