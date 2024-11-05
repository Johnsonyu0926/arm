#pragma once

namespace tags::pipewire {

inline constexpr auto ee_source_name = "easyeffects_source";
inline constexpr auto ee_sink_name = "easyeffects_sink";

namespace media_class {
inline constexpr auto device = "Audio/Device";
inline constexpr auto sink = "Audio/Sink";
inline constexpr auto source = "Audio/Source";
inline constexpr auto virtual_source = "Audio/Source/Virtual";
inline constexpr auto input_stream = "Stream/Input/Audio";
inline constexpr auto output_stream = "Stream/Output/Audio";
}  // namespace media_class

namespace media_role {
inline constexpr auto dsp = "DSP";
}  // namespace media_role

}  // namespace tags::pipewire

// By GST @2024/10/27