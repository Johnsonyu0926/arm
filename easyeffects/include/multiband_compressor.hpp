/*
 *  优化日期: 2023年10月27日
 */

#include "multiband_compressor.h"
#include <iostream>

MultibandCompressor::MultibandCompressor(const std::string& tag, const std::string& schema, const std::string& schema_path,
                                         PipeManager* pipe_manager, PipelineType pipe_type)
    : PluginBase(tag, schema, schema_path, pipe_manager, pipe_type), latency_n_frames(0U) {
}

MultibandCompressor::~MultibandCompressor() {
    for (auto* proxy : list_proxies) {
        if (proxy) pw_proxy_destroy(proxy);
    }
}

void MultibandCompressor::setup() {
    // Implementation of setup logic
}

void MultibandCompressor::process(std::span<float>& left_in, std::span<float>& right_in, std::span<float>& left_out,
                                  std::span<float>& right_out, std::span<float>& probe_left, std::span<float>& probe_right) {
    // Implementation of process logic
}

auto MultibandCompressor::get_latency_seconds() -> float {
    return static_cast<float>(latency_n_frames) / sample_rate;
}

void MultibandCompressor::update_probe_links() {
    // Implementation of update_probe_links logic
}

void MultibandCompressor::update_sidechain_links(const std::string& key) {
    // Implementation of update_sidechain_links logic
}

template <size_t n>
constexpr void MultibandCompressor::bind_band() {
    using namespace tags::multiband_compressor;
    using namespace std::string_literals;

    if constexpr (n > 0U) {
        lv2_wrapper->bind_key_bool<cbe[n], band_enable[n]>(settings);
        lv2_wrapper->bind_key_double<sf[n], band_split_frequency[n]>(settings);
    }

    lv2_wrapper->bind_key_bool<sce[n], band_external_sidechain[n]>(settings);
    lv2_wrapper->bind_key_bool<sclc[n], band_lowcut_filter[n]>(settings);
    lv2_wrapper->bind_key_bool<schc[n], band_highcut_filter[n]>(settings);
    lv2_wrapper->bind_key_bool<ce[n], band_compressor_enable[n]>(settings);
    lv2_wrapper->bind_key_bool<bs[n], band_solo[n]>(settings);
    lv2_wrapper->bind_key_bool<bm[n], band_mute[n]>(settings);
    lv2_wrapper->bind_key_enum<sscs[n], band_stereo_split_source[n]>(settings);
    lv2_wrapper->bind_key_enum<scs[n], band_sidechain_source[n]>(settings);
    lv2_wrapper->bind_key_enum<scm[n], band_sidechain_mode[n]>(settings);
    lv2_wrapper->bind_key_enum<cm[n], band_compression_mode[n]>(settings);
    lv2_wrapper->bind_key_double<sla[n], band_sidechain_lookahead[n]>(settings);
    lv2_wrapper->bind_key_double<scr[n], band_sidechain_reactivity[n]>(settings);
    lv2_wrapper->bind_key_double<sclf[n], band_lowcut_filter_frequency[n]>(settings);
    lv2_wrapper->bind_key_double<schf[n], band_highcut_filter_frequency[n]>(settings);
    lv2_wrapper->bind_key_double<at[n], band_attack_time[n]>(settings);
    lv2_wrapper->bind_key_double<rt[n], band_release_time[n]>(settings);
    lv2_wrapper->bind_key_double<cr[n], band_ratio[n]>(settings);
    lv2_wrapper->bind_key_double_db<scp[n], band_sidechain_preamp[n]>(settings);
    lv2_wrapper->bind_key_double_db<al[n], band_attack_threshold[n]>(settings);
    lv2_wrapper->bind_key_double_db<kn[n], band_knee[n]>(settings);
    lv2_wrapper->bind_key_double_db<bth[n], band_boost_threshold[n]>(settings);
    lv2_wrapper->bind_key_double_db<bsa[n], band_boost_amount[n]>(settings);
    lv2_wrapper->bind_key_double_db<mk[n], band_makeup[n]>(settings);
    lv2_wrapper->bind_key_double_db<rrl[n], band_release_threshold[n], false>(settings);

    gconnections.emplace_back(g_signal_connect(settings, ("changed::"s + band_external_sidechain[n].data()).c_str(),
                                                G_CALLBACK(+[](GSettings* settings, char* key, gpointer user_data) {
                                                    auto* self = static_cast<MultibandCompressor*>(user_data);
                                                    self->update_sidechain_links(key);
                                                }),
                                                this));
}

template <size_t... Ns>
constexpr void MultibandCompressor::bind_bands(std::index_sequence<Ns...> /*unused*/) {
    (bind_band<Ns>(), ...);
}

/*
 *  优化日期: 2023年10月27日
 */