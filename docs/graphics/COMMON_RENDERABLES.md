# Common visual representation types

This is a guide to common visual representation types. It is a part of the [graphics system documentation](./CORE.md).

## [ComplexModel](./../../lib/graphics/objects/complex_model.h)

*Pass: `RP_INITIAL`, Input: -, Output: `GB_COLOR`, `GB_NORM_DEPTH`, `GB_SURFACE`.*

Complex model is a regular 3D model made of vertices, edges and faces, and colored using [materials](#material).

Complex models consist of parts ([models](#model)), each of which can have only one material.

### [Model](./../../lib/graphics/objects/model.h)

Model is a single piece of a complex model, which can only have a single material.

The class itself is more of a wrapper over a pointer to a [mesh](./../../lib/graphics/primitives/mesh.h) (the geometry of the model, which manages vertices and faces) and a material.

### [Material](./../../lib/graphics/objects/material.h)

Materials define how the objects are rendered.

A material is a combination of a shader and a set of uniforms of different types.

#### [Shader](./../../lib/graphics/primitives/shader.h)

[Shader](https://en.wikipedia.org/wiki/Shader) is a program which dictates the GPU how to draw objects defined in vertex, index and texture buffers.

The process of writing framework-suitable shaders is described [here](./CUSTOM_MATERIALS.md).

#### Uniforms

On the basic level, material uniforms are parameters of a material, like color, temperature, texture e.t.c.

Which uniforms have an effect on a specific material depends on the shaders used by the material.

#### Creating a material

Materials are defined in `.material.xml` assets.

The description of the material asset format can be found here [here](./CUSTOM_MATERIALS.md).

### Using complex models in a component

Complex models can either be imported from a `.obj` file using the framework's [asset system](../asset_system/CORE.md):

```C++
const ComplexModel* source = AssetManager::request<ComplexModel>("my_model.obj");
if (source) Visual<ComplexModel> visual(*source);
```

Or assembled part-by-part (it is recommended to use this approach only for procedural geometry):

```C++
Model part_a, part_b, part_c;

//  . . .

Visual<ComplexModel> visual{};

visual->add_part(part_a, /* part ID = */ "part_a");
visual->add_part(part_b, /* part ID = */ "part_b");
visual->add_part(part_c, /* part ID = */ "part_c");
```

### Creating a complex model asset

In order for a `.obj` model to be a valid `ComplexModel` asset, it needs to satisfy two requirements:

1. Parts of the models should have unique names,
2. Material names should match paths to the material assets that should be used for parts.

Names of the objects in the file are used as part identifiers in the `ComplexModel`, and material names indicate the importer which materials should be used for each part.

```obj
o RightPlane
#  . . .
usemtl assets/materials/red.material.xml
#  . . .

o LeftPlane
#  . . .
usemtl assets/materials/white.material.xml
#  . . .
```
*An example of a correctly defined complex model asset.*

```xml
<!-- assets/materials/red.material.xml -->

<material>
    . . .
</material>
```

```xml
<!-- assets/materials/white.material.xml -->

<material>
    . . .
</material>
```

## [Ambient light](./../../lib/graphics/objects/ambient_light.h)

*Pass: `RP_LIGHT`, Input: `GB_COLOR`, `GB_NORM_DEPTH`, `GB_SURFACE`, Output: `GB_FINAL`.*

The ambient light is a type of global light that shines on every surface with a fixed amount.

It takes into consideration the AO parameter of the surface.

As it the case with many lights and effects, its exact way of functioning is defined by a [material](./../../assets/materials/lights/ambient_light.material.xml).

## [Decal](./../../lib/graphics/objects/decal.h)

*Pass: `RP_DECAL`, Input: `GB_COLOR`, `GB_NORM_DEPTH`, `GB_SURFACE`, Output: `GB_COLOR`, `GB_NORM_DEPTH`, `GB_SURFACE`.*

Decal is an object which modifies the properties of the already formed surface.

Paint splashes, stains, footsteps, cracks and bullet holes are usually implemented using decals.

It is useful to think about decals as light sources which modify not the final GBuffer, but all the other buffers.

Due to variety of decal effects and shapes, they rely heavily on the material system to function.

This is an [example of a decal material](./../../assets/materials/decals/red_paint.material.xml).

### *Possible future development*

It is possible that in the future the decal render pass will be split into two: for transparent and opaque decals.

The motivation behind the change is that opaque decals can be rendered with a bit more efficient algorithm.

## [Point light](./../../lib/graphics/objects/point_light.h)

*Pass: `RP_LIGHT`, Input: `GB_COLOR`, `GB_NORM_DEPTH`, `GB_SURFACE`, Output: `GB_FINAL`.*

Point light is a type of light that shines from a single point in all directions.

The library implementation of the point light along with the point's light color and brightness also defines light radius and spread.

This is the [material used by the point light](./../../assets/materials/lights/point_light.material.xml).

## [Postprocessor](./../../lib/graphics/objects/postprocessor.h)

*Pass: `RP_POSTPROCESSING`, Input: `GB_FINAL`, Output: `GB_FINAL`.*

Postprocessor is a type of renderable object that draws on-screen stains, applies color correction, vignette, aberrations and other similar effects.

As is the case with decals, its function is heavily reliant on the material that is used.

Here is an example of a [postprocessing material](./../../assets/materials/postprocessing/contrast_vignette.material.xml).
