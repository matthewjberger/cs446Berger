#ifndef __UTILITIES_H
#define __UTILITIES_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <queue>
#include <chrono>
#include <thread>
#include <ctime>
#include <pthread.h>
#include <unistd.h>

// Check if one string contains another string
bool ContainsString(std::string source, std::string searchTerm);

// Get everything in string after first occurence of ': '
std::string GetDataFromLine(std::string input);

#endif
