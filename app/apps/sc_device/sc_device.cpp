#include "sc_device.h"

#include <hal/hal.h>
#include <lvgl.h>
#include <mooncake_log.h>
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

    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x050912), LV_PART_MAIN);
    lv_obj_remove_flag(lv_screen_active(), LV_OBJ_FLAG_SCROLLABLE);

    _title = std::make_unique<Label>(lv_screen_active());
    _title->setTextColor(lv_color_hex(0xFFFFFF));
    _title->setTextFont(&lv_font_montserrat_24);
    _title->align(LV_ALIGN_CENTER, 0, -55);
    _title->setText("SPECIAL CHESS");

    _subtitle = std::make_unique<Label>(lv_screen_active());
    _subtitle->setTextColor(lv_color_hex(0xAEB8C8));
    _subtitle->setTextFont(&lv_font_montserrat_24);
    _subtitle->align(LV_ALIGN_CENTER, 0, 5);
    _subtitle->setText("THE NEW CHESS EXPERIENCE");

    _version = std::make_unique<Label>(lv_screen_active());
    _version->setTextColor(lv_color_hex(0x64748B));
    _version->setTextFont(&lv_font_montserrat_24);
    _version->align(LV_ALIGN_BOTTOM_MID, 0, -40);
    _version->setText("SC DEVICE  V0.1");
}

void SCDevice::onRunning()
{
}

void SCDevice::onClose()
{
    LvglLockGuard lock;
    _title.reset();
    _subtitle.reset();
    _version.reset();
}
