// Optimized on 2024/10/27

#include "plugin_base.hpp"
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <pipewire/filter.h>
#include <pipewire/keys.h>
#include <pipewire/loop.h>
#include <pipewire/port.h>
#include <pipewire/properties.h>
#include <pipewire/thread-loop.h>
#include <spa/node/io.h>
#include <spa/param/latency-utils.h>
#include <spa/param/latency.h>
#include <spa/param/param.h>
#include <spa/pod/builder.h>
#include <spa/pod/pod.h>
#include <spa/support/loop.h>
#include <spa/utils/defs.h>
#include <spa/utils/hook.h>
#include <sys/types.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <thread>
#include <utility>
#include "pipe_manager.hpp"
#include "tags_app.hpp"
#include "tags_plugin_name.hpp"
#include "util.hpp"

namespace {

void on_process(void* userdata, spa_io_position* position) {
  auto* d = static_cast<PluginBase::data*>(userdata);

  const auto n_samples = position->clock.duration;
  const auto rate = position->clock.rate.denom;

  if (n_samples == 0 || rate == 0) {
    return;
  }

  if (rate != d->pb->rate || n_samples != d->pb->n_samples) {
    d->pb->rate = rate;
    d->pb->n_samples = n_samples;

    d->pb->dummy_left.resize(n_samples);
    d->pb->dummy_right.resize(n_samples);

    std::ranges::fill(d->pb->dummy_left, 0.0F);
    std::ranges::fill(d->pb->dummy_right, 0.0F);

    d->pb->clock_start = std::chrono::system_clock::now();

    d->pb->setup();
  }

  d->pb->delta_t = 0.001F * static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                   std::chrono::system_clock::now() - d->pb->clock_start)
                                                   .count());

  d->pb->send_notifications = d->pb->delta_t >= d->pb->notification_time_window;

  auto* in_left = static_cast<float*>(pw_filter_get_dsp_buffer(d->in_left, n_samples));
  auto* in_right = static_cast<float*>(pw_filter_get_dsp_buffer(d->in_right, n_samples));

  auto* out_left = static_cast<float*>(pw_filter_get_dsp_buffer(d->out_left, n_samples));
  auto* out_right = static_cast<float*>(pw_filter_get_dsp_buffer(d->out_right, n_samples));

  std::span<float> left_in = in_left ? std::span(in_left, n_samples) : d->pb->dummy_left;
  std::span<float> right_in = in_right ? std::span(in_right, n_samples) : d->pb->dummy_right;
  std::span<float> left_out = out_left ? std::span(out_left, n_samples) : d->pb->dummy_left;
  std::span<float> right_out = out_right ? std::span(out_right, n_samples) : d->pb->dummy_right;

  if (!d->pb->enable_probe) {
    d->pb->process(left_in, right_in, left_out, right_out);
  } else {
    auto* probe_left = static_cast<float*>(pw_filter_get_dsp_buffer(d->probe_left, n_samples));
    auto* probe_right = static_cast<float*>(pw_filter_get_dsp_buffer(d->probe_right, n_samples));

    std::span<float> l = probe_left ? std::span(probe_left, n_samples) : d->pb->dummy_left;
    std::span<float> r = probe_right ? std::span(probe_right, n_samples) : d->pb->dummy_right;

    d->pb->process(left_in, right_in, left_out, right_out, l, r);
  }

  if (d->pb->send_notifications) {
    d->pb->clock_start = std::chrono::system_clock::now();
    d->pb->send_notifications = false;
  }
}

auto update_filter(struct spa_loop* loop, bool async, uint32_t seq, const void* data, size_t size, void* user_data)
    -> int {
  auto* self = static_cast<PluginBase*>(user_data);

  spa_process_latency_info latency_info{};
  latency_info.ns = static_cast<uint64_t>(self->latency_value * 1000000000.0F);

  std::array<char, 1024U> buffer{};
  spa_pod_builder b{};
  spa_pod_builder_init(&b, buffer.data(), sizeof(buffer));

  const spa_pod* param = spa_process_latency_build(&b, SPA_PARAM_ProcessLatency, &latency_info);
  pw_filter_update_params(self->filter, nullptr, &param, 1);

  return 0;
}

void on_filter_state_changed(void* userdata, pw_filter_state old, pw_filter_state state, const char* error) {
  auto* d = static_cast<PluginBase::data*>(userdata);
  d->pb->state = state;

  switch (state) {
    case PW_FILTER_STATE_ERROR:
    case PW_FILTER_STATE_UNCONNECTED:
    case PW_FILTER_STATE_CONNECTING:
      d->pb->can_get_node_id = false;
      break;
    case PW_FILTER_STATE_STREAMING:
    case PW_FILTER_STATE_PAUSED:
      d->pb->can_get_node_id = true;
      break;
    default:
      break;
  }
}

const struct pw_filter_events filter_events = {.state_changed = on_filter_state_changed, .process = on_process};

}  // namespace

PluginBase::PluginBase(std::string tag,
                       std::string plugin_name,
                       std::string package,
                       const std::string& schema,
                       const std::string& schema_path,
                       PipeManager* pipe_manager,
                       PipelineType pipe_type,
                       const bool& enable_probe)
    : log_tag(std::move(tag)),
      name(std::move(plugin_name)),
      package(std::move(package)),
      pipeline_type(pipe_type),
      enable_probe(enable_probe),
      settings(g_settings_new_with_path(schema.c_str(), schema_path.c_str())),
      global_settings(g_settings_new(tags::app::id)),
      pm(pipe_manager) {
  std::string description;

  if (name != "output_level" && name != "spectrum") {
    description = tags::plugin_name::get_translated()[name];
    bypass = g_settings_get_boolean(settings, "bypass") != 0;

    gconnections.push_back(g_signal_connect(settings, "changed::bypass",
                                            G_CALLBACK(+[](GSettings* settings, char* key, gpointer user_data) {
                                              auto* self = static_cast<PluginBase*>(user_data);
                                              self->bypass = g_settings_get_boolean(settings, "bypass") != 0;
                                            }),
                                            this));
  } else if (name == "output_level") {
    description = _("Output Level Meter");
  } else if (name == "spectrum") {
    description = _("Spectrum");
  }

  pf_data.pb = this;

  const auto filter_name = "ee_" + log_tag.substr(0U, log_tag.size() - 2U) + "_" + name;

  pm->lock();

  auto* props_filter = pw_properties_new(nullptr, nullptr);
  pw_properties_set(props_filter, PW_KEY_APP_ID, tags::app::id);
  pw_properties_set(props_filter, PW_KEY_NODE_NAME, filter_name.c_str());
  pw_properties_set(props_filter, PW_KEY_NODE_NICK, name.c_str());
  pw_properties_set(props_filter, PW_KEY_NODE_DESCRIPTION, description.c_str());
  pw_properties_set(props_filter, PW_KEY_MEDIA_TYPE, "Audio");
  pw_properties_set(props_filter, PW_KEY_MEDIA_CATEGORY, "Filter");
  pw_properties_set(props_filter, PW_KEY_MEDIA_ROLE, "DSP");
  pw_properties_set(props_filter, PW_KEY_NODE_PASSIVE, "true");

  filter = pw_filter_new(pm->core, filter_name.c_str(), props_filter);

  auto create_port = [&](const char* port_name, const char* channel, pw_direction direction) {
    auto* props = pw_properties_new(nullptr, nullptr);
    pw_properties_set(props, PW_KEY_FORMAT_DSP, "32 bit float mono audio");
    pw_properties_set(props, PW_KEY_PORT_NAME, port_name);
    pw_properties_set(props, "audio.channel", channel);
    return static_cast<port*>(pw_filter_add_port(filter, direction, PW_FILTER_PORT_FLAG_MAP_BUFFERS, sizeof(port), props, nullptr, 0));
  };

  pf_data.in_left = create_port("input_FL", "FL", PW_DIRECTION_INPUT);
  pf_data.in_right = create_port("input_FR", "FR", PW_DIRECTION_INPUT);
  pf_data.out_left = create_port("output_FL", "FL", PW_DIRECTION_OUTPUT);
  pf_data.out_right = create_port("output_FR", "FR", PW_DIRECTION_OUTPUT);

  if (enable_probe) {
    n_ports += 2;
    pf_data.probe_left = create_port("probe_FL", "PROBE_FL", PW_DIRECTION_INPUT);
    pf_data.probe_right = create_port("probe_FR", "PROBE_FR", PW_DIRECTION_INPUT);
  }

  pm->sync_wait_unlock();
}

PluginBase::~PluginBase() {
  post_messages = false;

  pm->lock();

  if (listener.link.next != nullptr || listener.link.prev != nullptr) {
    spa_hook_remove(&listener);
  }

  pw_filter_destroy(filter);

  pm->sync_wait_unlock();

  for (auto& handler_id : gconnections) {
    g_signal_handler_disconnect(settings, handler_id);
  }

  gconnections.clear();

  g_object_unref(settings);
}

void PluginBase::set_post_messages(const bool& state) {
  post_messages = state;
}

void PluginBase::reset_settings() {
  util::reset_all_keys_except(settings);
}

auto PluginBase::connect_to_pw() -> bool {
  connected_to_pw = false;
  can_get_node_id = false;
  state = PW_FILTER_STATE_UNCONNECTED;

  pm->lock();

  if (pw_filter_connect(filter, PW_FILTER_FLAG_RT_PROCESS, nullptr, 0) != 0) {
    pm->unlock();
    util::warning(log_tag + name + " cannot connect the filter to PipeWire!");
    return false;
  }

  initialize_listener();
  pm->sync_wait_unlock();

  while (!can_get_node_id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if (state == PW_FILTER_STATE_ERROR) {
      util::warning(log_tag + name + " is in an error");
      return false;
    }
  }

  pm->lock();
  node_id = pw_filter_get_node_id(filter);
  pm->sync_wait_unlock();

  while (pm->count_node_ports(node_id) != n_ports) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  connected_to_pw = true;
  util::debug(log_tag + name + " successfully connected to PipeWire graph");
  return true;
}

void PluginBase::initialize_listener() {
  pw_filter_add_listener(filter, &listener, &filter_events, &pf_data);
}

auto PluginBase::get_node_id() const -> uint {
  return node_id;
}

void PluginBase::set_active(const bool& state) const {
  pw_filter_set_active(filter, state);
}

void PluginBase::disconnect_from_pw() {
  pm->lock();
  set_active(false);

  if (listener.link.next != nullptr || listener.link.prev != nullptr) {
    spa_hook_remove(&listener);
  }

  pw_filter_disconnect(filter);
  connected_to_pw = false;
  pm->sync_wait_unlock();
  node_id = SPA_ID_INVALID;
}

void PluginBase::setup() {}

void PluginBase::process(std::span<float>& left_in,
                         std::span<float>& right_in,
                         std::span<float>& left_out,
                         std::span<float>& right_out) {}

void PluginBase::process(std::span<float>& left_in,
                         std::span<float>& right_in,
                         std::span<float>& left_out,
                         std::span<float>& right_out,
                         std::span<float>& probe_left,
                         std::span<float>& probe_right) {}

auto PluginBase::get_latency_seconds() -> float {
  return 0.0F;
}

void PluginBase::show_native_ui() {
  if (lv2_wrapper == nullptr) {
    return;
  }

  if (!lv2_wrapper->has_ui()) {
    lv2_wrapper->load_ui();
  }
}

void PluginBase::close_native_ui() {
  if (lv2_wrapper == nullptr) {
    return;
  }

  lv2_wrapper->native_ui_to_gsettings();
  lv2_wrapper->close_ui();
}

void PluginBase::set_native_ui_update_frequency(const uint& value) {
  if (lv2_wrapper == nullptr) {
    return;
  }

  lv2_wrapper->set_ui_update_rate(value);
}

void PluginBase::get_peaks(const std::span<float>& left_in,
                           const std::span<float>& right_in,
                           std::span<float>& left_out,
                           std::span<float>& right_out) {
  if (!post_messages) {
    return;
  }

  float peak_l = std::ranges::max(left_in);
  float peak_r = std::ranges::max(right_in);

  input_peak_left = std::max(peak_l, input_peak_left);
  input_peak_right = std::max(peak_r, input_peak_right);

  peak_l = std::ranges::max(left_out);
  peak_r = std::ranges::max(right_out);

  output_peak_left = std::max(peak_l, output_peak_left);
  output_peak_right = std::max(peak_r, output_peak_right);
}

void PluginBase::setup_input_output_gain() {
  input_gain = static_cast<float>(util::db_to_linear(g_settings_get_double(settings, "input-gain")));
  output_gain = static_cast<float>(util::db_to_linear(g_settings_get_double(settings, "output-gain")));

  g_signal_connect(settings, "changed::input-gain", G_CALLBACK(+[](GSettings* settings, char* key, gpointer user_data) {
                     auto* self = static_cast<PluginBase*>(user_data);
                     self->input_gain = util::db_to_linear(g_settings_get_double(settings, key));
                   }),
                   this);

  g_signal_connect(settings, "changed::output-gain",
                   G_CALLBACK(+[](GSettings* settings, char* key, gpointer user_data) {
                     auto* self = static_cast<PluginBase*>(user_data);
                     self->output_gain = util::db_to_linear(g_settings_get_double(settings, key));
                   }),
                   this);
}

void PluginBase::apply_gain(std::span<float>& left, std::span<float>& right, const float& gain) {
  if (left.empty() || right.empty()) {
    return;
  }

  std::ranges::for_each(left, [&](auto& v) { v *= gain; });
  std::ranges::for_each(right, [&](auto& v) { v *= gain; });
}

void PluginBase::notify() {
  const auto input_peak_db_l = util::linear_to_db(input_peak_left);
  const auto input_peak_db_r = util::linear_to_db(input_peak_right);

  const auto output_peak_db_l = util::linear_to_db(output_peak_left);
  const auto output_peak_db_r = util::linear_to_db(output_peak_right);

  input_level.emit(input_peak_db_l, input_peak_db_r);
  output_level.emit(output_peak_db_l, output_peak_db_r);

  input_peak_left = util::minimum_linear_level;
  input_peak_right = util::minimum_linear_level;
  output_peak_left = util::minimum_linear_level;
  output_peak_right = util::minimum_linear_level;
}

void PluginBase::update_probe_links() {}

void PluginBase::update_filter_params() {
  pw_loop_invoke(pw_thread_loop_get_loop(pm->thread_loop), update_filter, 1, nullptr, 0, false, this);
}

// Optimized on 2024/10/27