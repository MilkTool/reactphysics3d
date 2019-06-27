/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2018 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

#ifndef REACTPHYSICS3D_COLLISION_BODY_H
#define REACTPHYSICS3D_COLLISION_BODY_H

// Libraries
#include <cassert>
#include "Body.h"
#include "collision/shapes/AABB.h"
#include "mathematics/Transform.h"
#include "configuration.h"

/// Namespace reactphysics3d
namespace reactphysics3d {

// Declarations
struct ContactManifoldListElement;
class ProxyShape;
class CollisionWorld;
class CollisionShape;
struct RaycastInfo;
class PoolAllocator;
class Profiler;

/// Enumeration for the type of a body
/// STATIC : A static body has infinite mass, zero velocity but the position can be
///          changed manually. A static body does not collide with other static or kinematic bodies.
/// KINEMATIC : A kinematic body has infinite mass, the velocity can be changed manually and its
///             position is computed by the physics engine. A kinematic body does not collide with
///             other static or kinematic bodies.
/// DYNAMIC : A dynamic body has non-zero mass, non-zero velocity determined by forces and its
///           position is determined by the physics engine. A dynamic body can collide with other
///           dynamic, static or kinematic bodies.
enum class BodyType {STATIC, KINEMATIC, DYNAMIC};

// Class CollisionBody
/**
 * This class represents a body that is able to collide with others
 * bodies. This class inherits from the Body class.
 */
class CollisionBody : public Body {

    protected :

        // -------------------- Attributes -------------------- //

        // TODO : Move this into the dynamics components
        /// Type of body (static, kinematic or dynamic)
        BodyType mType;

        /// Reference to the world the body belongs to
        CollisionWorld& mWorld;

#ifdef IS_PROFILING_ACTIVE

		/// Pointer to the profiler
		Profiler* mProfiler;

#endif

        // -------------------- Methods -------------------- //

        /// Remove all the collision shapes
        void removeAllCollisionShapes();

        /// Update the broad-phase state for this body (because it has moved for instance)
        void updateBroadPhaseState() const;

        /// Ask the broad-phase to test again the collision shapes of the body for collision
        /// (as if the body has moved).
        void askForBroadPhaseCollisionCheck() const;

        /// Set the variable to know whether or not the body is sleeping
        virtual void setIsSleeping(bool isSleeping) override;

    public :

        // -------------------- Methods -------------------- //

        /// Constructor
        CollisionBody(CollisionWorld& world, Entity entity, bodyindex id);

        /// Destructor
        virtual ~CollisionBody() override;

        /// Deleted copy-constructor
        CollisionBody(const CollisionBody& body) = delete;

        /// Deleted assignment operator
        CollisionBody& operator=(const CollisionBody& body) = delete;

        /// Return the type of the body
        BodyType getType() const;

        /// Set the type of the body
        void setType(BodyType type);

        /// Set whether or not the body is active
        virtual void setIsActive(bool isActive) override;

        /// Return the current position and orientation
        const Transform& getTransform() const;

        /// Set the current position and orientation
        virtual void setTransform(const Transform& transform);

        /// Add a collision shape to the body.
        virtual ProxyShape* addCollisionShape(CollisionShape* collisionShape,
                                              const Transform& transform);

        /// Remove a collision shape from the body
        virtual void removeCollisionShape(ProxyShape *proxyShape);

        /// Return true if a point is inside the collision body
        bool testPointInside(const Vector3& worldPoint) const;

        /// Raycast method with feedback information
        bool raycast(const Ray& ray, RaycastInfo& raycastInfo);

        /// Test if the collision body overlaps with a given AABB
        bool testAABBOverlap(const AABB& worldAABB) const;

        /// Compute and return the AABB of the body by merging all proxy shapes AABBs
        AABB getAABB() const;

        /// Return a const pointer to a given proxy-shape of the body
        const ProxyShape* getProxyShape(uint proxyShapeIndex) const;

        /// Return a pointer to a given proxy-shape of the body
        ProxyShape* getProxyShape(uint proxyShapeIndex);

        /// Return the number of proxy-shapes associated with this body
        uint getNbProxyShapes() const;

        /// Return the world-space coordinates of a point given the local-space coordinates of the body
        Vector3 getWorldPoint(const Vector3& localPoint) const;

        /// Return the world-space vector of a vector given in local-space coordinates of the body
        Vector3 getWorldVector(const Vector3& localVector) const;

        /// Return the body local-space coordinates of a point given in the world-space coordinates
        Vector3 getLocalPoint(const Vector3& worldPoint) const;

        /// Return the body local-space coordinates of a vector given in the world-space coordinates
        Vector3 getLocalVector(const Vector3& worldVector) const;

#ifdef IS_PROFILING_ACTIVE

		/// Set the profiler
		virtual void setProfiler(Profiler* profiler);

#endif

        // -------------------- Friendship -------------------- //

        friend class CollisionWorld;
        friend class DynamicsWorld;
        friend class CollisionDetection;
        friend class BroadPhaseAlgorithm;
        friend class ConvexMeshShape;
        friend class ProxyShape;
};

// Return the type of the body
/**
 * @return the type of the body (STATIC, KINEMATIC, DYNAMIC)
 */
inline BodyType CollisionBody::getType() const {
    return mType;
}

/// Test if the collision body overlaps with a given AABB
/**
* @param worldAABB The AABB (in world-space coordinates) that will be used to test overlap
* @return True if the given AABB overlaps with the AABB of the collision body
*/
inline bool CollisionBody::testAABBOverlap(const AABB& worldAABB) const {
    return worldAABB.testCollision(getAABB());
}

#ifdef IS_PROFILING_ACTIVE

// Set the profiler
inline void CollisionBody::setProfiler(Profiler* profiler) {
	mProfiler = profiler;
}

#endif

}

#endif
