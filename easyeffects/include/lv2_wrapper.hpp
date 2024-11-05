/*
 *  优化日期: 2023年10月27日
 */

#pragma once

#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <lilv/lilv.h>
#include <lv2/buf-size/buf-size.h>
#include <lv2/core/lv2.h>
#include <lv2/ui/ui.h>
#include <lv2/urid/urid.h>
#include <sys/types.h>
#include <array>
#include <functional>
#include <limits>
#include <mutex>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>
#include "string_literal_wrapper.hpp"
#include "util.hpp"

namespace lv2 {

using namespace std::string_literals;

#define LV2_UI_makeResident LV2_UI_PREFIX "makeResident"
#define LV2_UI_makeSONameResident LV2_UI_PREFIX "makeSONameResident"

enum PortType { TYPE_CONTROL, TYPE_AUDIO, TYPE_ATOM };

struct Port {
  PortType type;  // Datatype
  uint index;  // Port index
  std::string name;
  std::string symbol;
  float value = 0.0F;  // Control value (if applicable)
  float min = -std::numeric_limits<float>::infinity();
  float max = std::numeric_limits<float>::infinity();
  bool is_input;  // True if an input port
  bool optional;  // True if the connection is optional
};

class Lv2Wrapper {
 public:
  explicit Lv2Wrapper(const std::string& plugin_uri);
  Lv2Wrapper(const Lv2Wrapper&) = delete;
  auto operator=(const Lv2Wrapper&) -> Lv2Wrapper& = delete;
  Lv2Wrapper(Lv2Wrapper&&) = delete;
  auto operator=(Lv2Wrapper&&) -> Lv2Wrapper& = delete;
  virtual ~Lv2Wrapper();

  bool found_plugin = false;

  bool create_instance(uint rate);
  void set_n_samples(uint value);
  uint get_n_samples() const;
  uint get_rate() const;

  void connect_data_ports(std::span<float> left_in,
                          std::span<float> right_in,
                          std::span<float> left_out,
                          std::span<float> right_out);

  void connect_data_ports(std::span<float> left_in,
                          std::span<float> right_in,
                          std::span<float> left_out,
                          std::span<float> right_out,
                          std::span<float> probe_left,
                          std::span<float> probe_right);

  void activate();
  void run() const;
  void deactivate();
  void set_control_port_value(const std::string& symbol, float value);
  float get_control_port_value(const std::string& symbol);
  bool has_instance();
  void load_ui();
  void notify_ui();
  void update_ui();
  bool has_ui();
  void close_ui();
  void set_ui_update_rate(uint value);
  void ui_port_event(uint port_index, float value);
  void native_ui_to_gsettings();

  template <StringLiteralWrapper key_wrapper, StringLiteralWrapper gkey_wrapper>
  void bind_key_bool(GSettings* settings) {
    set_control_port_value(key_wrapper.msg.data(),
                           static_cast<float>(g_settings_get_boolean(settings, gkey_wrapper.msg.data())));

    g_signal_connect(settings, ("changed::"s + gkey_wrapper.msg.data()).c_str(),
                     G_CALLBACK(+[](GSettings* settings, char* key, gpointer user_data) {
                       auto* self = static_cast<Lv2Wrapper*>(user_data);
                       self->set_control_port_value(key_wrapper.msg.data(),
                                                    static_cast<float>(g_settings_get_boolean(settings, key)));
                     }),
                     this);

    auto gkey = gkey_wrapper.msg.data();
    auto key = key_wrapper.msg.data();

    gsettings_sync_funcs.emplace_back([settings, gkey, key, this]() {
      g_settings_set_boolean(settings, gkey, static_cast<gboolean>(get_control_port_value(key)));
    });
  }

  template <StringLiteralWrapper key_wrapper, StringLiteralWrapper gkey_wrapper>
  void bind_key_enum(GSettings* settings) {
    set_control_port_value(key_wrapper.msg.data(),
                           static_cast<float>(g_settings_get_enum(settings, gkey_wrapper.msg.data())));

    g_signal_connect(settings, ("changed::"s + gkey_wrapper.msg.data()).c_str(),
                     G_CALLBACK(+[](GSettings* settings, char* key, gpointer user_data) {
                       auto* self = static_cast<Lv2Wrapper*>(user_data);
                       self->set_control_port_value(key_wrapper.msg.data(),
                                                    static_cast<float>(g_settings_get_enum(settings, key)));
                     }),
                     this);

    auto gkey = gkey_wrapper.msg.data();
    auto key = key_wrapper.msg.data();

    gsettings_sync_funcs.emplace_back([settings, gkey, key, this]() {
      g_settings_set_enum(settings, gkey, static_cast<gint>(get_control_port_value(key)));
    });
  }

  template <StringLiteralWrapper key_wrapper, StringLiteralWrapper gkey_wrapper>
  void bind_key_int(GSettings* settings) {
    set_control_port_value(key_wrapper.msg.data(),
                           static_cast<float>(g_settings_get_int(settings, gkey_wrapper.msg.data())));

    g_signal_connect(settings, ("changed::"s + gkey_wrapper.msg.data()).c_str(),
                     G_CALLBACK(+[](GSettings* settings, char* key, gpointer user_data) {
                       auto* self = static_cast<Lv2Wrapper*>(user_data);
                       self->set_control_port_value(key_wrapper.msg.data(),
                                                    static_cast<float>(g_settings_get_int(settings, key)));
                     }),
                     this);

    auto gkey = gkey_wrapper.msg.data();
    auto key = key_wrapper.msg.data();

    gsettings_sync_funcs.emplace_back([settings, gkey, key, this]() {
      g_settings_set_int(settings, gkey, static_cast<gint>(get_control_port_value(key)));
    });
  }

  template <StringLiteralWrapper key_wrapper, StringLiteralWrapper gkey_wrapper>
  void bind_key_double(GSettings* settings) {
    set_control_port_value(key_wrapper.msg.data(),
                           static_cast<float>(g_settings_get_double(settings, gkey_wrapper.msg.data())));

    g_signal_connect(settings, ("changed::"s + gkey_wrapper.msg.data()).c_str(),
                     G_CALLBACK(+[](GSettings* settings, char* key, gpointer user_data) {
                       auto* self = static_cast<Lv2Wrapper*>(user_data);
                       self->set_control_port_value(key_wrapper.msg.data(),
                                                    static_cast<float>(g_settings_get_double(settings, key)));
                     }),
                     this);

    auto gkey = gkey_wrapper.msg.data();
    auto key = key_wrapper.msg.data();

    gsettings_sync_funcs.emplace_back([settings, gkey, key, this]() {
      g_settings_set_double(settings, gkey, static_cast<gdouble>(get_control_port_value(key)));
    });
  }

  template <StringLiteralWrapper key_wrapper, StringLiteralWrapper gkey_wrapper, bool lower_bound = true>
  void bind_key_double_db(GSettings* settings) {
    auto key_v = g_settings_get_double(settings, gkey_wrapper.msg.data());
    auto linear_v = (!lower_bound && key_v <= util::minimum_db_d_level) ? 0.0F : static_cast<float>(util::db_to_linear(key_v));
    set_control_port_value(key_wrapper.msg.data(), linear_v);

    g_signal_connect(settings, ("changed::"s + gkey_wrapper.msg.data()).c_str(),
                     G_CALLBACK(+[](GSettings* settings, char* key, gpointer user_data) {
                       auto* self = static_cast<Lv2Wrapper*>(user_data);
                       auto key_v = g_settings_get_double(settings, gkey_wrapper.msg.data());
                       auto linear_v = (!lower_bound && key_v <= util::minimum_db_d_level)
                                           ? 0.0F
                                           : static_cast<float>(util::db_to_linear(key_v));
                       self->set_control_port_value(key_wrapper.msg.data(), linear_v);
                     }),
                     this);

    auto gkey = gkey_wrapper.msg.data();
    auto key = key_wrapper.msg.data();

    gsettings_sync_funcs.emplace_back([settings, gkey, key, this]() {
      const auto linear_v = get_control_port_value(key);
      const auto db_v = (!lower_bound & (linear_v == 0.0F)) ? util::minimum_db_d_level : util::linear_to_db(linear_v);
      g_settings_set_double(settings, gkey, static_cast<gdouble>(db_v));
    });
  }

 private:
  std::string plugin_uri;
  LilvWorld* world = nullptr;
  const LilvPlugin* plugin = nullptr;
  LilvInstance* instance = nullptr;
  LV2UI_Handle ui_handle = nullptr;
  const LV2UI_Descriptor* ui_descriptor = nullptr;
  const LV2UI_Idle_Interface* idle_iface = nullptr;
  void* libhandle = nullptr;
  uint n_ports = 0U;
  uint n_audio_in = 0U;
  uint n_audio_out = 0U;
  uint n_samples = 0U;
  uint rate = 0U;
  uint ui_update_rate = 30U;
  std::vector<Port> ports;

  // Multiband compressor/gate use 1+8*7=57 control ports. Round up to 64.
  std::array<std::pair<size_t, uint>, 64> control_ports_cache;

  struct {
    struct { uint left, right; } in;
    struct { uint left, right; } probe;
    struct { uint left, right; } out;
  } data_ports;

  std::vector<std::function<void()>> gsettings_sync_funcs;

  std::unordered_map<std::string, LV2_URID> map_uri_to_urid;
  std::unordered_map<LV2_URID, std::string> map_urid_to_uri;

  std::array<const LV2_Feature, 1U> static_features{{{LV2_BUF_SIZE__boundedBlockLength, nullptr}}};

  std::mutex ui_mutex;

  void check_required_features();
  void create_ports();
  void connect_control_ports();
  LV2_URID map_urid(const std::string& uri);
};

}  // namespace lv2

/*
 *  优化日期: 2023年10月27日
 */