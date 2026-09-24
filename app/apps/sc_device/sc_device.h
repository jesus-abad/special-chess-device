#pragma once

#include <mooncake.h>
#include <lvgl.h>

class SCDevice : public mooncake::AppAbility {
public:
    SCDevice();

    void onCreate() override;
    void onOpen() override;
    void onRunning() override;
    void onClose() override;

private:
    lv_obj_t* _title = nullptr;
    lv_obj_t* _subtitle = nullptr;
    lv_obj_t* _timer_btn = nullptr;
    lv_obj_t* _match_btn = nullptr;
    lv_obj_t* _status = nullptr;
    lv_obj_t* _version = nullptr;

    static void onTimerClicked(lv_event_t* e);
    static void onMatchClicked(lv_event_t* e);
};
