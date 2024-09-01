# Custom renderable objects

Any object inherited from the `Renderable` class (defined [here](./../../lib/graphics/objects/scene.h)) can act as a renderable object.

## Mandatory overrides

The main function any renderable object should implement is the `render()` method.

```C++
struct MyRenderable : public Renderable {
    virtual int render(const RenderInput& input,
                       const RenderBundle& bundle) const override;
};
```

Typically the render method does four consecutive things:
- Checks that the render stage specified in the `RenderInput` structure is appropriate for the object,
- Binds `RenderBungle` textures by calling the `bind_textures` method,
- Sets relevant [material uniforms](./CUSTOM_MATERIALS.md),
- Renders the appropriate pieces of geometry.

```C++
int AmbientLight::render(const RenderInput& input,
                         const RenderBundle& bundle) const {
    // Check if the current render pass matches the required one
    if (input.pass != RP_LIGHT) return 0;

    // Bind render attributes
    ambient_material.use();
    bundle.bind_textures(ambient_material.get_shader());
    ambient_material.get_shader().set_uniform_vec3("light_color", color_);

    // Render a plane that covers the entire screen
    FlatRenderer::render();

    // Notify the scene that something was rendered
    return 1;
}
```
*Example of the `render` method implementation based on the [implementation](./../../lib/graphics/objects/point_light.cpp) of the render method in the `AmbientLight` class.*

## Object transform

The `Renderable` class already provides its children with a world transform matrix.

It can be accessed either directly from the `world_transform_` matrix, or indirectly by calling the `get_object_matrix` method.

## Raw OpenGL calls

In case the object needs to make requests to OpenGL directly, it can do so in the `render` method along with the framework's abstractions.

## Possible future developments

It is possible that in the future direct `world_transform_` access will be deprecated, and integer return parameter of the `render` method will be replaced with a proper abstraction.
