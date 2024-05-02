// Depends on: collision.so
#ifndef PHYSICS
#define PHYSICS

#include "../types.hh"

struct PhysicsProps {
    std::vector<u32> *const physicsObjects;
    fx *const gravity, *const tvelocity;
};

#endif
