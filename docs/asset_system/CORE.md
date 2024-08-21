# Asset system

The asset system manages assets of the project. It helps components and other modules load, store and access different assets they might need.

The root of the system can be found [here](./../../lib/managers/asset_manager.h).

## Requesting an asset

To request as asset of type `T` from the file `file`, the function `AssetManager::request<T>(file)` can be used.

```C++
const Model* model = AssetManager::request<Model>("path/to/the/model.model.xml");

if (!model) {
    //! Do something if the model can not be loaded.
}
```

## Asset signatures

Files of different formats can have the same output asset type (like `.bmp`, `.png` and `.jpg` all producing an image), as well as the same file format can encode different types of objects (`.xml` files can encode all kinds of information, like material and level descriptors, as well as abstract data blocks).

To avoid the ambiguity, the system of file signatures can be used. A signature of a file is essentially the first substring of its name enclosed in dots (`.`).

```C++
signature("path/to/the/suzanne_model.model.xml") = "model";
signature("hello.brave.new.world") = "brave";
signature("chess_pawn.fbx") = "fbx";
signature("hello") = "hello"
```

The combination of the requested asset type and the signature extracted from the file name uniquely defines the way the asset will be imported. This is why many `XML` asset files have additional type specifications (signatures). It is just a specification of the file format on top of the standard `XML`.

## Asset buffering

Even if the same asset is requested multiple times, it only gets imported once (the first time it was requested).

It is possible to disable buffering for an asset, turning it rogue, which is done through one of the optional arguments of the request function.

The only difference between rogue and normal assets is that loaded rogue assets will never be looked at when answering a request of the same type.

```C++
//* Optional request argument will be described in the following paragraphs

AssetImporter::request<Texture>("texture1.png", /* signature = */ {}, /* flags = */ 0);
//  = <Texture at 0x1...>
AssetImporter::request<Texture>("texture1.png", /* signature = */ {}, /* flags = */ 0);
//  = <Texture at 0x1...>, the same as from the previous request

AssetImporter::request<Texture>("texture2.png", /* signature = */ {}, /* flags = */ AssetManager::RequestFlag::Rogue);
//  = <Texture at 0x2...>
AssetImporter::request<Texture>("texture2.png", /* signature = */ {}, /* flags = */ 0);
//  = <Texture at 0x3...>, not the same as from the previous request
AssetImporter::request<Texture>("texture2.png", /* signature = */ {}, /* flags = */ 0);
//  = <Texture at 0x3...>, the same as from the previous request
AssetImporter::request<Texture>("texture2.png", /* signature = */ {}, /* flags = */ AssetManager::RequestFlag::Rogue);
//  = <Texture at 0x3...>, the same as from the previous request
```

## Advanced requests

As shown in the previous snippet, the request function has two optional parameters.

### `signature`

The first parameter (`signature` of the optional string type (`std::optional<std::string>`)) can be used to specify the signature the importer should default to in case the given signature is now known.

```C++
AssetImporter::request<Texture>("owl.hoot", /* signature = */ {}, /* flags = */ 0);
//  = nullptr, WARNING: Failed to find an importer matching the signature "hoot" (type 0AC57B...)
AssetImporter::request<Texture>("owl.hoot", /* signature = */ "png", /* flags = */ 0);
//  = <Texture at 0x0...>, a beautiful picture of an owl
AssetImporter::request<Texture>("owl.bmp", /* signature = */ "png", /* flags = */ 0);
//  = <Texture at 0x1...>, a beautiful picture of an owl decoded from the `.bmp` file
//    (the `signature` parameter only has an effect if the manager failed to find a suitable importer)
```

### `flags`

The `unsigned flags` parameter is a bit mask of different import flags that can be applied.

Here is a list of them:

- `AssetManager::RequestFlag::Silent` - silence any errors/warnings that might occur,
- `AssetManager::RequestFlag::Reimport` - overwrite the buffered copy of the asset **(invalidates all the previous references the asset might have had)**,
- `AssetManager::RequestFlag::Rogue` - do not register the asset in the buffer.

## XML requests

Sometimes (usually inside of other importers) it is useful to request assets directly from `tinyxml2::XMLElement` elements.

In this case, the same `AssetManager::request` function can be used.

While having the same set of parameters, this function has some differences.

First, it treats the name of the XML element as the signature of the asset. This means that the signature of the asset will be read from it.

```C++
tinyxml2::XMLElement& element = ...;

AssetManager::request<Model>(element);
```

```XML
<model>
    . . .
</model>
```
*In this example, the string `"model"` will be used as the signature of the asset.*

Second, it considers every asset do be a rogue asset, unless the second optional parameter `handle` is specified. In this case, `handle` is the name of the asset in the registry of assets.

```C++
AssetManager::request<Model>(element, /* handle = */ "suzanne.model.xml", /* flags = */ 0);
//  = <Model at 0x1...>
AssetManager::request<Model>("suzanne.model.xml");
//  = <Model at 0x1...>, the same model that was imported on the previous line.
```

## Custom importers

The process of registering custom asset importers, allowing for requests for previously unknown asset types, is described in the [separate file](./CUSTOM_IMPORTERS.md).

In short, it is possible to add custom asset types to make them accessible through the `AssetManager::request` function.

## Special thanks

Special thanks to [MeerkatBoss](https://github.com/MeerkatBoss) for helping design the asset system.
