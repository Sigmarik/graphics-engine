# How to make a game using the framework

This guide covers the basics of game development with the framework, as well as gives references to more detailed feature descriptions and usage guides.

## Project creation

To create a new project make a repository using this project as a template.

The project will consist of two parts: the framework itself (located in the [./lib](./../../lib) directory) and a template project ([./src](./../../src)).

**Warning:** the procedure might be changed in the future to allow projects to change the framework version post-creation.

## Basic actions

There are a number of basic actions which need to be learned before attempting to do any modifications to the template.

### 1. Project compilation and test runs

The project can be build and ran with a `$ make run` command.

It will create a [./build](./../../build) directory where the executable file of the project will be located along with the asset folder. This folder is what players of the game should receive to play it.

### 2. File modification

Almost any file can be modified in the most straightforward manner, with the project accounting for the changes the next time the `$ make run` commend is ran.

The only exception to the rule are `.h` (header) files. As for now, the project has to be either completely rebuilt (`$ make rm`, `$ make run`) or have all the dependent `.o` files removed before the next build.

### 3. File deletion and creation

All `.c` and `.cpp` files taking part in the current build should be registered in the appropriate `.flist` files ([`src.flist`](./../../src.flist) or [`lib.flist`](./../../lib.flist)).

Other files can be used without explicit registration.

The system of file registration is here because it allows to create build-dependant executable lists in case the project has version-dependant or system-dependant files.

## Framework systems

Level and actor creation is described in the [Logics System](./../logics/CORE.md) documentation.

Although it is possible to get creative with the framework after this point, it is still recommended to get familiar with the other systems.

The [Asset System](./../asset_system/CORE.md) allows your game to use external files for models, levels, sounds, control bindings e.t.c.

The [Graphics System](./../graphics/CORE.md) implements the visual part of the game. Getting familiar with it allows the developer to create custom visual objects other than simple light sources and static models, available in the framework library by default.

The [Input System](./../input/CORE.md) is an essential system that allows the game to receive user's input.

The [Physics System](./../physics/CORE.md) allows game objects to have a robust physical representation.

## Debugging and testing

Although debugging is completely up to the user's interpretation, there are a few build-in tools for debugging.

### Sanitizer

Sanitizer can be enabled by including `$(CPP_SANITIZER_FLAGS)` into the `CPPFLAGS` definition in the [makefile](./../../makefile) of the project.

The project requires a reset (`$ make rm`) to be performed before it is ran again.

### GDB

There is a shortcut command `$ make debug` for GDB debugging.

GDB build flags are included into the flag list by default. To exclude them, remove `$(CPP_DEBUG_FLAGS)` from the `CPPFLAGS` definition.

### Logs

The project has a [logging system](./../../lib/logger/logger.h) in place. It is recommended to use logs instead of the console for debug output.

```C++
#include "logger/logger.h"

log_printf(STATUS_REPORTS, "status", "Loading the %lu-th chunk.\n", chunk_id);

log_printf(STATUS_REPORTS, "debug", "Requested chunk ID: %lu, hash: %0lX.\n", chunk_id, chunk_hash);

log_printf(WARNINGS, "warning", "The requested chunk is too big. Chunk size: %lu.\n", chunk_size);

log_printf(ERROR_REPORTS, "error", "Invalid chunk ID of %lu.\n", chunk_id);

// Ignore messages below a certain level of importance
set_logging_threshold(WARNINGS);

// Other log message types (levels of importance):
//  DATA_UPDATES - miscellaneous messages, less important than status reports,
//  STATUS_REPORTS - execution status reports,
//  AUTOMATIC_CORRECTIONS - user input overrides,
//  WARNINGS - warnings,
//  ERROR_REPORTS - non-fatal, recoverable (in the short run) errors,
//  TERMINATE_REPORTS - fatal irrecoverable errors,
//  ABSOLUTE_IMPORTANCE - messages that are more important than anything.
```

### Unit tests

The project also utilizes GTest for unit testing.

Test suits can be assembled and ran with the `$ make test` command.

Test suits are located in the [./gtest](./../../gtest/) directory along with the core test file.

To add a test suit, create a `.hpp` file with the suit and include it into the [gtest.cpp](./../../gtest/gtest.cpp) file.
