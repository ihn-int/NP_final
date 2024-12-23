#include "mylib.hpp"

using namespace std;

class StringBuff
{
public:
    map<int, string> buff;
    void write(int, const char *);
    string read(int);
    void clear(int);
};