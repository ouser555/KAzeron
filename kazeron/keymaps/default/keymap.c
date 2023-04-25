#include QMK_KEYBOARD_H
#include "analog.h"
#include "pointing_device.h"
#include "./drivers/sensors/analog_joystick.h"

enum layers {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST,
};

enum jmodes {
    _MOUSE = 0,
    _SCROLL,
    _JOYS,
    _WASD,
};

enum custom_keycodes {
#ifdef VIA_ENABLE
  //QWERTY = USER00,
  QWERTY = QK_KB_0,
  //QWERTY = CS00, // qmk v1.1.1
#else
  QWERTY = SAFE_RANGE,
#endif
  LOWER,
  RAISE,
  ADJUST,
  JOYSITKMOD
};

//uint8_t jMode = _MOUSE;
uint8_t jMode = _WASD;

void joysitck_mode_INC(void) {
  if(jMode == _WASD) {
    jMode = _MOUSE;
  }else{
    jMode++;
  }
}
void joysitck_mode_DEC(void) {
  if(jMode == _MOUSE) {
    jMode = _WASD;
  }else{
    jMode--;
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* LAYER 0
 * ,-----------------------.
 * |   1   |   2   |   3   |
 * |-------+-------+-------|
 * |   4   | 5/ENT |   6   | Dbl Tap 5 for Enter
 * |-------+-------+-------|
 * |  7/0  |   8   | 9/FN  | 7/0 = Dbl Tap 7 for 0  -  9/FN = Hold 9 for FN
 * `-----------------------'
 */
[_QWERTY] = LAYOUT(
  LCTL(KC_Y),     LCTL(KC_Z),     LCTL(KC_C),    LCTL(KC_V),     JOYSITKMOD,
  KC_Y,           KC_N,           KC_BTN3,       KC_ENTER,       _______,
  LOWER,          RAISE,          KC_BTN2,       KC_BTN1,        _______
),

/* LAYER 1
 * ,-----------------------.
 * |  ESC  |   +   |   -   |
 * |-------+-------+-------|
 * |  BSPC |   *   |   /   |
 * |-------+-------+-------|
 * |  00   |   .   |       |
 * `-----------------------'
 */
[_LOWER] = LAYOUT(
  LCTL(KC_Y),     LCTL(KC_Z),     LCTL(KC_C),    LCTL(KC_V),     JOYSITKMOD,
  KC_Y,           KC_N,           KC_BTN3,       KC_ENTER,       _______,
  LOWER,          RAISE,          KC_BTN2,       KC_BTN1,        _______
),

/* LAYER 1
 * ,-----------------------.
 * |  ESC  |   +   |   -   |
 * |-------+-------+-------|
 * |  BSPC |   *   |   /   |
 * |-------+-------+-------|
 * |  00   |   .   |       |
 * `-----------------------'
 */
[_RAISE] = LAYOUT(
  LCTL(KC_Y),     LCTL(KC_Z),     LCTL(KC_C),    LCTL(KC_V),     JOYSITKMOD,
  KC_Y,           KC_N,           KC_BTN3,       KC_ENTER,       _______,
  LOWER,          RAISE,          KC_BTN2,       KC_BTN1,        _______
),

/* LAYER 1
 * ,-----------------------.
 * |  ESC  |   +   |   -   |
 * |-------+-------+-------|
 * |  BSPC |   *   |   /   |
 * |-------+-------+-------|
 * |  00   |   .   |       |
 * `-----------------------'
 */
[_ADJUST] = LAYOUT(
  LCTL(KC_Y),     LCTL(KC_Z),     LCTL(KC_C),    LCTL(KC_V),     JOYSITKMOD,
  KC_Y,           KC_N,           KC_BTN3,       KC_ENTER,       _______,
  LOWER,          RAISE,          KC_BTN2,       KC_BTN1,        _______
)

};


#if 1
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
         print("mode just switched to qwerty and this is a huge string\n");
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      //break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      //break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      //break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      //break;

    case JOYSITKMOD:
      if (record->event.pressed) {
          joysitck_mode_INC();
      }
      return false;
      //break;
  }
  return true;
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            //tap_code_delay(KC_VOLU, 10);
            joysitck_mode_INC();
        } else {
            //tap_code_delay(KC_VOLD, 10);
            joysitck_mode_DEC();
        }
    } else if (index == 1) { /* Second encoder */
        if (clockwise) {
            tap_code_delay(KC_VOLU, 5);
        } else {
            tap_code_delay(KC_VOLD, 5);
        }
    }
    return false;
}
#endif
#ifdef POINTING_DEVICE_ENABLE

int16_t xOrigin, yOrigin;

int16_t maxisCoordinate(pin_t pin, uint16_t origin) {
    int8_t  direction;
    int16_t distanceFromOrigin;
    int16_t range;

    int16_t position = analogReadPin(pin);

    if (origin == position) {
        return 0;
    } else if (origin > position) {
        distanceFromOrigin = origin - position;
        range              = origin - _MIN;
        direction          = -1;
    } else {
        distanceFromOrigin = position - origin;
        range              = _MAX - origin;
        direction          = 1;
    }

    float   percent    = (float)distanceFromOrigin / range;
    int16_t coordinate = (int16_t)(percent * _CENTER);
    if (coordinate < 0) {
        return 0;
    } else if (coordinate > _CENTER) {
        return _CENTER * direction;
    } else {
        return coordinate * direction;
    }
}

void pointing_device_driver_init(void) {

  analog_joystick_init();
  xOrigin = analogReadPin(JSH);
  yOrigin = analogReadPin(JSV);

}

__attribute__((weak)) void process_mouse_user(report_mouse_t* mouse_report, int16_t x, int16_t y) {
}

__attribute__((weak)) void process_mouse(report_mouse_t* mouse_report) {
    if(jMode == _MOUSE){
      report_analog_joystick_t data = analog_joystick_read();
      mouse_report->h = 0;
      mouse_report->v = 0;
      mouse_report->x = data.x;
      mouse_report->y = -data.y;
    //}else if(jMode == _SCROLL){
    //  report_analog_joystick_t data = analog_joystick_read();
    //  mouse_report->h = -data.x/10;
    //  mouse_report->v = -data.y/10;
    //  mouse_report->x = 0;
    //  mouse_report->y = 0;
    }else{
      mouse_report->h = 0;
      mouse_report->v = 0;
      mouse_report->x = 0;
      mouse_report->y = 0;
    }
}

void pointing_device_task(void) {
    report_mouse_t mouse_report = pointing_device_get_report();
    process_mouse(&mouse_report);
    pointing_device_set_report(mouse_report);
    pointing_device_send();
}
#endif

#ifdef JOYSTICK_ENABLE

  int16_t xPos = 0;
  int16_t yPos = 0;

  bool wasdShiftMode = false;
  //bool autorun = false;

  bool yDownHeld = false;
  bool yUpHeld = false;
  bool xLeftHeld = false;
  bool xRightHeld = false;
  bool shiftHeld = false;

  void matrix_scan_user(void) {
    if (jMode == _WASD) {
      // W & S
      //if (!autorun) {
      yPos = analogReadPin(JSV);
      if (!yDownHeld && yPos >= _DOWN_TRESHOLD) {
        register_code(KC_W);
        yDownHeld = true;
      } else if (yDownHeld && yPos < _DOWN_TRESHOLD) {
        yDownHeld = false;
        unregister_code(KC_W);
      } else if (!yUpHeld && yPos <= _UP_TRESHOLD) {
        yUpHeld = true;
        register_code(KC_S);
      } else if (yUpHeld && yPos > _UP_TRESHOLD) {
        yUpHeld = false;
        unregister_code(KC_S);
      }
      //}
      
      xPos = analogReadPin(JSH);
      if (!xLeftHeld && xPos >= _DOWN_TRESHOLD) {
        register_code(KC_D);
        xLeftHeld = true;
      } else if (xLeftHeld && xPos < _DOWN_TRESHOLD) {
        xLeftHeld = false;
        unregister_code(KC_D);
      } else if (!xRightHeld && xPos <= _UP_TRESHOLD) {
        xRightHeld = true;
        register_code(KC_A);
      } else if (xRightHeld && xPos > _UP_TRESHOLD) {
        xRightHeld = false;
        unregister_code(KC_A);
      }
#if 0
      if (wasdShiftMode) {
        bool yShifted = yPos < _SHIFT;
        if (!shiftHeld && yShifted) {
          register_code(KC_LSFT);
          shiftHeld = true;
        } else if (shiftHeld && !yShifted) {
          unregister_code(KC_LSFT);
          shiftHeld = false;
        }
      }
#endif
    }else if(jMode == _JOYS){
      int16_t jx = maxisCoordinate(JSH, xOrigin);
      int16_t jy = maxisCoordinate(JSV, yOrigin);
      joystick_set_axis( 0,  jx);
      joystick_set_axis( 1, -jy);
 
    }else if(jMode == _MOUSE){

    }else if(jMode == _SCROLL){
      yPos = analogReadPin(JSV);
      if (!yDownHeld && yPos >= _DOWN_TRESHOLD) {
        register_code(KC_WH_U);
        yDownHeld = true;
      } else if (yDownHeld && yPos < _DOWN_TRESHOLD) {
        yDownHeld = false;
        unregister_code(KC_WH_U);
      } else if (!yUpHeld && yPos <= _UP_TRESHOLD) {
        yUpHeld = true;
        register_code(KC_WH_D);
      } else if (yUpHeld && yPos > _UP_TRESHOLD) {
        yUpHeld = false;
        unregister_code(KC_WH_D);
      }
      //}
      
      xPos = analogReadPin(JSH);
      if (!xLeftHeld && xPos >= _DOWN_TRESHOLD) {
        register_code(KC_WH_R);
        xLeftHeld = true;
      } else if (xLeftHeld && xPos < _DOWN_TRESHOLD) {
        xLeftHeld = false;
        unregister_code(KC_WH_R);
      } else if (!xRightHeld && xPos <= _UP_TRESHOLD) {
        xRightHeld = true;
        register_code(KC_WH_L);
      } else if (xRightHeld && xPos > _UP_TRESHOLD) {
        xRightHeld = false;
        unregister_code(KC_WH_L);
      }
    }
  }

  //joystick config
  joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
    JOYSTICK_AXIS_VIRTUAL,
    JOYSTICK_AXIS_VIRTUAL
    //JOYSTICK_AXIS_IN(JSH, _MAX, _CENTER, _MIN),
    //JOYSTICK_AXIS_IN(JSV, _MIN, _CENTER, _MAX)  
  };
#endif // joystick

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_BASE] =   { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN), ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_LOWER] =  { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),           ENCODER_CCW_CW(RGB_SAD, RGB_SAI)  },
    [_RAISE] =  { ENCODER_CCW_CW(RGB_VAD, RGB_VAI),           ENCODER_CCW_CW(RGB_SPD, RGB_SPI)  },
    [_ADJUST] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD),          ENCODER_CCW_CW(KC_RIGHT, KC_LEFT) },
};
#endif

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
        return OLED_ROTATION_180;  // flips the display 270 degrees if offhand
}

static void render_logo(void) {
    static const char PROGMEM raw_logo[] = {
        // '115', 128x32px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 
0xe0, 0xe0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 
0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x80, 
0xe0, 0xe0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 
0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 
0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 
0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 
0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
0x03, 0x03, 0x03, 0x03, 0x03, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
0x03, 0x03, 0x03, 0x03, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x03, 
0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x07, 
0x07, 0x03, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    oled_write_raw_P(raw_logo, sizeof(raw_logo));
}

bool oled_task_user(void) {
    // Host Keyboard Layer Status

    render_logo();
    oled_scroll_right();
#if 0
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("DEFAULT\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("LOWER\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("RAISE\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("ADJUST\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
#endif
    return false;
}
#endif