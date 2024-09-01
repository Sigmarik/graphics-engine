# Graphics system

The graphics system is responsible for managing visual representations of objects on a scene, transmitting graphical information between render stages and maintaining the correct rendering order.

The core of the implementation can be found [here](./../../lib/graphics/objects/scene.h).

## Visual representation of an object

Each entity in a game has three representations: logical, visual and physical. Visual representation of an object is an instance of a class that is a part of the graphics system.

A visual representation should be able to do two things:
- draw itself on the GBuffers,
- receive information from the other representation of the object on updates.

The first requirement is satisfied by all visual representations being derived from the `Renderable` class, which has a pure virtual `render()` method.

The second requirement can be satisfied by adding appropriate parameter setters to the object.

## Adding a visual representation to a [component](./../logics/CORE.md)

It is recommended to get familiar with the [logics system](./../logics/CORE.md) of the framework before trying to understand this paragraph.

To add a visual representation for a component, create a private `Visual<RequiredVisualType>` field and call `get_scene().get_renderer().track_object(visual_component_)` for every representation in the `begin_play` override method.

```C++
struct Component : public SceneComponent {
    Component(): visual_(/* exactly as if it was a direct SomeRenderable() constructor */) {}

   protected:
    virtual void begin_play(Scene& scene) {
        SceneComponent::begin_play(scene);

        get_scene().get_renderer().track_object(visual_);
    }

   private:
    Visual<SomeRenderable> visual_;
};
```

Do not forget to update the representation in the graphics update method.

After that the component should have a visual representation that is visible in-game.

## Deferred rendering

The framework's render system uses the [deferred rendering technique](https://en.wikipedia.org/wiki/Deferred_shading) for every object.

It means that rendering is done by drawing each object's geometric data on a series of GBuffers (textures which specific color interpretation), and then doing a number of light passes over the entire canvas.

### GBuffers

The framework uses four GBuffers. The exact way of working with them will be described in the [guide to creating custom materials](./CUSTOM_MATERIALS.md), but for now it is important to understand which information is accessible to the objects.

- `GB_COLOR` - the color of the surface (a.k.a. albedo),
- `GB_NORM_DEPTH` - the surface normal direction and the distance at which the surface is located from the camera,
- `GB_SURFACE` - physical surface parameters, like roughness, shininess, "openness" (used for AO approximation) and "decalability" of the surface,
- `GB_FINAL` - the final image that is shown to the user.

## Render passes

Not all objects can be rendered simultaneously. Opaque objects need to be displayed before getting splashed with decals, who, in turn, need to transmit their data before the light pass.

Therefore rendering is done in four passes.

- `RP_INITIAL` - solid level geometry, like models and solid particles,
- `RP_DECAL` - decals, like paint splashes, graffiti e.t.c.,
- `RP_LIGHT` - all kinds of lights,
- `RP_POSTPROCESSING` - postprocessing effects like vignette, color correction and on-screen effects.

Each object is allowed to be rendered on however many passes it needs, but most objects draw themselves only on a single pass.

## Common renderable objects

[Here](./COMMON_RENDERABLES.md) is a list of all framework-defined renderable components.

## World-owned visuals

It is often a good idea to make global visual objects like ambient lights and postprocessors belong to the level itself, and the framework allows that.

```C++
struct MyWorld : public Scene {
    MyWorld(): Scene(/* . . . */), theme_(/* . . . */) {
        get_renderer().track_object(theme_);
    }

   private:
    Visual<Postprocessor> theme_;
};
```

## Creating custom renderable objects

To learn how to create specialized renderable objects read [this guide](./CUSTOM_RENDERABLES.md).
