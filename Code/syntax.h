#ifndef SYNTAX_H
#define SYNTAX_H
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <fstream>
#include <sstream>
#include <list>
#include <string>
#include <queue>
#include <iomanip>

#include "tag.h"
#include "lexer.h"

using namespace std;

//Lexer lexer("1.txt");

enum FileStatus {
    SUCCESS,
    OPEN_ERROR,
    ARROW_ERROR,
    EPSILON_ERROR,
    OVERFLOW_ERROR,
    TAG_ERROR,
    NONE
};

FileStatus readGrammar(const char* file);

void computeFirstSet();

enum class ActionResult {
    Success,
    GrammarError
};

ActionResult ActionGoto();

void print_actiongoto();

void drawProjectFamilyGraph();

void drawProjectFamilyGraphSimplified();

void clear_all();

void print();

int parser(Lexer& lexer,const char* src_path, Token& err_token);

void clear_code();

void actiongoto_tocsv();

#endif

