#pragma once

#include <mooncake.h>
#include <memory>
#include "lvgl_cpp/label.h"
#include <smooth_ui_toolkit.h>
#include <smooth_lvgl.h>

class SCDevice : public mooncake::AppAbility {
public:
    SCDevice();

    void onCreate() override;
    void onOpen() override;
    void onRunning() override;
    void onClose() override;

private:
    std::unique_ptr<smooth_ui_toolkit::lvgl_cpp::Label> _title;
    std::unique_ptr<smooth_ui_toolkit::lvgl_cpp::Label> _subtitle;
    std::unique_ptr<smooth_ui_toolkit::lvgl_cpp::Label> _version;
};
