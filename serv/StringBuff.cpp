#include "mylib.hpp"
#include "StringBuff.hpp"

using namespace std;

void StringBuff::write(int n, const char *p)
{
    string S = p;
    if (buff.find(n) == buff.end())
    {
        buff[n] = "";
    }
    buff[n] += S;
    return;
}

string StringBuff::read(int n)
{
    size_t splitpt;
    if (buff.find(n) == buff.end())
    {
        return "";
    }
    if ((splitpt = buff[n].find('\n')) == string::npos)
    {
        return "";
    }
    string rt = buff[n].substr(0, splitpt + 1);
    buff[n] = buff[n].substr(splitpt + 1);
    return rt;
}

void StringBuff::clear(int n)
{
    if (buff.find(n) == buff.end())
    {
        buff[n] = "";
    }
    buff[n] = "";
}