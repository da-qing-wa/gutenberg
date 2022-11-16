#include <iostream>

#include "BulletWorld.h"
#include "model/MovingObject.h"

int main(int argc, char* argv[])
{

	BulletWorld mWorld;

	btCollisionShape *groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
	MovingObject ground(0.0, groundShape, btVector3(0, -56, 0));
	mWorld.addRigidBody(ground.getBody());
	
	btCollisionShape *colShape = new btSphereShape(btScalar(1.));
	MovingObject colBody(1.0, colShape, btVector3(2, 10, 0));
	mWorld.addRigidBody(colBody.getBody());

	/// Do some simulation

	///-----stepsimulation_start-----
	for (int i = 0; i < 150; i++)
	{
		mWorld.step();
		
		ground.draw();
		colBody.draw();

		std::cout << std::endl;
	}

	delete groundShape;
	delete colShape;
}
