# Input system

The input system manages external input devices like a mouse and a keyboard.

## Keyboard and mouse button input

Keyboard and mouse button input is done through a class called [`BinaryInput`](./../../lib/input/binary_input.h).

Binary input is an interface over a rebindable control scheme, with each having one or multiple asset-defined activators (keyboard or mouse buttons).

Binary inputs can be requested from the asset system:

```C++
#include "input/binary_input.h"
#include "managers/asset_manager.h"

const BinaryInput* jump_input = AssetManager::request<BinaryInput>("jump.keybind.xml");

bool still_jumping = jump_input->get_pushed();
bool just_jumped = jump_input->poll_pushed();
bool just_stopped_jumping = jump_input->poll_released();
```

`.keybind.xml` assets have the following format:

```XML
<keybind>
    <input code="KEY_SPACE"/>
    <input code="BTN_LEFT"/>
</keybind>
```

[Keyboard](#keyboard-input-codes) an [mouse](#mouse-input-codes) input codes can be found at the end of this document.

## Mouse input

Mouse movement input can be retrieved from the [`InputController`](./../../lib/input/input_controller.h) class.

```C++
#include "input/input_controller.h"

double mouse_pos_x = InputController::get_mouse_x();
double mouse_pos_y = InputController::get_mouse_y();

double mouse_shift_x = InputController::get_mouse_delta_x();
double mouse_shift_y = InputController::get_mouse_delta_y();
```

## Possible future development

It is possible that in the future the system will be replaced with an event-based system.

It is also possible that a text input system will be added.

## Mouse input codes

- `BTN_LEFT`
- `BTN_RIGHT`
- `BTN_MIDDLE`
- `BTN_4`
- `BTN_5`
- `BTN_6`
- `BTN_7`
- `BTN_8`

## Keyboard input codes

- `KEY_SPACE`
- `KEY_APOSTROPHE`
- `KEY_COMMA`
- `KEY_MINUS`
- `KEY_PERIOD`
- `KEY_SLASH`
- `KEY_0`
- `KEY_1`
- `KEY_2`
- `KEY_3`
- `KEY_4`
- `KEY_5`
- `KEY_6`
- `KEY_7`
- `KEY_8`
- `KEY_9`
- `KEY_SEMICOLON`
- `KEY_EQUAL`
- `KEY_A`
- `KEY_B`
- `KEY_C`
- `KEY_D`
- `KEY_E`
- `KEY_F`
- `KEY_G`
- `KEY_H`
- `KEY_I`
- `KEY_J`
- `KEY_K`
- `KEY_L`
- `KEY_M`
- `KEY_N`
- `KEY_O`
- `KEY_P`
- `KEY_Q`
- `KEY_R`
- `KEY_S`
- `KEY_T`
- `KEY_U`
- `KEY_V`
- `KEY_W`
- `KEY_X`
- `KEY_Y`
- `KEY_Z`
- `KEY_LEFT_BRACKET`
- `KEY_BACKSLASH`
- `KEY_RIGHT_BRACKET`
- `KEY_GRAVE_ACCENT`
- `KEY_WORLD_1`
- `KEY_WORLD_2`
- `KEY_ESCAPE`
- `KEY_ENTER`
- `KEY_TAB`
- `KEY_BACKSPACE`
- `KEY_INSERT`
- `KEY_DELETE`
- `KEY_RIGHT`
- `KEY_LEFT`
- `KEY_DOWN`
- `KEY_UP`
- `KEY_PAGE_UP`
- `KEY_PAGE_DOWN`
- `KEY_HOME`
- `KEY_END`
- `KEY_CAPS_LOCK`
- `KEY_SCROLL_LOCK`
- `KEY_NUM_LOCK`
- `KEY_PRINT_SCREEN`
- `KEY_PAUSE`
- `KEY_F1`
- `KEY_F2`
- `KEY_F3`
- `KEY_F4`
- `KEY_F5`
- `KEY_F6`
- `KEY_F7`
- `KEY_F8`
- `KEY_F9`
- `KEY_F10`
- `KEY_F11`
- `KEY_F12`
- `KEY_F13`
- `KEY_F14`
- `KEY_F15`
- `KEY_F16`
- `KEY_F17`
- `KEY_F18`
- `KEY_F19`
- `KEY_F20`
- `KEY_F21`
- `KEY_F22`
- `KEY_F23`
- `KEY_F24`
- `KEY_F25`
- `KEY_KP_0`
- `KEY_KP_1`
- `KEY_KP_2`
- `KEY_KP_3`
- `KEY_KP_4`
- `KEY_KP_5`
- `KEY_KP_6`
- `KEY_KP_7`
- `KEY_KP_8`
- `KEY_KP_9`
- `KEY_KP_DECIMAL`
- `KEY_KP_DIVIDE`
- `KEY_KP_MULTIPLY`
- `KEY_KP_SUBTRACT`
- `KEY_KP_ADD`
- `KEY_KP_ENTER`
- `KEY_KP_EQUAL`
- `KEY_LEFT_SHIFT`
- `KEY_LEFT_CONTROL`
- `KEY_LEFT_ALT`
- `KEY_LEFT_SUPER`
- `KEY_RIGHT_SHIFT`
- `KEY_RIGHT_CONTROL`
- `KEY_RIGHT_ALT`
- `KEY_RIGHT_SUPER`
- `KEY_MENU`
