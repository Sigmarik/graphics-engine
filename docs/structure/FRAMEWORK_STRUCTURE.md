# Framework structure

The framework is divided into five main systems that dictate the major part of what is happening inside a game.

## [1. Graphics](../graphics/CORE.md)

The graphics system keeps track of which object have to be shown on each frame, in which order they should be drown, and how to draw them.

It uses a fixed GBuffer-based rendering pipeline. It means that it has a fixed order in which different types of objects are rendered, and all information transmitted between stages is stored in GBuffers, which are like images the player sees on the screen, but with different geometric information stored inside each pixel.

This part of the framework has its own scene representation, which is synched with the actual scene representation regularly.

More information on the graphics system can be found [here](../graphics/CORE.md).

## [2. Physics](../physics/CORE.md)

The physics system takes care of physics simulations in the framework.

Similar to the graphics system, it also has its own scene representation.

More information on the physics system can be found [here](../graphics/CORE.md).

## [3. Logics](../logics/CORE.md)

The logics system synchronizes all other systems, as well as creates the high level of abstraction needed for gameplay programming.

It defined the scene the user works with, components that manage their physical and graphical appearance, and the way for the user to interact with the scene components.

More information on the logics system can be found [here](../logics/CORE.md)

## [4. Input](../input/CORE.md)

The input system manages the input of the user.

It reads raw mouse and keyboard input and transforms it into framework-usable actions and axises.

More information on the input system can be found [here](../input/CORE.md)

## [5. Assets](../asset_system/CORE.md)

The asset system manages the loading process of external files like meshes, textures, sounds, input bindings, scene descriptions, materials e.t.c.

This system is spread across the project as a set of independent importers which can be used to satisfy requests to assets of different types.

It also contains the [event routing script](../asset_system/EVENT_ROUTING.md) importer, even though it can also be classified as a part of the logics system.

More information on the asset system can be found [here](../asset_system/CORE.md)
