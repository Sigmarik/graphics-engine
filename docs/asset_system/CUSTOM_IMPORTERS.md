# Custom importers

This guide describes the process of creation of custom importers.

It is recommended to read the [general guide to asset system](./CORE.md) to understand the interface which will be referenced here.

## What is an importer

When processing a request for an asset the `AssetManager` reads the signature of the requested asset and combines it with the type of the asset to form the importer identifier, by which it tries to find a suitable importer which would do the importing itself.

```C++
// Draft of the asset request function
AssetManager::request<AssetType>(string name, ...) {
    // Find a suitable importer from a list of importers
    importer = importers[{type_hash(AssetType), signature(name)}];

    // Use the importer to import the asset
    return importer(name);
}
```
*The actual function implementation can be found [here](./../../lib/managers/_am_request.hpp).*

Consequently, an importer is a function that takes parameters passed by the `AssetManager` and transforms them into a pointer to the imported asset.

## Creation and registration

To define an importer create a separate `.cpp` file and include it into the [`src.flist`](./../../src.flist) file list, as you would with every other `.cpp` file.

Then include the [`lib/managers/importer.h`](./../../lib/managers/importer.h) header and use the `IMPORTER` macro to declare and define the importer with an appropriate type and signature.

```C++
// my_class_importer.cpp

#include "managers/importer.h"

// MyClass header
#include "my_class.h"

// The `IMPORTER` macro functions like a function declaration.
//  = Asset<MyClass>* import(const std::string& path, AssetManager::RequestFlags flags)
IMPORTER(MyClass, "myclass") {
    
    //  . . .

    return new Asset<MyClass>(/* arguments suitable for the MyClass constructor */);
}
```
*Importer template for assets of type `MyClass` with `.myclass.` signature.*

After writing the importer, make sure it returns a pointer to the asset allocated by the `new` operator.

### Input

Two parameters can be used inside an importer.

- `path` is a constant `std::string`, which is the exact same string that is passed into the `AssetManager::request` function,
- `flags` is the same `unsigned int` bit mask used by the `AssetManager::request` function.

### Output

An importer should return a pointer to the `Asset<>` template instance of your class created by the `new` operator.

`Asset<T>` is a simple wrapper that controls the lifetime of the asset.

It copies the constructor of the wrapped type.
The underlying object can be accessed through the `content` field.

```C++
IMPORTER(Type, "signature") {
    Asset<Type>* asset = new Asset<Type>(/* . . . */);
    if (!asset) return nullptr;

    asset->do_something();
    asset->field += 10;

    return asset;
}
```

`nullptr` should be returned in case of failure to import the asset.

## XML importers

XML importers are defined in a similar way to regular importers.

```C++
// my_class_importer.cpp

#include "managers/importer.h"

// MyClass header
#include "my_class.h"

//  = Asset<MyClass>* import(const tinyxml2::XMLElement& data, AssetManager::RequestFlags flags)
XML_IMPORTER(MyClass, "myclass") {

    //  . . .

    return new Asset<MyClass>(/* . . . */);
}
```

The only difference is that instead of `path` the `data` variable of type const `tinyxml2::XMLElement&` is accessible.
It refers to the root of the requested element.

```XML
<myclass>  <-- `data`
    . . .
</myclass>
```

## Combined importers

It is often convenient to create XML importers in pairs: one is the main XML importer, and the other is a regular importer that parses the file as XML and feeds the root to the former importer.

The `XML_BASED_IMPORTER` macro does exactly that.

```C++
// my_class_importer.cpp

#include "managers/importer.h"

// MyClass header
#include "my_class.h"

//  = Asset<MyClass>* import(const tinyxml2::XMLElement& data, AssetManager::RequestFlags flags)
XML_BASED_IMPORTER(MyClass, "myclass") {

    // Same as in the XML_IMPORTER
    //  . . .

    return new Asset<MyClass>(/* . . . */);
}
```

## Direct importer calls

Even though it is recommended to use the `AssetManager::request` function even inside of other importers (as many library importers actually do), there still is a more direct way to recursively call another importer.

```C++
IMPORTER(...) {
    
    //  . . .

    // The recommended way of recursively calling the other importer
    const OtherAsset* other = AssetManager::request<OtherAsset>(other_path, "other_signature", flags);

    //  . . .

    // A direct call of the other importer
    const Asset<OtherAsset>* other = AssetImporter<OtherAsset, "other_signature">::import(other_path, flags);

    //  . . .

    // A direct call of the other XML importer
    const Asset<OtherAsset>* other = XMLAssetImporter<OtherAsset, "other_signature">::import(some_data, flags);
}
```

## SIO warning

**NEVER** request assets from statically constructed objects (e.g. objects which constructor is called before the `main` function).

It can screw the static initialization order and make some importers invisible at the moment of the request.

But, if the request needs to be done as a part of the static initialization, make sure it happens after the importer is initialized. For most compilers, this can be achieved by listing the file with the request after the importer's `.cpp` file in [`src.flist`](./../../src.flist).

This is important because importers are registered by executing code during static initialization, meaning, that if the request requiring the importer happens before the importer is registered, the importer will appear non-existent.

Importers actually notify when they are registered, so if the message confirming the registration of the importer appears after the `AssetManager`-produced `error`-s in logs, that probably signals the failure on the SIO part.
