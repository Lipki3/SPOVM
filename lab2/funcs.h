#ifndef LAB2
#define LAB2

#include <functional>
#include <string>
#include <vector>
#include <dirent.h>

using namespace std;

struct Flags {
    bool d = false;
    bool l = false;
    bool f = false;
    bool s = false;
};
struct Types {
    enum class Type {
        Other, Dir, Link, File
    } type{Type::Other};
    string name;
    Types(string name);
    Types(int type, string name);
};

string GoToDir(int argc, char* argv[]);
Flags Choice(int argc, char* argv[]);

bool operator<(const Types& lhs, const Types& rhs);
using Type = function<bool(int dt_type)>;
Type Create(const Flags& flags);

using List = vector<Types>;
void GetList(const string& dir, List& list, const Type& flag);
void Print(List& list);


#endif //LAB2