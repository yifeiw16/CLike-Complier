#ifndef LEXER_H
#define LEXER_H

#pragma once
#include<iostream>
#include <string>
#include <fstream>
#include <regex>
#include <bitset>
#include <sstream>

#include <map>
#include "tag.h"
using namespace std;

#define EndofFile -1
#define ERROR -1
#define keywordCount 34
#define OpertaorCount 12
#define DelimCount 11
#define DelimBegin 50
#define OpertaorBegin 37
#define Index "-"
#define IntegerType 35
#define DecimalType 36

extern const char* KeyWord[];
extern const char* Opertaor[];
extern const char* Delim[];

/*
//'-','0','1'分别表示关键字、运算符、界符
//0——33：关键字："int", "void", "if", "else", "while", "return"
//34：标识符
//35：整数
//36：小数
//37——42："<=", ">=", ":=", "<>", "==", "!="
//43——49：'+', '-', '*', '/', '=', '<', '>'
//50——60：'(', ')', ',', ';', '.', '[', ']', ':', '{', '}', '"'
//*/

//返回是否是空白制表符
bool isWhitespace(char ch);

//判断是否是字母
bool isAlphabet(char c);


//判断是否是数字
bool isDigit(char c);

//十进制数字转成二进制字符串
string Binary(string t);


class Lexer;


class File {
public:

    friend Lexer;

    File(const std::string& filename) {
        // 打开文件
        file.open(filename);

        cout << filename << endl;
        // 检查文件是否成功打开
        if (!file.is_open()) {
            std::cerr << "无法打开文件: " << filename << std::endl;
        }
    }

    // 写入信息到文件
    void Write(const std::string& message) {
        if (file.is_open()) {
            file << message;
        }
        //else {
        //    std::cerr << "文件未打开，无法写入信息。" << std::endl;
        //}
    }

    // 关闭文件
    void Close() {
        if (file.is_open()) {
            file.close();
        }
    }

    //回退一格
    void Retract() {
        file.seekg(-1, std::ios::cur);
    }

    //回到文件开头
    void Restart() {
        file.clear(); // 清除可能的错误标志
        file.seekg(0, std::ios::beg);
    }

    //读下一个字符
    int Getnext(bool ignore_blank) {
        next_char = file.get();
        if (next_char == EOF) {
            return EndofFile;
        }
        if (ignore_blank == true) {  //需要跳过空白制表符，当一个token读完以后，需要这么做

            while (isWhitespace(next_char)) {
                next_char = file.get();
            }
            if (next_char == EOF) {
                return EndofFile;
            }
        }
        return true;
    }

    // 读取下一行
    bool GetNextLine(std::string& line) {
        if (file.is_open()) {
            if (std::getline(file, line)) {
                return true;  // 成功读取一行
            }
        }
        return false;  // 读取失败或文件未打开
    }

    ~File() {
        // 确保在对象销毁时关闭文件
        Close();
    }

    // 检查文件是否成功打开
    bool is_open() const {
        return file.is_open();
    }

private:
    std::fstream file;
    char next_char = NULL;
    //char pre_read = NULL;

};

struct WordType {
    string word;
    int type = 0;
    string index;
};



class Lexer {
public:
    Lexer(const std::string& filename) : file(filename) {}
    int getNextLexical(Token& next_token);
    ScanStatus Scan();
    void Pretreat();
    void PrintLexer();
    void printtoken();

private:
    File file;
    int IdentifierNum = 0;
    int token_index = 0;
    vector<WordType> fileword;//存放识别的字符
    vector<Token> all_token;//存放识别的字符
};

#endif // LEXER_H












































