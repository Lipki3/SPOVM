#include "funcs.h"
#include <iostream>
#include <unistd.h>

string GoToDir(int argc, char* argv[]) {
    string GoTo = argc == 1 || argv[1][0] == '-' ? "." : argv[1];
    return GoTo;
}

    Flags Choice(int argc, char* argv[]) {
    Flags flags;
    bool fl = false;
    int c;
    while ((c = getopt(argc, argv, "ldf")) != -1) {
        if (c == 'l' || c == 'd' || c == 'f') {
            fl = true;
        }
        switch (c) {
            case 'l':
                flags.l = true;
                break;
            case 'd':
                flags.d = true;
                break;
            case 'f':
                flags.f = true;
                break;
            case 's':
                flags.s = true;
                break;
            default:
                throw std::domain_error("Error!");
        }
    }
    if (!fl) {
        flags.l = flags.d = flags.f = true;
    }
    return flags;
}

Types::Types(string new_name)
        : name(move(new_name)) {}

Types::Types(int dt_type, string new_name)
        : name(move(new_name)) {
    switch (dt_type) {
        case DT_DIR:
            type = Types::Type::Dir;
            break;
        case DT_LNK:
            type = Types::Type::Link;
            break;
        case DT_REG:
            type = Types::Type::File;
            break;
        default:
            type = Types::Type::Other;
            break;
    }
}

Type Create(const Flags& flags) {
    return [flags](int a) -> bool {
        switch (a) {
            case DT_DIR:
                return flags.d;
            case DT_LNK:
                return flags.l;
            case DT_REG:
                return flags.f;
            default:
                return true;
        }
    };
}

void GetList(const string& current, List& list, const Type & t) {
    DIR* dir = opendir((current + '/').c_str());

    auto make_path = [current](const dirent* content) {
        return current + '/' + content->d_name;
    };
    auto add_record = [make_path, &list](const dirent* content) {
        list.emplace_back(content->d_type, make_path(content));
    };

    dirent* content;
    while ((content = readdir(dir))) {
        const string name = content->d_name;
        if (name == "." || name == "..") {
            continue;
        }
        if (t(content->d_type)) {
            add_record(content);
        }
        if (content->d_type == DT_DIR) {
            GetList(make_path(content), list, t);
        }
    }
    closedir(dir);
}

void Print(List& list) {
    for (const auto& record : list) {
        switch (record.type) {
            case Types::Type::File:
                cout << record.name << endl;
                break;
            case Types::Type::Dir:
                cout << record.name << endl;
                break;
            case Types::Type::Link:
                cout << record.name << endl;
                break;
            default:
                cout << record.name << endl;
                break;
        }
    }
}
