/************************************************************************************
_____________________________DEMONWARE COMPANION______________________________******
***********************************************************************************
**
** - Name        : ModernWarfareRemastered Handler Source
** - Description : Handler for dumping mwr livestorage files
** - Author      : Hosseinpourziyaie
**
** - Note        : define DEBUG to print out extended debug information
**
**
**
** [Copyright © Hosseinpourziyaie 2022] <hosseinpourziyaie@gmail.com>
**
************************************************************************************/
#include "stdinc.h"
#define DEBUG

std::map<std::string, uintptr_t> ModernWarfareRemastered::dwFileTaskList;

void ModernWarfareRemastered::StartUpExtraction(ProcHex process)
{
	dwFileTaskList.insert({ "g_fetchPlaylistsFileTask", 0x143432680 });
	dwFileTaskList.insert({ "g_fetchFFotDFileTask", 0x14A9BC9D0 });
	dwFileTaskList.insert({ "g_fetchMotdFileTask", 0x14AA759D0 });
	dwFileTaskList.insert({ "g_downloadStoreConfig", 0x1435ACE70 });
	dwFileTaskList.insert({ "commondata", 0x14342C280 });
	dwFileTaskList.insert({ "metplayerdata", 0x143438A30 });


	for (auto const& pair : dwFileTaskList) 
	{
		dwFileTask* fileTask = new dwFileTask{};
		if (process.ReadMemory(pair.second, fileTask, sizeof(dwFileTask)))
		{
			std::string filename_str = process.ReadString(reinterpret_cast<uint64_t>(fileTask->m_filename));

			if (fileTask->m_error == 1000)
			{
				std::cout << "[INFO] [" << pair.first << "] " << filename_str << " is not provided by demonware server (error code " << fileTask->m_error << ")" << std::endl;
			}
			else
			{
				bool succsseed = process.DumpMemory(reinterpret_cast<uint64_t>(fileTask->m_dataResult.m_fileData), fileTask->m_dataResult.m_fileSize, "h1_dwfiles\\" + filename_str);
				if(succsseed) std::cout << "[INFO] [" << pair.first << "] " << filename_str << " with total size of " << fileTask->m_dataResult.m_fileSize << " bytes dumped successfully" << std::endl;
				else std::cout << "[ERROR] [" << pair.first << "] failed to dump " << filename_str << "(size: " << fileTask->m_dataResult.m_fileSize << ")" << std::endl;
			}

#ifdef DEBUG
			if (fileTask->m_error) printf("** %i ** ", fileTask->m_error);
			printf("%s: %llX\n\n", pair.first.c_str(), pair.second);

			printf("%s->m_file: %s(%i)\n", pair.first.c_str(), process.ReadString(reinterpret_cast<uint64_t>(fileTask->m_filename)).data(), fileTask->m_fileSize);
			printf("%s->m_dataResult: %p(%i)\n", pair.first.c_str(), fileTask->m_dataResult.m_fileData, fileTask->m_dataResult.m_fileSize);
			printf("%s->m_buffer: %p(%i)\n", pair.first.c_str(), fileTask->m_buffer, fileTask->m_bufferSize);

			//printf("------------------------------------------------------------------------------------------------\n");
#endif
		}
		delete fileTask; std::cout << std::endl;
		//std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
	}
	std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
}

