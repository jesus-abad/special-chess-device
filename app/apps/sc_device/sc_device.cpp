#include "sc_device.h"

#include <hal/hal.h>

#include <mooncake_log.h>
#include <smooth_ui_toolkit.h>
#include <smooth_lvgl.h>

using namespace smooth_ui_toolkit;
using namespace smooth_ui_toolkit::lvgl_cpp;

SCDevice::SCDevice()
{
    setAppInfo().name = "SCDevice";
}

void SCDevice::onCreate()
{
    mclog::tagInfo(getAppInfo().name, "on create");
    open();
}

void SCDevice::onOpen()
{
    mclog::tagInfo(getAppInfo().name, "on open");

    LvglLockGuard lock;

    lv_obj_t* screen = lv_screen_active();

    lv_obj_set_style_bg_color(screen, lv_color_hex(0x050912), LV_PART_MAIN);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    _title = lv_label_create(screen);
    lv_label_set_text(_title, "SPECIAL CHESS");
    lv_obj_set_style_text_color(_title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(_title, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_align(_title, LV_ALIGN_TOP_MID, 0, 85);

    _subtitle = lv_label_create(screen);
    lv_label_set_text(_subtitle, "THE NEW CHESS EXPERIENCE");
    lv_obj_set_style_text_color(_subtitle, lv_color_hex(0xAEB8C8), LV_PART_MAIN);
    lv_obj_set_style_text_font(_subtitle, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_align(_subtitle, LV_ALIGN_TOP_MID, 0, 135);

    _timer_btn = lv_btn_create(screen);
    lv_obj_set_size(_timer_btn, 480, 170);
    lv_obj_align(_timer_btn, LV_ALIGN_CENTER, -270, 35);
    lv_obj_set_style_bg_color(_timer_btn, lv_color_hex(0x172033), LV_PART_MAIN);
    lv_obj_set_style_radius(_timer_btn, 24, LV_PART_MAIN);
    lv_obj_set_style_border_width(_timer_btn, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(_timer_btn, lv_color_hex(0x3B82F6), LV_PART_MAIN);
    lv_obj_add_event_cb(_timer_btn, onTimerClicked, LV_EVENT_CLICKED, this);

    lv_obj_t* timer_label = lv_label_create(_timer_btn);
    lv_label_set_text(timer_label, "SC TIMER");
    lv_obj_set_style_text_color(timer_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(timer_label, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_center(timer_label);

    _match_btn = lv_btn_create(screen);
    lv_obj_set_size(_match_btn, 480, 170);
    lv_obj_align(_match_btn, LV_ALIGN_CENTER, 270, 35);
    lv_obj_set_style_bg_color(_match_btn, lv_color_hex(0x172033), LV_PART_MAIN);
    lv_obj_set_style_radius(_match_btn, 24, LV_PART_MAIN);
    lv_obj_set_style_border_width(_match_btn, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(_match_btn, lv_color_hex(0x8B5CF6), LV_PART_MAIN);
    lv_obj_add_event_cb(_match_btn, onMatchClicked, LV_EVENT_CLICKED, this);

    lv_obj_t* match_label = lv_label_create(_match_btn);
    lv_label_set_text(match_label, "SC MATCH SYSTEM");
    lv_obj_set_style_text_color(match_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(match_label, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_center(match_label);

    _status = lv_label_create(screen);
    lv_label_set_text(_status, "Selecciona un modo");
    lv_obj_set_style_text_color(_status, lv_color_hex(0x94A3B8), LV_PART_MAIN);
    lv_obj_set_style_text_font(_status, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_align(_status, LV_ALIGN_BOTTOM_MID, 0, -80);

    _version = lv_label_create(screen);
    lv_label_set_text(_version, "SC DEVICE  V0.2");
    lv_obj_set_style_text_color(_version, lv_color_hex(0x64748B), LV_PART_MAIN);
    lv_obj_set_style_text_font(_version, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_align(_version, LV_ALIGN_BOTTOM_MID, 0, -25);
}

void SCDevice::onTimerClicked(lv_event_t* e)
{
    auto* self = static_cast<SCDevice*>(lv_event_get_user_data(e));
    if (self && self->_status) {
        lv_label_set_text(self->_status, "SC TIMER seleccionado");
    }
}

void SCDevice::onMatchClicked(lv_event_t* e)
{
    auto* self = static_cast<SCDevice*>(lv_event_get_user_data(e));
    if (self && self->_status) {
        lv_label_set_text(self->_status, "SC MATCH SYSTEM seleccionado");
    }
}

void SCDevice::onRunning()
{
}

void SCDevice::onClose()
{
}
