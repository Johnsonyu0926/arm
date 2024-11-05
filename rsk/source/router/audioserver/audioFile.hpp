// audioFile.hpp
#pragma once

#include <string>
#include <nlohmann/json.hpp>

struct AudioFile {
    int type;
    std::string fileName;
    size_t size;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AudioFile, type, fileName, size)
};