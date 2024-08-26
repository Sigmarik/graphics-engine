# Physics system

The physics system manages the physics representation of a scene and implements basic physics simulations.

## Limitations

The physics system only implements one-coupling simulations, meaning that only level-to-object interactions are being calculated.
Objects can be of any shape and behavior complexity, but level geometry is bound to consist of boxes.
This may sound limiting (mostly because it is), but for many implications this is more than enough.

It is possible that static triangle primitives will be implemented in the future, allowing for custom-shaped static colliders.

## Adding a physical representation to a [component](./../logics/CORE.md)

Physics representations can be added directly to the component as class members.

They do not require registration or wrappers of any kind.

```C++
struct MyComponent : public SceneComponent {
   private:
    PhysicsRepresentation physical_representation_;
};
```

The only mandatory representation-to-component interaction are update calls at physics ticks.

```C++
void MyComponent::phys_tick(double delta_time) {
    // Component physics update
    physical_representation_.tick(get_scene().get_collision(), delta_time);

    //  . . .
}
```

## Custom physics components

Every physical representation type should be derived from the `PhysObject` class and implement the `tick` method.

```C++
#include "physics/phys_object.h"

struct MyPhysicalObject : public PhysObject {

    virtual void tick(const LevelGeometry& level, double delta_time) override;

   private:
    
    // Position, velocity, collision, and other behavior-related fields
};
```

### Custom collision

It is often the case that physics components require a collision for scene interaction.

Collision is an object that defines object's physical shape, while the object itself defines its response to intersections.

```C++
#include "physics/collider.h"

struct MyCollision : public DynamicCollider {
    // Mandatory collision overrides

    // Get the bounding box of the object
    virtual Box get_bounding_box() const override;

    // Get object's intersection with a box collider
    virtual Intersection intersect_box(const BoxCollider& box) const override;
};
```
