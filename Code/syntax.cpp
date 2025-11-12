#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <fstream>
#include <sstream>
#include <list>
#include <queue>
#include <iomanip>

#include "syntax.h"
#include <unordered_set>

using namespace std;


//extern Lexer lexer("1.txt");

FileStatus readGrammar(const char* file) {
    ifstream in(file, ios::in);

    if (!in.is_open()) {
        // 文件打开失败
        return OPEN_ERROR;
    }
    grammars.push_back({ Tag::S0,{} });

    string line;
    string temp_left;
    string temp_miss;
    string temp_right;

    while (getline(in, line)) {
        // 输出每一行的内容
        istringstream buffer(line); // buffer是每一行
        // temp是每行中的一个字符串
        Grammar new_add;
        buffer >> temp_left;

        new_add.left = findTag(temp_left);
        if (flags_tag == false)
        {
            return TAG_ERROR;
            flags_tag = true;
        }

        buffer >> temp_miss;  // 略去->
        if (temp_miss != "->") {  // 添加检查
            return ARROW_ERROR;
            //break;
            //throw runtime_error("Invalid syntax. Expected '->'.");
        }

        while (buffer >> temp_right) {
            if (temp_right == "epsilon") {
                return EPSILON_ERROR;
                //throw runtime_error("Invalid syntax. Use an empty space after '->' for an empty production.");
            }
            new_add.right.push_back(findTag(temp_right));
            if (flags_tag == false)
            {
                return TAG_ERROR;
                flags_tag = true;
            }
        }
        grammars.push_back(new_add);
    }
    grammars[0].right.push_back(grammars[1].left);	//构造拓广文法S0->program

    // 关闭文件
    in.close();

    // 如果有其他错误，可以在此处返回 OTHER_ERROR

    // 如果一切正常，返回 SUCCESS
    return SUCCESS;
}

void computeFirstSet()
{
    //终结符的first集就是它本身
    for (int i = int(Tag::epsilon); isTerminal(Tag(i)) == true; i++)
        firstSets[Tag(i)] = { Tag(i) };


    //1. 遍历所有产生式，对所有右部首字符为终结符的产生式，
    // 将右边的第一个终结符放入产生左部的first集中。

    std::vector<int> R1NOTT;

    for (int j = 0; j < grammars.size(); j++) {
        if (grammars[j].right.size() == 0) { //产生式右部为空,添epsilon
            Tag left = grammars[j].left;
            firstSets[left].insert(Tag::epsilon);
        }
        else {
            Tag right_first = grammars[j].right.front();
            if (isTerminal(right_first) == true) {  //右部首字符为终结符,将右边的第一个终结符放入产生左部的first集中
                Tag left = grammars[j].left;
                firstSets[left].insert(right_first);
            }
            else {
                R1NOTT.push_back(j);  //记录哪些式子的右边第一个是非终结符，这些式子需要在下一个while体中进行处理
            }
        }
    }

    //2. 对于每一个右部首字符为非终结符的产生式，对其进行如下操作：
    //2.1  若其首部字符和产生式左部字符不同，则将右部首字符的first集去除空后加入右边的fisrt集
    // 如果含有空，则继续分析下一个字符，直到遇到first集不含空的非终结符或者已经结束了

    bool ifchange;

    while (1) {
        ifchange = 0;
        for (int num : R1NOTT) {  //
            //cout << num << endl;
            Tag left = grammars[num].left;  //左
            bool epsilon_exist;
            int i;
            for (i = 0; i < grammars[num].right.size(); i++) {

                epsilon_exist = 0;
                Tag right_nowfirst = grammars[num].right[i]; //此时要处理的右边字符
                //cout << findTagName(right_nowfirst);
                //cout << isTerminal(right_nowfirst);
                if (isTerminal(right_nowfirst) == 0) {  //如果右边不是终结符
                    if (right_nowfirst == left) {   //如果产生式的右边第一个字符和左边一样，则不需要做2.1这一步,但也要考虑空的情况
                        if (firstSets[right_nowfirst].count(Tag::epsilon))
                            continue;
                        else
                            break;
                    }
                    else { //A->B..  将B的first集减去空集加入A

                        for (Tag Bfirst : firstSets[right_nowfirst]) {
                            //cout << findTagName(Bfirst;
                            if (Bfirst == Tag::epsilon) {
                                epsilon_exist = 1;
                                continue;
                            }
                            else {
                                int before_size = firstSets[left].size();
                                firstSets[left].insert(Bfirst);
                                int after_size = firstSets[left].size();

                                if (before_size < after_size) {
                                    ifchange = 1;
                                }
                            }
                        }
                        if (epsilon_exist == 0) {
                            break;
                        }
                    }
                }
                else {
                    int before_size = firstSets[left].size();
                    firstSets[left].insert(right_nowfirst);//直到读到终结符，加入first集后，就结束
                    int after_size = firstSets[left].size();
                    if (before_size < after_size) {
                        ifchange = 1;
                    }
                    break;
                }

            }
            if ((i == grammars[num].right.size() - 1) && epsilon_exist == 1) {//如果产生式最后一个符号也含空,则将空加入First集
                int before_size = firstSets[left].size();
                firstSets[left].insert(Tag::epsilon);

                int after_size = firstSets[left].size();

                if (before_size < after_size) {
                    ifchange = 1;
                }
            }
        }
        if (ifchange == 0) {
            break;
        }
    }
}

set<ProjectClosure> Closure(set<ProjectClosure>& oldp)
{
    set<ProjectClosure> old(oldp);
    set<ProjectClosure> newcompute(oldp);  //初始化，自身所有项目都在里面
    set<ProjectClosure> newp;

    //set<ProjectClosure> newcompute(oldp);


    bool ifchange;
    while (1) {
        ifchange = 0;
        for (auto it = old.begin(); it != old.end(); it++) {
            const ProjectClosure& i = *it;
            //A->α.Bβ,a 形如这样的要处理，要展开B point=1,size=3
            ;
            if (grammars[i.p_grammar].right.size() > i.point && isTerminal(grammars[i.p_grammar].right[i.point]) == 0) { //判断点后面还有，且紧跟的是非终结符
                //这时候需要求出first(βa)
                Tag after_point = grammars[i.p_grammar].right[i.point];
                Tag look_forward;
                set<Tag> first_lookforward;
                if (i.point < grammars[i.p_grammar].right.size() - 1) {
                    //A->α.Bβ 这种类型的
                    look_forward = grammars[i.p_grammar].right[i.point + 1];
                    first_lookforward = firstSets[look_forward];
                    auto p = first_lookforward.find(Tag::epsilon);
                    if (p != first_lookforward.cend()) {
                        //如果含有epsilon,则删除epsilon并把原项目的follows加入
                        first_lookforward.erase(p);
                        for (const auto& follow : i.forward_tag)
                            first_lookforward.insert(follow);
                    }
                }
                else {
                    for (const auto& follow : i.forward_tag)
                        first_lookforward.insert(follow);
                }


                //将B推出的加入，在原来所有的产生式中寻找
                for (int i = 0; i < grammars.size(); i++) {
                    if (grammars[i].left == after_point) {
                        //找到了B->...这个产生式
                        //看看原来的闭包里有没有B->.γ，first_lookforward
                        bool already_exist = 0;
                        for (auto iit = old.begin(); iit != old.end(); ++iit) {
                            if (iit->p_grammar == i && iit->point == 0) {
                                //已经大致在集合里了，下面要看first_lookforward一不一样


                                if (iit->forward_tag != first_lookforward) {

                                    already_exist = false;
                                    newcompute.erase(iit);

                                }
                                else {
                                    already_exist = true;
                                }
                                break;
                            }
                        }
                        if (already_exist == false) {
                            ifchange = 1;
                            newcompute.insert({ i,0,first_lookforward });
                            newp.insert({ i,0,first_lookforward });

                        }
                    }
                }
            }
        }
        old = newp;
        newp.clear(); //这一次已经把已有的处理完了，下面只需要处理新增的即可

        if (ifchange == 0)
            break;

    }
    return newcompute;

}

ActionResult ActionGoto() {

    //先初始化第一个I0
    project_set_vector.clear();
    set<ProjectClosure> start;
    start.insert({ 0, 0, { Tag::the_end } });

    set<ProjectClosure> I00 = Closure(start);

    project_set_vector.emplace_back(I00); //对于这个I00还得加

    for (int index = 0; index < project_set_vector.size(); index++) {
        //对于I0来说
        //cout << index;
        set<ProjectClosure>& current = project_set_vector[index];  //这个是当前项目族的文法式集合
        map<Tag, set<ProjectClosure>> create_new_clousre;

        for (auto& expression : current) {   //对于I0中的式子来说
            Grammar g = grammars[expression.p_grammar];
            if (expression.point < g.right.size()) {
                //不是规约项目
                //直接把forward_tag抄下来，point往右边移一个，转移的就是point前的tag
                create_new_clousre[g.right[expression.point]].insert({ expression.p_grammar,expression.point + 1,expression.forward_tag });
            }
            else {
                //是规约项目
                if (expression.p_grammar == 0 && expression.point == 1 && expression.forward_tag.size() == 1 && expression.forward_tag.count(Tag::the_end) == 1)
                    action_go_map[index][Tag::the_end] = { Action::accept,expression.p_grammar };	//可接受状态,规约式子的下标
                else {
                    for (Tag forward_tag : expression.forward_tag) {
                        if (action_go_map[index].count(forward_tag) != 0) {
                            return ActionResult::GrammarError;
                            //............报错不是LR1文法
                            //throw exception("LR1 Grammar expected!");
                        }
                        else {
                            action_go_map[index][forward_tag] = { Action::reduction,expression.p_grammar };
                        }
                    }
                }
            }
        }

        for (auto& k : create_new_clousre) {
            set<ProjectClosure> newps = Closure(k.second);	//生成新closure集
            cout << "done";
            int same_index = -1;
            for (int i = 0; i < project_set_vector.size(); i++) {  //看看有没有重复
                if (project_set_vector[i].size() != newps.size())
                    continue;
                else {
                    bool ifsame = true;
                    for (auto it1 = newps.cbegin(), it2 = project_set_vector[i].cbegin(); it1 != newps.cend(); it1++, it2++) {
                        if (it1->p_grammar != it2->p_grammar || it1->point != it2->point || it1->forward_tag != it2->forward_tag) {
                            ifsame = false;
                            break;
                        }
                    }
                    if (ifsame == 1) {
                        same_index = i;
                        break;
                    }
                }
            }


            if (same_index == -1) {   //没有相同的
                project_set_vector.emplace_back(newps);
                action_go_map[index][k.first] = { Action::shift_in, int(project_set_vector.size()) - 1 };	//移进
            }
            else {
                action_go_map[index][k.first] = { Action::shift_in, same_index };	//移进
            }
        }

    }
    return ActionResult::Success;


}

void print_actiongoto() {
    // Write LR(1) items to a text file
    ofstream lr1ItemsFile("LR1ItemsAndTable.txt");
    lr1ItemsFile << std::left;  // Set left alignment for the table

    // Write LR(1) items
    for (int i = 0; i < project_set_vector.size(); ++i) {
        lr1ItemsFile << "I" << i << ":\n";
        for (const auto& closure : project_set_vector[i]) {
            lr1ItemsFile << std::setw(8) << " "; // Padding for the numbering
            lr1ItemsFile << findTagName(grammars[closure.p_grammar].left) << " ->";
            const auto& right = grammars[closure.p_grammar].right;
            for (int j = 0; j < right.size(); ++j) {
                if (j == closure.point) {
                    lr1ItemsFile << " .";
                }
                lr1ItemsFile << " " << findTagName(right[j]);
            }
            if (closure.point == right.size()) {
                lr1ItemsFile << " .";
            }
            lr1ItemsFile << ", {";
            for (const auto& tag : closure.forward_tag) {
                lr1ItemsFile << findTagName(tag) << " ";
            }
            lr1ItemsFile << "}\n";
        }
        lr1ItemsFile << "\n";
    }

    lr1ItemsFile.close();

    // Write Action/Goto table to another text file
    set<Tag> all_tags;

    for (const auto& state_entry : action_go_map) {
        for (const auto& tag : state_entry.second) {
            all_tags.insert(tag.first);
        }
    }

    // 打印表头
    ofstream actionGotoFile("ActionGotoTable.txt");
    actionGotoFile << std::setw(15) << "State";
    for (const auto& tag : all_tags) {
        actionGotoFile << std::setw(15) << findTagName(tag);
    }
    actionGotoFile << "\n";

    // 打印表格内容
    for (const auto& state_entry : action_go_map) {
        int state = state_entry.first;
        const auto& transitions = state_entry.second;

        actionGotoFile << std::setw(15) << state;

        for (const auto& tag : all_tags) {
            auto it = transitions.find(tag);
            if (it != transitions.end()) {
                const Movement& movement = it->second;

                switch (movement.action) {
                    case Action::shift_in:
                        actionGotoFile << std::setw(15) << "S" + std::to_string(movement.go);
                        break;
                    case Action::reduction:
                        actionGotoFile << std::setw(15) << "R" + std::to_string(movement.go);
                        break;
                    case Action::accept:
                        actionGotoFile << std::setw(15) << "Acc";
                        break;
                    default:
                        actionGotoFile << std::setw(15) << "Error";
                }
            }
            else {
                actionGotoFile << std::setw(15) << ""; // 填充空格
            }
        }

        actionGotoFile << "\n";
    }

    actionGotoFile.close();
}

void actiongoto_tocsv() {
    // 获取所有可能的转移字符
    set<Tag> all_tags;

    for (const auto& state_entry : action_go_map) {
        for (const auto& tag : state_entry.second) {
            all_tags.insert(tag.first);
        }
    }

    // 打印表头
    ofstream actionGotoFile("ActionGotoTable.csv", ios::trunc);  // 使用 ios::trunc 标志覆盖已存在的文件
    actionGotoFile << "State";
    for (const auto& tag : all_tags) {
        actionGotoFile << "," << findTagName(tag);
    }
    actionGotoFile << "\n";

    // 打印表格内容
    for (const auto& state_entry : action_go_map) {
        int state = state_entry.first;
        const auto& transitions = state_entry.second;

        actionGotoFile << state;

        for (const auto& tag : all_tags) {
            auto it = transitions.find(tag);
            if (it != transitions.end()) {
                const Movement& movement = it->second;

                switch (movement.action) {
                    case Action::shift_in:
                        actionGotoFile << "," << "S" + std::to_string(movement.go);
                        break;
                    case Action::reduction:
                        actionGotoFile << "," << "R" + std::to_string(movement.go);
                        break;
                    case Action::accept:
                        actionGotoFile << "," << "Acc";
                        break;
                    default:
                        actionGotoFile << "," << "Error";
                }
            }
            else {
                actionGotoFile << ",";
            }
        }

        actionGotoFile << "\n";
    }

    actionGotoFile.close();
}

void generateGraph(const string& dotCode, const string& outputFileName) {
    ofstream dotFile("temp.dot");
    dotFile << dotCode;
    dotFile.close();

    // Run Graphviz to generate the graph
    string command = "dot -Tpng temp.dot -o " + outputFileName;
    system(command.c_str());
}

void drawProjectFamilyGraphSimplified() {
    // Generate DOT code for the project family graph
    stringstream dotCode;
    dotCode << "digraph ProjectFamily {\n";
    dotCode << "  node [shape=circle, style=filled, color=\"#d3e0ea\", fontname=\"Arial\", fontsize=10];\n";
    dotCode << "  edge [fontname=\"Arial\", fontsize=10, color=\"#404040\"];\n";

    // Add nodes
    for (int i = 0; i < project_set_vector.size(); ++i) {
        dotCode << "  " << i << " [label=\"I" << i << "\"];\n";
    }

    // Add edges
    for (int i = 0; i < project_set_vector.size(); ++i) {
        const auto& transitions = action_go_map[i];
        for (const auto& entry : transitions) {
            Tag tag = entry.first;
            Movement movement = entry.second;

            dotCode << "  " << i << " -> " << movement.go << " [label=\"" << findTagName(tag) << "\", color=\"#404040\"];\n";
        }
    }

    dotCode << "}\n";

    // Generate the graph
    generateGraph(dotCode.str(), "ProjectFamilyGraphSimplified.png");
}

void drawProjectFamilyGraph() {
    // Generate DOT code for the project family graph
    stringstream dotCode;
    dotCode << "digraph ProjectFamily {\n";
    dotCode << "  rankdir=LR;\n";
    dotCode << "  node [shape=box, style=rounded, fontname=\"Arial\", fontsize=10, color=\"#ffffff\"];\n";
    dotCode << "  edge [fontname=\"Arial\", fontsize=10];\n";

    // Add nodes
    for (int i = 0; i < project_set_vector.size(); ++i) {
        dotCode << "  " << i << " [label=\"";

        // Print LR(1) items
        for (const auto& closure : project_set_vector[i]) {
            dotCode << findTagName(grammars[closure.p_grammar].left) << " ->";
            const auto& right = grammars[closure.p_grammar].right;
            for (int j = 0; j <= right.size(); ++j) {
                if (j == closure.point) {
                    dotCode << " .";
                }
                if (j < right.size()) {
                    dotCode << " " << findTagName(right[j]);
                }
            }
            dotCode << ", {";
            for (const auto& tag : closure.forward_tag) {
                dotCode << findTagName(tag) << " ";
            }
            dotCode << "}\n";
        }

        dotCode << "\", style=filled, fillcolor=\"#d3e0ea\", color=\"#2e7db2\"];\n"; // Light blue
    }

    // Add edges
    for (int i = 0; i < project_set_vector.size(); ++i) {
        const auto& transitions = action_go_map[i];
        for (const auto& entry : transitions) {
            Tag tag = entry.first;
            Movement movement = entry.second;

            dotCode << "  " << i << " -> " << movement.go << " [label=\"" << findTagName(tag) << "\", color=\"#404040\"];\n"; // Dark gray
        }
    }

    dotCode << "}\n";

    // Generate the graph
    generateGraph(dotCode.str(), "ProjectFamilyGraph.png");
}

void clear_all()
{
    action_go_map.clear();
    project_set_vector.clear();
    grammars.clear();
    firstSets.clear();

}

//int parser(const char* src_path, Token& err_token)
//{
//    //if (!this->lexer.openFile(src_path))
//    //	return State::ERROR;
//
//    stack<int> state_Stack;	//状态栈
//    stack<Tag> in_Stack;	//输入栈
//    stack<int> node_Stack;  //树结点栈，存放树节点下标
//
//    state_Stack.push(0);		//初始化
//    in_Stack.push(Tag::the_end);	//初始化
//    //NStack.push(-1);			//初始化
//
//    bool islast_Token = false;	//判断是否使用上次的token
//    Token temp_token;	//当前token
//    int temp_state;		//当前state
//    Movement temp_move;	//当前动作
//    while (true) {
//        cout << 2 << endl;
//        //需要新获取一个token
//        if (!islast_Token) {
//            if (lexer.getNextLexical(temp_token) == -1)
//            {
//                return -1;
//            }
//
//        }
//        temp_state = state_Stack.top();
//        cout << temp_token.value << "*" << findTagName(temp_token.tag) << endl;
//        if (action_go_map.count(temp_state) == 0 || action_go_map[temp_state].count(temp_token.tag) == 0) {
//            //若对应表格项为空,则出错
//            cout << action_go_map[temp_state].count(temp_token.tag) << "*" << endl;
//            cout << action_go_map.count(temp_state) << "*" << endl;
//            err_token = temp_token;
//
//            return -1;
//        }
//        temp_move = action_go_map[temp_state][temp_token.tag];	//获取当前动作
//        //归约操作
//        if (temp_move.action == Action::reduction) {
//            int len = grammars[temp_move.go].right.size();	//产生式右部长度
//
//            TNode node_left;								//产生式左部
//            node_left.ntag = grammars[temp_move.go].left;	//产生式左部tag
//            node_left.p = pTree.TNode_List.size();			//移进树结点
//
//            //移出栈
//            while (len-- > 0) {
//                state_Stack.pop();
//                in_Stack.pop();
//
//                node_left.childs.push_front(node_Stack.top());	//创建子结点链表
//                node_Stack.pop();
//            }
//
//            pTree.TNode_List.push_back(node_left);			//移进树栈
//
//            temp_state = state_Stack.top();	//更新当前状态
//            if (action_go_map.count(temp_state) == 0 ||
//                action_go_map[temp_state].count(node_left.ntag) == 0) {
//                //若对应表格项为空,则出错
//                err_token = temp_token;
//
//                cout << 5 << endl;
//                return -1;
//            }
//
//            temp_move = action_go_map[temp_state][node_left.ntag];	//更新当前动作
//            //入栈操作
//            islast_Token = true;
//            state_Stack.push(temp_move.go);
//            in_Stack.push(node_left.ntag);
//            node_Stack.push(node_left.p);
//
//        }
//        //移进操作
//        else if (temp_move.action == Action::shift_in) {
//            state_Stack.push(temp_move.go);
//            in_Stack.push(temp_token.tag);
//
//            TNode in_node;	//移进的树结点
//            in_node.ntag = temp_token.tag;	//初始化tag值
//            in_node.p = pTree.TNode_List.size();	//指定树节点在TNode_List中的下标
//            pTree.TNode_List.push_back(in_node);	//移进树结点
//            node_Stack.push(in_node.p);
//
//            islast_Token = false;
//        }
//        else //接受
//        {
//            pTree.RootNode = pTree.TNode_List.size() - 1;	//根结点即为最后一个移进树结点集的结点
//            return 1;		//accept
//        }
//    }
//}

void print()
{
    string Treecode = "";
    if (pTree.RootNode == -1)	//没有根节点，树都不存在，没得画咯
    {
        cout << "error no tree" << endl;
        return;
    }
    queue<int> myqueue;
    Treecode += "digraph parser_tree{\n";
    Treecode += "rankdir=TB;\n";
    \
        //初始化结点
        for (int i = 0; i < pTree.TNode_List.size(); i++)
        {
            Treecode += "node_" + to_string(i) + "[label=\"" + TAG2STR.at(pTree.TNode_List[i].ntag) + "\" ";
            Treecode += "shape=\"";

            if (isTerminal(pTree.TNode_List[i].ntag)) //终结符，蓝色字体，无圆框
                Treecode += "none\" fontcolor=\"blue\"];\n";

            else                               //非终结符，黑色字体，有圆框
                Treecode += "box\" fontcolor=\"black\"];\n";

        }
    Treecode += "\n";

    myqueue.push(pTree.RootNode);	//根节点入队列，即将开始BFS输出语法树
    while (!myqueue.empty())
    {
        TNode node = pTree.TNode_List[myqueue.front()];	//取第一个结点，对其进行画树
        myqueue.pop();

        if (node.childs.size() == 0)	//若无子结点，不用画他的子树
            continue;
        //若有子结点，则画其子树
        for (auto it = node.childs.cbegin(); it != node.childs.cend(); it++)	//声明连接关系
        {
            Treecode += "node_" + to_string(node.p) + "->node_" + to_string(*it) + ";\n";

            myqueue.push(*it);
        }
    }
    Treecode += "}\n";

    generateGraph(Treecode, "Graph_Tree.png");
}

void clear_code()
{
    pTree.TNode_List.clear();
    pTree.RootNode = -1;
}
//void all_step(string filename)
//{


//}
