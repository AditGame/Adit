#include "SyncedMotionState.h"

#include <osgbCollision\Utils.h>

#include "Entities/Entity.h"

SyncedMotionState::SyncedMotionState(const btTransform &initialPosition, Entity* ent, osg::Vec3d centerOfMass, bool syncRotation) : 
	_syncRotation(syncRotation),
	_com(centerOfMass)
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
	pos -= _com;
	_entity->setPosition(pos, false);

}
