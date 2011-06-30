#include "CocoaPlatform.h"
#include "../Debug.h"

using namespace Monocle;

//  Taken from SDL scancode table, many keys inactive

static KeyCode darwin_scancode_table[] = {
	/*   0 */   KEY_A,
	/*   1 */   KEY_S,
	/*   2 */   KEY_D,
	/*   3 */   KEY_F,
	/*   4 */   KEY_H,
	/*   5 */   KEY_G,
	/*   6 */   KEY_Z,
	/*   7 */   KEY_X,
	/*   8 */   KEY_C,
	/*   9 */   KEY_V,
	/*  10 */   KEY_UNDEFINED, // KEY_NONUSBACKSLASH, /* KEY_NONUSBACKSLASH on ANSI and JIS keyboards (if this key would exist there), KEY_GRAVE on ISO. (The USB keyboard driver actually translates these usage codes to different virtual key codes depending on whether the keyboard is ISO/ANSI/JIS. That's why you have to help it identify the keyboard type when you plug in a PC USB keyboard. It's a historical thing - ADB keyboards are wired this way.) */
	/*  11 */   KEY_B,
	/*  12 */   KEY_Q,
	/*  13 */   KEY_W,
	/*  14 */   KEY_E,
	/*  15 */   KEY_R,
	/*  16 */   KEY_Y,
	/*  17 */   KEY_T,
	/*  18 */   KEY_1,
	/*  19 */   KEY_2,
	/*  20 */   KEY_3,
	/*  21 */   KEY_4,
	/*  22 */   KEY_6,
	/*  23 */   KEY_5,
	/*  24 */   KEY_EQUALS,
	/*  25 */   KEY_9,
	/*  26 */   KEY_7,
	/*  27 */   KEY_MINUS,
	/*  28 */   KEY_8,
	/*  29 */   KEY_0,
	/*  30 */   KEY_RIGHTBRACKET,
	/*  31 */   KEY_O,
	/*  32 */   KEY_U,
	/*  33 */   KEY_LEFTBRACKET,
	/*  34 */   KEY_I,
	/*  35 */   KEY_P,
	/*  36 */   KEY_RETURN,
	/*  37 */   KEY_L,
	/*  38 */   KEY_J,
	/*  39 */   KEY_UNDEFINED, // KEY_APOSTROPHE,
	/*  40 */   KEY_K,
	/*  41 */   KEY_SEMICOLON,
	/*  42 */   KEY_BACKSLASH,
	/*  43 */   KEY_COMMA,
	/*  44 */   KEY_SLASH,
	/*  45 */   KEY_N,
	/*  46 */   KEY_M,
	/*  47 */   KEY_PERIOD,
	/*  48 */   KEY_TAB,
	/*  49 */   KEY_SPACE,
	/*  50 */   KEY_UNDEFINED, // KEY_GRAVE, /* KEY_GRAVE on ANSI and JIS keyboards, KEY_NONUSBACKSLASH on ISO (see comment about virtual key code 10 above) */
	/*  51 */   KEY_UNDEFINED, // KEY_BACKSPACE,
	/*  52 */   KEY_UNDEFINED, // KEY_KP_ENTER, /* keyboard enter on portables */
	/*  53 */   KEY_ESCAPE,
	/*  54 */   KEY_UNDEFINED, // KEY_RGUI,
	/*  55 */   KEY_UNDEFINED, // KEY_LGUI,
	/*  56 */   KEY_LSHIFT,
	/*  57 */   KEY_CAPSLOCK,
	/*  58 */   KEY_LALT,
	/*  59 */   KEY_LCTRL,
	/*  60 */   KEY_RSHIFT,
	/*  61 */   KEY_RALT,
	/*  62 */   KEY_RCTRL,
	/*  63 */   KEY_UNDEFINED, // KEY_RGUI, /* fn on portables, acts as a hardware-level modifier already, so we don't generate events for it, also XK_Meta_R */
	/*  64 */   KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  65 */   KEY_KP_PERIOD,
	/*  66 */   KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  67 */   KEY_KP_MULTIPLY,
	/*  68 */   KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  69 */   KEY_KP_PLUS,
	/*  70 */   KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  71 */   KEY_UNDEFINED, // KEY_NUMLOCKCLEAR,
	/*  72 */   KEY_UNDEFINED, // KEY_VOLUMEUP,
	/*  73 */   KEY_UNDEFINED, // KEY_VOLUMEDOWN,
	/*  74 */   KEY_UNDEFINED, // KEY_MUTE,
	/*  75 */   KEY_KP_DIVIDE,
	/*  76 */   KEY_UNDEFINED, // KEY_KP_ENTER, /* keypad enter on external keyboards, fn-return on portables */
	/*  77 */   KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  78 */   KEY_KP_MINUS,
	/*  79 */   KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  80 */   KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  81 */   KEY_UNDEFINED, // KEY_KP_EQUALS,
	/*  82 */   KEY_UNDEFINED, // KEY_KP_0,
	/*  83 */   KEY_UNDEFINED, // KEY_KP_1,
	/*  84 */   KEY_UNDEFINED, // KEY_KP_2,
	/*  85 */   KEY_UNDEFINED, // KEY_KP_3,
	/*  86 */   KEY_UNDEFINED, // KEY_KP_4,
	/*  87 */   KEY_UNDEFINED, // KEY_KP_5,
	/*  88 */   KEY_UNDEFINED, // KEY_KP_6,
	/*  89 */   KEY_UNDEFINED, // KEY_KP_7,
	/*  90 */   KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  91 */   KEY_UNDEFINED, // KEY_KP_8,
	/*  92 */   KEY_UNDEFINED, // KEY_KP_9,
	/*  93 */   KEY_UNDEFINED, // KEY_INTERNATIONAL3, /* Cosmo_USB2ADB.c says "Yen (JIS)" */
	/*  94 */   KEY_UNDEFINED, // KEY_INTERNATIONAL1, /* Cosmo_USB2ADB.c says "Ro (JIS)" */
	/*  95 */   KEY_UNDEFINED, // KEY_KP_COMMA, /* Cosmo_USB2ADB.c says ", JIS only" */
	/*  96 */   KEY_F5,
	/*  97 */   KEY_F6,
	/*  98 */   KEY_F7,
	/*  99 */   KEY_F3,
	/* 100 */   KEY_F8,
	/* 101 */   KEY_F9,
	/* 102 */   KEY_UNDEFINED, // KEY_LANG2, /* Cosmo_USB2ADB.c says "Eisu" */
	/* 103 */   KEY_F11,
	/* 104 */   KEY_UNDEFINED, // KEY_LANG1, /* Cosmo_USB2ADB.c says "Kana" */
	/* 105 */   KEY_UNDEFINED, // KEY_PRINTSCREEN, /* On ADB keyboards, this key is labeled "F13/print screen". Problem: USB has different usage codes for these two functions. On Apple USB keyboards, the key is labeled "F13" and sends the F13 usage code (KEY_F13). I decided to use KEY_PRINTSCREEN here nevertheless since SDL applications are more likely to assume the presence of a print screen key than an F13 key. */
	/* 106 */   KEY_UNDEFINED, // KEY_F16,
	/* 107 */   KEY_UNDEFINED, // KEY_SCROLLLOCK, /* F14/scroll lock, see comment about F13/print screen above */
	/* 108 */   KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/* 109 */   KEY_UNDEFINED, // KEY_F10,
	/* 110 */   KEY_UNDEFINED, // KEY_APPLICATION, /* windows contextual menu key, fn-enter on portables */
	/* 111 */   KEY_F12,
	/* 112 */   KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/* 113 */   KEY_PAUSE, /* F15/pause, see comment about F13/print screen above */
	/* 114 */   KEY_INSERT, /* the key is actually labeled "help" on Apple keyboards, and works as such in Mac OS, but it sends the "insert" usage code even on Apple USB keyboards */
	/* 115 */   KEY_HOME,
	/* 116 */   KEY_PAGEUP,
	/* 117 */   KEY_DELETE,
	/* 118 */   KEY_F4,
	/* 119 */   KEY_END,
	/* 120 */   KEY_F2,
	/* 121 */   KEY_PAGEDOWN,
	/* 122 */   KEY_F1,
	/* 123 */   KEY_LEFT,
	/* 124 */   KEY_RIGHT,
	/* 125 */   KEY_DOWN,
	/* 126 */   KEY_UP,
	/* 127 */   KEY_UNDEFINED, // KEY_POWER
};

void Cocoa_HandleKeyEvent(NSEvent *event)
{
	unsigned short scancode = [event keyCode];
	KeyCode key;

	if (scancode < 128) {
		key = darwin_scancode_table[scancode];
	}
	else {
        printf("unknown scancode: %d", scancode);
		//Debug::Log("Unknown scancode");
	}

	switch ([event type]) {
		case NSKeyDown:
			// if (![event isARepeat]) {
			//     /* See if we need to rebuild the keyboard layout */
			//     UpdateKeymap(data);
			// }

			CocoaPlatform::platform->SetLocalKey(key, true);
			break;
		case NSKeyUp:
	   CocoaPlatform::platform->SetLocalKey(key, false);
	   break;
	   // case NSFlagsChanged:
	   //     HandleModifiers(_this, scancode, [event modifierFlags]);
	   //     break;
		default: /* just to avoid compiler warnings */
	   break;
	}
}

