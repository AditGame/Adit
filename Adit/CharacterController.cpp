#include "CharacterController.h"

#include "Player.h"
#include "GameEngine.h"
#include "PhysicsEngine.h"

CharacterController::CharacterController(Player* player) : 
	_player(player), 
	_currState(MotionStates::No_State)
{
}

void CharacterController::update()
{
	//if no player is set up exit early
	if (_player == nullptr)
		return;

	//change state
	{

		int nextState = _currState;

		//fire a ray down to see if we're on the ground
		osg::Vec3d pos = _player->getPosition();
		btVector3 btFrom(pos.x(), pos.y(), pos.z());
		btVector3 btTo(pos.x(), pos.y(), pos.z() - 5);
		btCollisionWorld::ClosestConvexResultCallback res(btFrom, btTo);
		res.m_collisionFilterMask = btBroadphaseProxy::StaticFilter;

		btConvexShape* shape = new btSphereShape(0.25);

		btTransform startTransform(btMatrix3x3(btQuaternion(0, 0, 0, 1)), btFrom);
		btTransform endTransform(btMatrix3x3(btQuaternion(0, 0, 0, 1)), btTo);

		GameEngine::inst().getPhysics()->getWorld()->convexSweepTest(shape, startTransform, endTransform, res);

		if (res.hasHit() && res.m_hitPointWorld.distance2(btFrom) < 1.2*1.2)
		{
			//on ground
			nextState = On_Ground;
		}
		else
		{
			//in air
			nextState = In_Air;
		}

		//if we're changing state, set it up
		if (_currState != nextState)
		{
			setUpState(nextState);
		}
	}

	// Update based on state
	updateState();

	// Clear the movement vectors
	_player->getMovement().clear();
}

void CharacterController::setUpState(int newState)
{
	switch(newState)
	{
	case On_Ground:
		std::cout << "On Ground" << std::endl;
		_player->getRigidBody()->setDamping(0, 0);
		_player->getRigidBody()->setFriction(0);
		break;
	case In_Air:
		std::cout << "In Air" << std::endl;
		_player->getRigidBody()->setDamping(0, 0);
		break;
	}

	_currState = newState;

}

void CharacterController::updateState()
{
	switch (_currState)
	{
	case On_Ground:
		{
			//on ground
			float x = _player->getMovement().forwardBack;
			float y = _player->getMovement().leftRight;
			float z = _player->getMovement().jump;
			if (x != 0.0 || y != 0.0 || z != 0.0)
			{
				if (x < 0.2 && x > -0.2)
					x = 0.0;

				if (y < 0.2 && y > -0.2)
					y = 0.0;

				float mag = 2;
				float angle = atan2(y, x);

				angle += _player->getRotation().x();
				angle += 4.71239;
				angle *= -1;

				btScalar initZ = _player->getRigidBody()->getLinearVelocity().x();
				if (z == 0.0)
					z = initZ;
				else
					z *= 5;

				_player->getRigidBody()->setLinearVelocity(btVector3(cos(angle)*mag, sin(angle)*mag, z));
			}
			else
			{
				_player->getRigidBody()->setLinearVelocity(btVector3(0,0,0));
			}
		}
		break;
	case In_Air:
	{

	}
		break;
	}
}
