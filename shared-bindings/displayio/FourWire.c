/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2019 Scott Shawcroft for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "shared-bindings/displayio/FourWire.h"

#include <stdint.h>

#include "lib/utils/context_manager_helpers.h"
#include "py/binary.h"
#include "py/objproperty.h"
#include "py/runtime.h"
#include "shared-bindings/displayio/Group.h"
#include "shared-bindings/microcontroller/Pin.h"
#include "shared-bindings/util.h"
#include "shared-module/displayio/__init__.h"
#include "supervisor/shared/translate.h"

//| .. currentmodule:: displayio
//|
//| :class:`FourWire` -- Manage updating a display over SPI four wire protocol
//| ==========================================================================
//|
//| Manage updating a display over SPI four wire protocol in the background while Python code runs.
//| It doesn't handle display initialization.
//|
//| .. warning:: This will be changed before 4.0.0. Consider it very experimental.
//|
//| .. class:: FourWire(spi_bus, *, command, chip_select, reset)
//|
//|   Create a FourWire object associated with the given pins.
//|
//|   :param busio.SPI spi_bus: The SPI bus that make up the clock and data lines
//|   :param microcontroller.Pin command: Data or command pin
//|   :param microcontroller.Pin chip_select: Chip select pin
//|   :param microcontroller.Pin reset: Reset pin
//|
STATIC mp_obj_t displayio_fourwire_make_new(const mp_obj_type_t *type, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_spi_bus, ARG_command, ARG_chip_select, ARG_reset };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_spi_bus, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_command, MP_ARG_OBJ | MP_ARG_KW_ONLY | MP_ARG_REQUIRED },
        { MP_QSTR_chip_select, MP_ARG_OBJ | MP_ARG_KW_ONLY | MP_ARG_REQUIRED },
        { MP_QSTR_reset, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = mp_const_none} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_obj_t command = args[ARG_command].u_obj;
    mp_obj_t chip_select = args[ARG_chip_select].u_obj;
    assert_pin_free(command);
    assert_pin_free(chip_select);
    mp_obj_t reset = args[ARG_reset].u_obj;
    if (reset != mp_const_none) {
        assert_pin_free(reset);
    } else {
        reset = NULL;
    }

    displayio_fourwire_obj_t* self = NULL;
    mp_obj_t spi = args[ARG_spi_bus].u_obj;
    for (uint8_t i = 0; i < CIRCUITPY_DISPLAY_LIMIT; i++) {
        if (displays[i].fourwire_bus.base.type == NULL ||
            displays[i].fourwire_bus.base.type == &mp_type_NoneType) {
            self = &displays[i].fourwire_bus;
            self->base.type = &displayio_fourwire_type;
            break;
        }
    }
    if (self == NULL) {
        mp_raise_RuntimeError(translate("Too many display busses"));
    }

    common_hal_displayio_fourwire_construct(self,
        MP_OBJ_TO_PTR(spi), command, chip_select, reset);
    return self;
}


//|   .. method:: send(command, data)
//|
//|
STATIC mp_obj_t displayio_fourwire_obj_send(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    mp_raise_NotImplementedError(translate("displayio is a work in progress"));

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(displayio_fourwire_send_obj, 1, displayio_fourwire_obj_send);

STATIC const mp_rom_map_elem_t displayio_fourwire_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_send), MP_ROM_PTR(&displayio_fourwire_send_obj) },
};
STATIC MP_DEFINE_CONST_DICT(displayio_fourwire_locals_dict, displayio_fourwire_locals_dict_table);

const mp_obj_type_t displayio_fourwire_type = {
    { &mp_type_type },
    .name = MP_QSTR_FourWire,
    .make_new = displayio_fourwire_make_new,
    .locals_dict = (mp_obj_dict_t*)&displayio_fourwire_locals_dict,
};
