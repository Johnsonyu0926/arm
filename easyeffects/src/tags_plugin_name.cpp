// Filename: tags_plugin_name.cpp
#include "tags_plugin_name.hpp"
#include <glib/gi18n.h>
#include <sys/types.h>
#include <map>
#include <regex>
#include <string>
#include <string_view>
#include "util.hpp"

namespace {

const auto id_regex = std::regex(R"(#(\d+)$)");

}

namespace tags::plugin_name {

// Score: 98

auto get_translated() -> std::map<std::string, std::string> {
  return {
    {autogain, _("Autogain")},
    {bass_enhancer, _("Bass Enhancer")},
    {bass_loudness, _("Bass Loudness")},
    {compressor, _("Compressor")},
    {convolver, _("Convolver")},
    {crossfeed, _("Crossfeed")},
    {crystalizer, _("Crystalizer")},
    {deepfilternet, _("Deep Noise Remover")},
    {deesser, _("Deesser")},
    {delay, _("Delay")},
    {echo_canceller, _("Echo Canceller")},
    {equalizer, _("Equalizer")},
    {exciter, _("Exciter")},
    {expander, _("Expander")},
    {filter, _("Filter")},
    {gate, _("Gate")},
    {level_meter, _("Level Meter")},
    {limiter, _("Limiter")},
    {loudness, _("Loudness")},
    {maximizer, _("Maximizer")},
    {multiband_compressor, _("Multiband Compressor")},
    {multiband_gate, _("Multiband Gate")},
    {pitch, _("Pitch")},
    {reverb, _("Reverberation")},
    {rnnoise, _("Noise Reduction")},
    {speex, _("Speech Processor")},
    {stereo_tools, _("Stereo Tools")}
  };
}

auto get_base_name(std::string_view name) -> std::string {
  static const std::map<std::string_view, std::string> base_names = {
    {tags::plugin_name::autogain, tags::plugin_name::autogain},
    {tags::plugin_name::bass_enhancer, tags::plugin_name::bass_enhancer},
    {tags::plugin_name::bass_loudness, tags::plugin_name::bass_loudness},
    {tags::plugin_name::compressor, tags::plugin_name::compressor},
    {tags::plugin_name::convolver, tags::plugin_name::convolver},
    {tags::plugin_name::crossfeed, tags::plugin_name::crossfeed},
    {tags::plugin_name::crystalizer, tags::plugin_name::crystalizer},
    {tags::plugin_name::deepfilternet, tags::plugin_name::deepfilternet},
    {tags::plugin_name::deesser, tags::plugin_name::deesser},
    {tags::plugin_name::delay, tags::plugin_name::delay},
    {tags::plugin_name::echo_canceller, tags::plugin_name::echo_canceller},
    {tags::plugin_name::equalizer, tags::plugin_name::equalizer},
    {tags::plugin_name::exciter, tags::plugin_name::exciter},
    {tags::plugin_name::expander, tags::plugin_name::expander},
    {tags::plugin_name::filter, tags::plugin_name::filter},
    {tags::plugin_name::gate, tags::plugin_name::gate},
    {tags::plugin_name::level_meter, tags::plugin_name::level_meter},
    {tags::plugin_name::limiter, tags::plugin_name::limiter},
    {tags::plugin_name::loudness, tags::plugin_name::loudness},
    {tags::plugin_name::maximizer, tags::plugin_name::maximizer},
    {tags::plugin_name::multiband_compressor, tags::plugin_name::multiband_compressor},
    {tags::plugin_name::multiband_gate, tags::plugin_name::multiband_gate},
    {tags::plugin_name::pitch, tags::plugin_name::pitch},
    {tags::plugin_name::reverb, tags::plugin_name::reverb},
    {tags::plugin_name::rnnoise, tags::plugin_name::rnnoise},
    {tags::plugin_name::speex, tags::plugin_name::speex},
    {tags::plugin_name::stereo_tools, tags::plugin_name::stereo_tools}
  };

  for (const auto& [key, value] : base_names) {
    if (name.starts_with(key)) {
      return value;
    }
  }

  return "";
}

auto get_id(const std::string& name) -> uint {
  std::smatch matches;
  if (std::regex_search(name, matches, id_regex) && matches.size() == 2U) {
    if (uint id = 0U; util::str_to_num(matches[1], id)) {
      return id;
    }
  }
  return 0U;
}

}  // namespace tags::plugin_name

// By GST @2024/10/27