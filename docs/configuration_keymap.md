# Configuring your own keymap
To understand how to configure your keymap, you should first know what constitutes a keycode:

## Keycodes
A single keycode field is made up of four different pieces of information:

### The HID Keycode 
* this is what the key actually sends to the computer, e.g. 'a'
* These conform with the USB HID standard and can be viewed in hid\_keycodes.h

### The activation method
This is how the key is activated. When a key is active, it preforms its function, e.g. sending the letter 'a' 
or toggling the modifier "SHIFT". There are five different setting as shown below.

They key is registered as active upon: 
* 0 PRESS: press
* 1 MT\_HOLD: press and not releasing (holding the key) for a certain time
* 2 MT\_TAP: press and then release (active upon released)
* 3 DT\_DOUBLTAP: press, release and then pressed again within a certain time limit (doubletapping it)
(DOUBLETAP\_TIME\_LIMIT in KeyStates.h)
* 4 DT\_TAP: press, release and not pressing for DOUBLETAP\_TIME\_LIMIT

#### The difference between PRESS, MT\_TAP and DT\_TAP
PRESS is what everyone is used to and is the default mode. MT\_TAP is like press, but only activates when you 
release the key. DT\_TAP is like MT\_TAP, but only activates after a certain time after release.

The reasoning behind MT\_TAP and DT\_TAP is that you might want to have a key that does something when you press it and 
something different when you hold it (or doubletap it). If the "press" was registered using PRESS, then the key would activate even if you intend to hold it (or doubletap it), potentially activating twice. MT\_TAP and DT\_TAP are intended for use with MT\_HOLD and
DT\_DOUBLETAP respectively, so that the key only activates once if you hold it (or doubletap it) and once if you tap it.

#### Configuring the activation method
Please refer to the section on layers.

### The activation duration
This is when the key is actually sent:
* Momentary (default): as long as the key is active the keycode will be sent in every HID report to the computer
* Toggle (TG(<i>HID keycode</i>)): The keycode will be sent in every HID report to the computer, as long as a key with the same keycode isn't pressed. This works even if two physically different keys toggle the same keycode/modifier.
* Oneshot(OS(<i>HID keycode</i>)): The keycode will be sent with the next non-null report to the computer (next keypress), e.g. Shift key makes the next typed letter into a capital letter, even if shift isn't held

The activation durations can be all defined in the following way:
<pre>
    <i>DURATION</i>(<i>HID keycode</i>)
</pre>
e.g.
<pre>
    TG(KC_LSHIFT)
    OS(KC_LCTRL)
    MO(KC_A) //The MO(...) isn't necessary since it's the default
</pre>

These are part of the definition for a layer or a single key.

### The layer
This defines to which virtual layer a everything that defines a key belongs to (so the HID Keycode, activation method and activation duration). This is especially useful for people with smaller keyboards so that one key can have many different
functions.
e.g. There is a key on layer 0 which you can PRESS to send 'a', if you change to layer 1, you can PRESS the very same key to toggle shift (TG(KC_LSHIFT)).

## Configuration
### The default layer
A layer is a two dimensional array (a matrix) that corresponds to the physical layout of the keyboard. You can define and configure many different virtual layers, each of which can have different activation methods (PRESS, MT\_TAP, etc.).
The default layer is the layer which you will most often use. Generally this is the "QWERTY" layer with the PRESS activation method.
Here, you give the 
HID Keycode and optionally the activation duration for the default method on the default layer (currently PRESS and layer 0).
<pre>
    main_layer_t matrix {
    KEYMAP(
        <i>KEYCODE1</i>, <i>KEYCODE2</i>, <i>etc.</i>)
    };
</pre>

<i>Note: These constants are defined in keymap.h in the keymaps folder of the keyboard configuration files. </i>

## Further layers **WIP**
You can define layers in the same way as the default layer for any activation method and layer combo, 
e.g. PRESS on layer 1 or MT\_TAP for layer 0. Most of the time, you'll only need press layers unless you use other activation methods a lot. You will need to add a tuple ** people won't understand what a tuple is...** of this layer using the addLayers function.
<pre>
    layer_t <i>LAYER_NAME</i> {
        KEYMAP(<i>...</i>)
    };

    addLayers({std::make_tuple(<i>LAYER</i>, <i>METHOD</i>, <i>LAYER_NAME</i>), <i>further layers</i>});
</pre>

## Single keys
You'll most likely use this to configure keys with uncommon activation methods, e.g. DT\_DOUBLETAP. 
addActivation is a function taking the virtual layer of the key, the activation method (a number corresponding to the intended activation method, in the order that they are listed in the activation method section starting from 0 for PRESS).
<pre> 
    matrix[<i>row</i>][<i>col</i>].addActivation(<i>LAYER</i>, <i>METHOD</i>, <i>KEYCODE</i>);
</pre>
e.g.
<pre> 
    matrix[3][2].addActivation(_QWERTY, _MT_TAP, TG(KC_LSHIFT));
</pre>

<i>Note: in the above snippet _QWERTY is a macro for 0, which is the default layer, and _MT_TAP is a macro for 1, which
    is the activation layer for keys that should be registered as active upon MT\_TAP. </i>
