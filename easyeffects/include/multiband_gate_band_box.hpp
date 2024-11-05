#pragma once

#include <adwaita.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory> // for smart pointers

namespace ui::multiband_gate_band_box {

G_BEGIN_DECLS

#define EE_TYPE_MULTIBAND_GATE_BAND_BOX (multiband_gate_band_box_get_type())

G_DECLARE_FINAL_TYPE(MultibandGateBandBox, multiband_gate_band_box, EE, MULTIBAND_GATE_BAND_BOX, GtkBox)

G_END_DECLS

// 使用智能指针管理内存，防止内存泄漏
auto create() -> std::unique_ptr<MultibandGateBandBox>;

void setup(MultibandGateBandBox* self, GSettings* settings, int index);

// 设置标签值，增加边界检查和输入验证
void set_end_label(MultibandGateBandBox* self, float value);

void set_envelope_label(MultibandGateBandBox* self, float value);

void set_curve_label(MultibandGateBandBox* self, float value);

void set_gain_label(MultibandGateBandBox* self, float value);

}  // namespace ui::multiband_gate_band_box

/*
 *  优化日期: 2023年10月27日
 */