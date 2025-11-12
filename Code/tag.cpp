#include "tag.h"

map<Tag, set<Tag>> firstSets;

vector<Grammar> grammars;

vector<set<ProjectClosure>> project_set_vector;	//项目集

map<int, map<Tag, Movement>> action_go_map;		//表！每个状态 对应一个map数组，

bool flags_tag = true;

PTree pTree;

Tag findTag(const std::string& str) {
    auto it = STRtoTAG.find(str);
    if (it != STRtoTAG.end()) {
        return it->second;
    }
    else {
        // 鎶涘嚭寮傚父锛岃〃绀烘壘涓嶅埌瀵瑰簲鐨?Tag
        flags_tag = false;
        return STRtoTAG.begin()->second;
    }
}

std::string findTagName(const Tag& tag) {
    auto it = TAG2STR.find(tag);
    if (it != TAG2STR.end()) {
        return it->second;
    }
    else {
        return "Tag not found";
    }
}

bool isTerminal(Tag t)
{
    return t < Tag::vtnboundary;
}
