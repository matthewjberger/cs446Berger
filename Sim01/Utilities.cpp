#include "Utilities.h"
using namespace std;

bool ContainsString(string source, string searchTerm)
{
    if(source.find(searchTerm) != std::string::npos)
    {
        return true;
    }
    else
    {
        return false;
    }
}

string GetDataFromLine(string input)
{
    if(!ContainsString(input, ":"))
    {
        return "";
    }

    return input.substr(input.find(':') + 2);
}


