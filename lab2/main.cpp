#include "funcs.h"

int main(int argc, char* argv[]) {
    string GoTo = GoToDir(argc, argv);
    Flags flags = Choice(argc, argv);
    auto flag = Create(flags);
    vector<Types> list;
    GetList(GoTo, list, flag);
    Print(list);
}