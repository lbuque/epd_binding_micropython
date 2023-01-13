#include "epd_epd47.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "py/mpthread.h"
#include "py/stream.h"
#include "py/objtype.h"
#include "py/mperrno.h"
#include "py/gc.h"
#include "extmod/vfs.h"

#include "epd_driver.h"
#include "ed047tc1.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct mp_obj_epd47_t {
    mp_obj_base_t base;
    // uint8_t *jpeg_buf;
    uint16_t width;
    uint16_t height;
} mp_obj_epd47_t;


STATIC mp_obj_t epd47_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    /**
     * When using del obj to destroy the current object, the epd47_delete()
     * method will not be called immediately, resulting in hardware resources
     * not being destroyed in time. It is necessary to call gc_collect() to
     * destroy the previous hardware resources when creating the object,
     * or use obj.deinit() to destroy the corresponding hardware resources in
     * micropython.
     */
    gc_collect();

    mp_arg_check_num(n_args, n_kw, 0, 0, false);
    mp_obj_epd47_t *o = mp_obj_malloc(mp_obj_epd47_t, type);
    if (!o) {
        mp_raise_TypeError(MP_ERROR_TEXT("malloc fail"));
        return mp_const_none;
    }

    o->base.type = &mp_type_epd;
    o->width = EPD_WIDTH;
    o->height = EPD_HEIGHT;

    epd_init();

    return MP_OBJ_FROM_PTR(o);
}


STATIC mp_obj_t epd47_power(mp_obj_t self_in, mp_obj_t onoff_in)
{
    // mp_obj_epd47_t *self = MP_OBJ_TO_PTR(self_in);

    if (mp_obj_is_true(onoff_in)) {
        epd_poweron();
    } else {
        epd_poweroff();
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(epd47_power_obj, epd47_power);


STATIC mp_obj_t epd47_bitmap(size_t n_args, const mp_obj_t *args)
{
    Rect_t area;
    mp_buffer_info_t bufinfo;

    if (n_args < 2 || n_args > 6) return mp_const_none;

    mp_get_buffer_raise(args[1], &bufinfo, MP_BUFFER_READ);
    area.x      = mp_obj_get_int(args[2]);
    area.y      = mp_obj_get_int(args[3]);
    area.width  = mp_obj_get_int(args[4]);
    area.height = mp_obj_get_int(args[5]);

    epd_clear_area(area);
    epd_draw_image(area, (uint8_t *)bufinfo.buf, BLACK_ON_WHITE);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(epd47_bitmap_obj, 6, 6, epd47_bitmap);


STATIC mp_obj_t epd47_clear(size_t n_args, const mp_obj_t *args)
{
    mp_obj_epd47_t *self = MP_OBJ_TO_PTR(args[0]);
    Rect_t area = {.x = 0, .y = 0, .width = self->width, .height = self->height};

    if (n_args > 1 && mp_obj_get_int(args[1]) > 0 && mp_obj_get_int(args[1]) < self->width) {
        area.x = mp_obj_get_int(args[1]);
    }

    if (n_args > 2 && mp_obj_get_int(args[2]) > 0 && mp_obj_get_int(args[2]) < self->height) {
        area.y = mp_obj_get_int(args[2]);
    }

    if (n_args > 3 && mp_obj_get_int(args[3]) > 0 && mp_obj_get_int(args[3]) < self->width) {
        area.width = mp_obj_get_int(args[3]);
    }

    if (n_args > 4 && mp_obj_get_int(args[4]) > 0 && mp_obj_get_int(args[4]) < self->height) {
        area.height = mp_obj_get_int(args[4]);
    }

    epd_clear_area(area);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(epd47_clear_obj, 1, 5, epd47_clear);


STATIC mp_obj_t epd47_width(mp_obj_t self_in)
{
    mp_obj_epd47_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_int(self->width);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(epd47_width_obj, epd47_width);


STATIC mp_obj_t epd47_height(mp_obj_t self_in)
{
    mp_obj_epd47_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_int(self->height);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(epd47_height_obj, epd47_height);


STATIC mp_obj_t epd47_delete(mp_obj_t self_in)
{
    // mp_obj_epd47_t *self = MP_OBJ_TO_PTR(self_in);

    epd_deinit();

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(epd47_delete_obj, epd47_delete);


STATIC const mp_rom_map_elem_t epd47_if_locals_dict_table[] = {
    // method
    { MP_ROM_QSTR(MP_QSTR_power),   MP_ROM_PTR(&epd47_power_obj)  },
    { MP_ROM_QSTR(MP_QSTR_bitmap),  MP_ROM_PTR(&epd47_bitmap_obj) },
    { MP_ROM_QSTR(MP_QSTR_clear),   MP_ROM_PTR(&epd47_clear_obj)  },
    { MP_ROM_QSTR(MP_QSTR_width),   MP_ROM_PTR(&epd47_width_obj)  },
    { MP_ROM_QSTR(MP_QSTR_height),  MP_ROM_PTR(&epd47_height_obj) },
    { MP_ROM_QSTR(MP_QSTR_deinit),  MP_ROM_PTR(&epd47_delete_obj) },
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&epd47_delete_obj) },
};
STATIC MP_DEFINE_CONST_DICT(epd47_if_locals_dict, epd47_if_locals_dict_table);


const mp_obj_type_t mp_type_epd = {
    { &mp_type_type },
    .name = MP_QSTR_EPD47,
    .make_new = epd47_make_new,
    .locals_dict = (mp_obj_dict_t *)&epd47_if_locals_dict,
};