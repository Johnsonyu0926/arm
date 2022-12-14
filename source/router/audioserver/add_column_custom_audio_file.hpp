#pragma once

#include <iostream>
#include <fstream>
#include "json.hpp"
#include "add_custom_audio_file.hpp"

using json = nlohmann::json;

namespace asns
{
    const std::string ADD_COLUMN_CUSTOM_AUDIO_FILE = "/etc/config/add_column_custom_audio_file.json";

    class CAddColumnCustomAudioFileData
    {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAddColumnCustomAudioFileData, fileName)

        int setName(const std::string &name)
        {
            fileName = name;
        }
        std::string getName() const
        {
            return fileName;
        }

    private:
        std::string fileName;
    };

    class CAddColumnCustomAudioFileBusiness
    {
    public:
        int Columnload()
        {
            std::ifstream i(ADD_COLUMN_CUSTOM_AUDIO_FILE);
            if (!i.is_open())
            {
                return 0;
            }
            json js;
            i >> js;
            business = js;
        }
        int load()
        {
            std::ifstream i(ADD_COLUMN_CUSTOM_AUDIO_FILE);
            if (!i.is_open())
            {
                return 0;
            }
            json js;
            i >> js;
            business = js;

            CAddCustomAudioFileBusiness bus;
            bus.load();
            for (const auto &it : bus.business)
            {
                CAddColumnCustomAudioFileData data;
                data.setName(it.customAudioName);
                business.push_back(data);
            }
        }

        void saveJson()
        {
            std::ofstream o(ADD_COLUMN_CUSTOM_AUDIO_FILE);
            json js = business;
            o << js << std::endl;
        }

        int isNameEmpty(const std::string &name)
        {
            load();
            for (const auto &it : business)
            {
                if (it.getName() == name)
                {
                    return 1;
                }
            }

            CAddCustomAudioFileBusiness bus;
            bus.load();
            for (const auto &it : bus.business)
            {
                if (it.filename == name)
                {
                    return 1;
                }
            }
            return 0;
        }

    public:
        std::vector<CAddColumnCustomAudioFileData> business;
    };

} // namespace asms