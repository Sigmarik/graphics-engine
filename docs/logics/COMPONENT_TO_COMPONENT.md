# Component-to-component interaction

It is often the case that [components](../../lib/logics/scene_component.h) have to interact with each other. For that, the [`Scene`](../../lib/logics/scene.h) class provides an interface for accessing components either by IDs or by their spacial properties.

## Direct component access

Each component has its own GUID, which uniquely identifies it amongst all other object on the scene.

```C++
GUID guid = component->get_guid();

SceneComponent* same_component = scene->get_component<SceneComponent>(guid);

CustomComponent* custom_component = scene->get_component<CustomComponent>(guid);

if (!custom_component) {
    // Component either does not exist or has a wrong class.
}
```

The `Scene` class guarantees that the reference returned by `get_component` method expires only if the referenced object is destroyed.

## Area requests

In case the component needs to interact with other components in a specific area (e.g. an explosive barrel damaging all objects within the radius of the explosion), such components can be accessed with the `for_each_component_in_area` method.

```C++
ComponentLayerId area_damageable{};
Box explosion_zone(explosive.get_position(), explosive.get_dimensions());

std::set<GUID> hit_objects =
    scene.get_components_in_area(explosion_zone,
                                 area_damageable,
                                 IntersectionType::OVERLAP);

for (...) {
    // Do something with potentially hit components
}

// OR perform something on the components directly

auto entity_damager = [](EntityBody& component){
    component.damage(10.0);
};


scene.for_each_component_in_area(explosion_zone,
                                 area_damageable,
                                 IntersectionType::OVERLAP,
                                 entity_damager);
```

### Intersection types

These are filters defining how component's boxes should intersect in order to be processed.

Intersection types are as follows:

- `IntersectionType::OVERLAP` - component's box intersects the requested area in any way,
- `IntersectionType::OVERSET` - component's box lies fully within the requested area,
- `IntersectionType::UNDERSET` - component's box covers the requested area.

### Component layers

Component layers add another layer of filtering to the request, e.g. only components registered on a specified layer will be affected by the request.

More on that in [component registration](#component-registration).

## Component registration

In order to make a component accessible through area requests, it needs to **have an accurate bounding box** and **be registered on all appropriate component layers**.

### Defining a bounding box

In order for the component to have an accurate bounding box it needs to

1. Override the `get_box` method and

```C++
public:
virtual Box get_box() const override;
```

2. Call the `update_box()` method every physics tick the box might change, or subscribe to automatic box updates with the `auto_update_box(true/false)` method.

```C++
void SomeComponent::begin_play(Scene& scene) {
    SceneComponent::begin_play(scene);

    //  . . .

    auto_update_box(true);
}

// --- OR ---

void SomeComponent::phys_tick(double delta_time) {
    bool has_moved = false;

    //  . . .

    if (has_moved) update_box();
}
```

### Specifying component layers

Component layer specification happens in the `begin_play` method.

```C++
const ComponentLayerId COMPONENT_LAYER_1{};
const ComponentLayerId COMPONENT_LAYER_2{};
const ComponentLayerId COMPONENT_LAYER_3{};
//  . . .

void SomeComponent::begin_play(Scene& scene) {
    SceneComponent::begin_play(scene);

    //  . . .

    use_positional_layer(COMPONENT_LAYER_1);
    use_positional_layer(COMPONENT_LAYER_2);
    use_positional_layer(COMPONENT_LAYER_3);
    //  . . .
}
```

Components may use multiple component layers. In the example above, the component will be visible on all three of the specified layers.
