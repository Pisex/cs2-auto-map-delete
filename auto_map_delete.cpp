#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>

#include "auto_map_delete.h"
#include "metamod_oslink.h"

AutoMapDelete g_AutoMapDelete;
PLUGIN_EXPOSE(AutoMapDelete, g_AutoMapDelete);

IVEngineServer2* engine = nullptr;

bool AutoMapDelete::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
    GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);

    g_SMAPI->AddListener(this, this);

	char szPath[256];
	g_SMAPI->PathFormat(szPath, sizeof(szPath), "%s/addons/configs/AutoClearMaps/whitelist.txt", g_SMAPI->GetBaseDir());

	if (!std::filesystem::exists(szPath))
	{
		Msg("[AutoClearMaps v1.0.0] Ошибка: whitelist.txt не найден \n");
		Msg("[AutoClearMaps v1.0.0] Ошибка: whitelist.txt не найден \n");
		Msg("[AutoClearMaps v1.0.0] Ошибка: whitelist.txt не найден \n");
		Msg("[AutoClearMaps v1.0.0] Ошибка: whitelist.txt не найден \n");
		Msg("[AutoClearMaps v1.0.0] Ошибка: whitelist.txt не найден \n");
		return false;
	}

	std::vector<std::string> whitelist;
	std::ifstream infile(szPath);
	std::string line;
	
	while (std::getline(infile, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
	
		if (!line.empty()) {
			Msg("[AutoClearMaps v1.0.0] whitelist: %s\n", line.c_str());
			whitelist.push_back(line);
		}
	}

	char szPathMaps[256];
	g_SMAPI->PathFormat(szPathMaps, sizeof(szPathMaps), "%s/maps", g_SMAPI->GetBaseDir());
	DeleteMaps(szPathMaps, whitelist, false);
	g_SMAPI->PathFormat(szPathMaps, sizeof(szPathMaps), "%s/../csgo_community_addons", g_SMAPI->GetBaseDir());
	DeleteMaps(szPathMaps, whitelist, true);
	
    return true;
}

bool AutoMapDelete::Unload(char* error, size_t maxlen)
{
    return true;
}

void AutoMapDelete::DeleteMaps(const std::string& folderPath, const std::vector<std::string>& whitelist, bool bFolders)
{
    try
    {
        if (!std::filesystem::exists(folderPath)) return;

        for (const auto& entry : std::filesystem::directory_iterator(folderPath))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().stem().string();

                if (std::find(whitelist.begin(), whitelist.end(), filename) == whitelist.end())
                {
                    std::filesystem::remove(entry.path());
                }
            }
            else if (entry.is_directory() && bFolders)
            {
                std::string folderName = entry.path().filename().string();

                if (std::find(whitelist.begin(), whitelist.end(), folderName) == whitelist.end())
                {
                    std::filesystem::remove_all(entry.path());
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << "[AutoClearMaps v1.0.0] Ошибка при удалении карт или папок: " << ex.what() << std::endl;
    }
}

///////////////////////////////////////
const char* AutoMapDelete::GetLicense()
{
	return "GPL";
}

const char* AutoMapDelete::GetVersion()
{
	return "1.0";
}

const char* AutoMapDelete::GetDate()
{
	return __DATE__;
}

const char *AutoMapDelete::GetLogTag()
{
	return "AutoMapDelete";
}

const char* AutoMapDelete::GetAuthor()
{
	return "Pisex";
}

const char* AutoMapDelete::GetDescription()
{
	return "AutoMapDelete";
}

const char* AutoMapDelete::GetName()
{
	return "Auto Map Delete";
}

const char* AutoMapDelete::GetURL()
{
	return "https://discord.gg/g798xERK5Y";
}
