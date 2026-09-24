#include "sc_device.h"

#include <hal/hal.h>

#include <mooncake_log.h>
#include <smooth_ui_toolkit.h>
#include <smooth_lvgl.h>\n#include "apps/utils/audio/audio.h"

using namespace smooth_ui_toolkit;
using namespace smooth_ui_toolkit::lvgl_cpp;\n\nextern const uint8_t sc_horse_png_start[] asm("_binary_special_chess_horse_png_start");\nextern const uint8_t sc_horse_png_end[] asm("_binary_special_chess_horse_png_end");

namespace {

constexpr uint32_t BG = 0x020812;
constexpr uint32_t PANEL = 0x081525;
constexpr uint32_t PANEL_SOFT = 0x0A1C31;
constexpr uint32_t TEXT = 0xF5F7FA;
constexpr uint32_t MUTED = 0xB4C0D1;
constexpr uint32_t MUTED_2 = 0x8195AC;
constexpr uint32_t GOLD = 0xEFC65C;
constexpr uint32_t GOLD_LIGHT = 0xFFE5A0;
constexpr uint32_t BLUE = 0x78B2FF;
constexpr uint32_t LINE = 0x29415E;

void styleLabel(lv_obj_t* obj, uint32_t color, const lv_font_t* font)
{
    lv_obj_set_style_text_color(obj, lv_color_hex(color), LV_PART_MAIN);
    lv_obj_set_style_text_font(obj, font, LV_PART_MAIN);
}

lv_obj_t* makeText(lv_obj_t* parent, const char* text, uint32_t color, const lv_font_t* font)
{
    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text(label, text);
    styleLabel(label, color, font);
    return label;
}

void styleCard(lv_obj_t* card, uint32_t border, uint32_t accent)
{
    lv_obj_set_style_bg_color(card, lv_color_hex(PANEL_SOFT), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(card, lv_color_hex(PANEL), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(card, LV_GRAD_DIR_VER, LV_PART_MAIN);
    lv_obj_set_style_border_width(card, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(card, lv_color_hex(border), LV_PART_MAIN);
    lv_obj_set_style_radius(card, 26, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(card, 30, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(card, LV_OPA_20, LV_PART_MAIN);
    lv_obj_set_style_shadow_color(card, lv_color_hex(accent), LV_PART_MAIN);
    lv_obj_set_style_pad_all(card, 0, LV_PART_MAIN);
}

void addAccent(lv_obj_t* parent, uint32_t color)
{
    lv_obj_t* accent = lv_obj_create(parent);
    lv_obj_remove_flag(accent, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(accent, 8, 220);
    lv_obj_align(accent, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_color(accent, lv_color_hex(color), LV_PART_MAIN);
    lv_obj_set_style_border_width(accent, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(accent, 8, LV_PART_MAIN);
}

void addPill(lv_obj_t* parent, const char* text, uint32_t color, int x, int y)
{
    lv_obj_t* pill = lv_obj_create(parent);
    lv_obj_remove_flag(pill, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(pill, LV_SIZE_CONTENT, 38);
    lv_obj_align(pill, LV_ALIGN_TOP_LEFT, x, y);
    lv_obj_set_style_bg_color(pill, lv_color_hex(color), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(pill, LV_OPA_10, LV_PART_MAIN);
    lv_obj_set_style_border_width(pill, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(pill, lv_color_hex(color), LV_PART_MAIN);
    lv_obj_set_style_radius(pill, 19, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(pill, 14, LV_PART_MAIN);
    lv_obj_set_style_pad_ver(pill, 6, LV_PART_MAIN);

    lv_obj_t* label = makeText(pill, text, color, &lv_font_montserrat_16);
    lv_obj_center(label);
}

}  // namespace

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
    lv_obj_set_style_bg_color(screen, lv_color_hex(BG), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(screen, lv_color_hex(0x071B31), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_HOR, LV_PART_MAIN);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    // Soft ambient glows: native LVGL approximation of the iPhone/web visual language.
    lv_obj_t* glow_left = lv_obj_create(screen);
    lv_obj_remove_flag(glow_left, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(glow_left, 430, 430);
    lv_obj_align(glow_left, LV_ALIGN_TOP_LEFT, -190, -180);
    lv_obj_set_style_radius(glow_left, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(glow_left, lv_color_hex(0x0B2A4D), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(glow_left, LV_OPA_30, LV_PART_MAIN);
    lv_obj_set_style_border_width(glow_left, 0, LV_PART_MAIN);

    lv_obj_t* glow_right = lv_obj_create(screen);
    lv_obj_remove_flag(glow_right, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(glow_right, 420, 420);
    lv_obj_align(glow_right, LV_ALIGN_BOTTOM_RIGHT, 170, 210);
    lv_obj_set_style_radius(glow_right, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(glow_right, lv_color_hex(0x08458C), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(glow_right, LV_OPA_20, LV_PART_MAIN);
    lv_obj_set_style_border_width(glow_right, 0, LV_PART_MAIN);

    // Brand lockup.
    _title = makeText(screen, "SPECIAL CHESS", TEXT, &lv_font_montserrat_32);
    lv_obj_align(_title, LV_ALIGN_TOP_LEFT, 58, 32);

    _subtitle = makeText(screen, "THE NEW CHESS EXPERIENCE", GOLD, &lv_font_montserrat_16);
    lv_obj_align_to(_subtitle, _title, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 7);

    lv_obj_t* device_label = makeText(screen, "SC DEVICE", MUTED_2, &lv_font_montserrat_16);
    lv_obj_align(device_label, LV_ALIGN_TOP_RIGHT, -58, 38);

    lv_obj_t* top_line = lv_obj_create(screen);
    lv_obj_remove_flag(top_line, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(top_line, 1164, 1);
    lv_obj_align(top_line, LV_ALIGN_TOP_MID, 0, 96);
    lv_obj_set_style_bg_color(top_line, lv_color_hex(LINE), LV_PART_MAIN);
    lv_obj_set_style_border_width(top_line, 0, LV_PART_MAIN);

    lv_obj_t* intro = makeText(screen, "ELIGE TU EXPERIENCIA", GOLD_LIGHT, &lv_font_montserrat_16);
    lv_obj_set_style_text_letter_space(intro, 3, LV_PART_MAIN);
    lv_obj_align(intro, LV_ALIGN_TOP_MID, 0, 118);

    // Quick timer card.
    _timer_btn = lv_btn_create(screen);
    lv_obj_set_size(_timer_btn, 540, 315);
    lv_obj_align(_timer_btn, LV_ALIGN_CENTER, -292, 42);
    styleCard(_timer_btn, 0x5C5131, GOLD);
    lv_obj_add_event_cb(_timer_btn, onTimerClicked, LV_EVENT_CLICKED, this);
    addAccent(_timer_btn, GOLD);

    lv_obj_t* timer_num = makeText(_timer_btn, "01", GOLD, &lv_font_montserrat_16);
    lv_obj_align(timer_num, LV_ALIGN_TOP_LEFT, 42, 30);

    lv_obj_t* timer_mode = makeText(_timer_btn, "RAPIDO", MUTED_2, &lv_font_montserrat_12);
    lv_obj_set_style_text_letter_space(timer_mode, 2, LV_PART_MAIN);
    lv_obj_align(timer_mode, LV_ALIGN_TOP_RIGHT, -32, 33);

    lv_obj_t* timer_title = makeText(_timer_btn, "SC TIMER", TEXT, &lv_font_montserrat_32);
    lv_obj_align(timer_title, LV_ALIGN_TOP_LEFT, 42, 84);

    lv_obj_t* timer_desc = makeText(
        _timer_btn,
        "Cronometraje inmediato para jugar.\nTiempo total y tiempo de respuesta.",
        MUTED,
        &lv_font_montserrat_18);
    lv_obj_set_width(timer_desc, 440);
    lv_label_set_long_mode(timer_desc, LV_LABEL_LONG_WRAP);
    lv_obj_align(timer_desc, LV_ALIGN_TOP_LEFT, 42, 137);

    addPill(_timer_btn, "ENTRAR", GOLD, 42, 239);

    // Advanced match card.
    _match_btn = lv_btn_create(screen);
    lv_obj_set_size(_match_btn, 540, 315);
    lv_obj_align(_match_btn, LV_ALIGN_CENTER, 292, 42);
    styleCard(_match_btn, 0x355777, BLUE);
    lv_obj_add_event_cb(_match_btn, onMatchClicked, LV_EVENT_CLICKED, this);
    addAccent(_match_btn, BLUE);

    lv_obj_t* match_num = makeText(_match_btn, "02", BLUE, &lv_font_montserrat_16);
    lv_obj_align(match_num, LV_ALIGN_TOP_LEFT, 42, 30);

    lv_obj_t* match_mode = makeText(_match_btn, "AVANZADO", MUTED_2, &lv_font_montserrat_12);
    lv_obj_set_style_text_letter_space(match_mode, 2, LV_PART_MAIN);
    lv_obj_align(match_mode, LV_ALIGN_TOP_RIGHT, -32, 33);

    lv_obj_t* match_title = makeText(_match_btn, "SC MATCH SYSTEM", TEXT, &lv_font_montserrat_30);
    lv_obj_align(match_title, LV_ALIGN_TOP_LEFT, 42, 84);

    lv_obj_t* match_desc = makeText(
        _match_btn,
        "Planos, capturas, puntuacion y analisis.\nLa experiencia completa de SPECIAL CHESS.",
        MUTED,
        &lv_font_montserrat_18);
    lv_obj_set_width(match_desc, 440);
    lv_label_set_long_mode(match_desc, LV_LABEL_LONG_WRAP);
    lv_obj_align(match_desc, LV_ALIGN_TOP_LEFT, 42, 137);

    addPill(_match_btn, "ENTRAR", BLUE, 42, 239);

    _status = makeText(screen, "Selecciona un modo", MUTED, &lv_font_montserrat_18);
    lv_obj_align(_status, LV_ALIGN_BOTTOM_LEFT, 58, -34);

    _version = makeText(screen, "SC DEVICE  V0.4  PREVIEW", MUTED_2, &lv_font_montserrat_14);
    lv_obj_align(_version, LV_ALIGN_BOTTOM_RIGHT, -58, -36);
}

void SCDevice::onTimerClicked(lv_event_t* e)
{
    auto* self = static_cast<SCDevice*>(lv_event_get_user_data(e));
    if (self && self->_status) {
        lv_label_set_text(self->_status, "SC TIMER seleccionado");\n        audio::play_chord({76, 83}, 0.055);
        lv_obj_set_style_border_color(self->_timer_btn, lv_color_hex(GOLD), LV_PART_MAIN);
        lv_obj_set_style_border_width(self->_timer_btn, 3, LV_PART_MAIN);
        lv_obj_set_style_border_color(self->_match_btn, lv_color_hex(0x355777), LV_PART_MAIN);
        lv_obj_set_style_border_width(self->_match_btn, 2, LV_PART_MAIN);
    }
}

void SCDevice::onMatchClicked(lv_event_t* e)
{
    auto* self = static_cast<SCDevice*>(lv_event_get_user_data(e));
    if (self && self->_status) {
        lv_label_set_text(self->_status, "SC MATCH SYSTEM seleccionado");\n        audio::play_chord({71, 78}, 0.065);
        lv_obj_set_style_border_color(self->_match_btn, lv_color_hex(BLUE), LV_PART_MAIN);
        lv_obj_set_style_border_width(self->_match_btn, 3, LV_PART_MAIN);
        lv_obj_set_style_border_color(self->_timer_btn, lv_color_hex(0x5C5131), LV_PART_MAIN);
        lv_obj_set_style_border_width(self->_timer_btn, 2, LV_PART_MAIN);
    }
}

void SCDevice::onRunning()
{
}

void SCDevice::onClose()
{
}
