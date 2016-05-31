// IMPORTANT: FIXME: I have not tested the WIN32 part, so it must contain errors.

#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>

#include "menu.hpp"

#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <errno.h>
#endif

#ifndef WIN32
// Filter function for scandir
int filter(const struct dirent *dir) {
	if (dir->d_type == DT_DIR && (dir->d_name)[0] != '.')
		return 1;
	else
		return 0;
}
#endif

namespace menu {
	std::string print(const char* arg) {
		std::vector<std::string> subgames;
#ifdef WIN32	
		HANDLE hFind;
		WIN32_FIND_DATA FindData;
		hFind = FindFirstFile("subgames/*", &FindData);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if ((FindData.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
					subgames.push_back(findfiledata.cFileName);
			} while (FindNextFile(hFind, &findfiledata) != 0);
		} else {
			std::cout << "Error while trying to get content of directory subgames (WIN32)." << std::endl;
			exit(EXIT_FAILURE);
		}
		FindClose(hFind);
#else
		struct dirent **namelist;
		int n;
		n = scandir("subgames", &namelist, filter, alphasort);
		if(n < 0) {
			perror("Error while trying to get content of directory subgames: scandir");
			exit(EXIT_FAILURE);
		}
		while(n--) {
			subgames.push_back(namelist[n]->d_name);
			free(namelist[n]);
		}
		free(namelist);
#endif
		
		if (arg != NULL) {
			if (std::find(subgames.begin(), subgames.end(), arg) == subgames.end())
				std::cout << "The subgame " << arg << " doesn't exist." << std::endl;
				// Continuing in the loop
			else
				return arg;
		}
		std::string subgame;
		while (true) {
			std::cout << "Available subgames:" << std::endl;
			for (int i = 0 ; i < subgames.size() ; i++) {
				std::cout << subgames[i] << std::endl;
			}
			std::cout << "What subgame do you want to play on ? ";
			std::cin >> subgame;
			if (std::find(subgames.begin(), subgames.end(), subgame) == subgames.end())
				std::cout << "The subgame " << subgame << " doesn't exist." << std::endl;
			else
				break; // Stopping the loop
		}
		
		return subgame;
	}
}
