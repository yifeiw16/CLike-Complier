#include "lexer.h"
#include <iomanip>
const char* KeyWord[keywordCount] = {
     "int","void","if","else","while","return",
     "signed","char","double","unsigned","const",
     "goto","for","float","break","class","case",
     "do","long","typedef","static","friend",
     "new","enum","try","short","continue","sizeof",
     "switch","private","catch","delete","public","struct"
};//关键字
const char* Opertaor[] = {
    "<=", ">=", ":=", "<>", "==", "!=",
    "+", "-","/","=","<",">",
};//运算符

const char* Delim[] = {
    "(", ")", ",", ";", ".", "[",
    "]", ":", "{", "}", "\"",
};//界符

//返回是否是空白制表符
bool isWhitespace(char ch) {

    return (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r');
}

//判断是否是字母
bool isAlphabet(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}


//判断是否是数字
bool isDigit(char c) {
    return (c >= '0' && c <= '9');
}

//十进制数字转成二进制字符串
string Binary(string t)
{
    int x = std::stoi(t);
    string s = "";
    while (x) {
        if (x % 2 == 0) s = '0' + s;
        else s = '1' + s;
        x /= 2;
    }
    return s;
}


int Lexer::getNextLexical(Token& next_token)
{
    if (token_index == fileword.size())
    {
        next_token.tag = Tag::the_end;
        next_token.value = "null";
        Token e = next_token;
        all_token.push_back(e);
        return 0;
    }
    else if (token_index > fileword.size())
        return ERROR;

    next_token.value = fileword[token_index].word;
    if (fileword[token_index].type == 34)
    {
        next_token.tag = Tag::id;
    }
    else if (fileword[token_index].type >= 0 && fileword[token_index].type <= 33)
        next_token.tag = findTag(fileword[token_index].word);
    else if (fileword[token_index].type == 35 || fileword[token_index].type == 36)
    {
        next_token.tag = Tag::num;
    }

    else
        next_token.tag = findTag(fileword[token_index].word);
    Token e = next_token;
    all_token.push_back(e);
    token_index++;
    return 0;

}

//预处理
void Lexer::Pretreat()
{
    std::string line;
    ofstream outfile;
    outfile.open("pretreat-code.txt");
    bool addin_1 = false;                    //addin表示这一字符是不是在注释//里 true表示在 FALSE表示不在
    bool addin_2 = false;
    while (file.GetNextLine(line))        //读取一整行
    {
        addin_1 = false;
        for (int i = 0; i < line.size(); i++) {                    //判断是否需要删除注释部分
            if (line[i] != '/' && line[i] != '*')
            {
                if (addin_1 == false && addin_2 == false)          //如果不为/且不为*，且不在注释部分，则输出
                    outfile << line[i];
            }
            else if (line[i] == '/')                               //如果是/，则判断 后一个字符是否为/或*，是则addin取反，不是则择情况输出
            {
                if (line[i + 1] == '/')
                {
                    addin_1 = !addin_1;                            //addin取反
                    i++;                                           //跳过下一个字符
                }
                else if (line[i + 1] == '*')
                {
                    addin_2 = !addin_2;                            //addin取反
                    i++;                                           //跳过下一个字符
                }
                else if (addin_1 == false && addin_2 == false)
                    outfile << line[i];
            }
            else
            {
                if (line[i + 1] == '/')                                //如果是* 则判断后一个字符是否为/，是则addin取反
                {
                    addin_2 = !addin_2;                               //addin取反
                    i++;
                }
                else if (addin_1 == false && addin_2 == false)
                    outfile << line[i];
            }
        }
        if (addin_1 == false && addin_2 == false)
            outfile << endl;
    }
    outfile.close();
    //指针回到文件开头
    file.Restart();
}

//Token& next_token
ScanStatus Lexer::Scan() {

    int ret = 1;

    if (file.Getnext(true) == EndofFile) {
        //cout << "at end" << endl;
        //return false;
        return SCAN_TOEND;
    }
    char current_process = file.next_char;
    //cout << current_process;

    if (isAlphabet(current_process) == true) {   //如果当前处理的是字母
        string buffer;
        buffer += current_process;

        //继续往后预读一位，如果是字母或者数字就拼接
        while ((ret = file.Getnext(false)) != EndofFile) {
            char next = file.next_char;
            if (isAlphabet(next) || isDigit(next) || next == '_') {  //下一位是字母或者数字
                buffer += next;
            }
            else {
                //回退一位
                //cout << next;
                file.Retract();
                //查保留字表
                //cout << buffer << ' ';

                //判断是标识符还是关键字
                WordType e;
                e.word = buffer;
                bool isKeyWord = false;

                for (int i = 0; i < keywordCount; ++i)
                {
                    if (e.word == KeyWord[i])
                    {
                        e.type = i;
                        e.index = Index;
                        isKeyWord = true;
                        break;
                    }
                }
                if (!isKeyWord)
                {
                    e.type = keywordCount;
                    e.index = to_string(IdentifierNum++);
                }
                fileword.push_back(e);
                break;

            }
        }
    }
    else if (isDigit(current_process) == true || current_process == '.') {   //如果当前处理的是数字

        //1.23E10

        //regex reg(R"((\d+\.\d*|\.\d+|\d+\.\d+|\d+)((e|E)\d+)?)");    //这里E之后可以有正负,这里需要完善
        regex reg(R"((\d+\.\d*|\.\d+|\d+\.\d+|\d+)([eE][+-]?\d+)?)");

        string buffer;
        string TmpBuffer;//用于存放在特殊字符前的数字字符串
        buffer += current_process;
        TmpBuffer += current_process;
        WordType e;
        bool is_minus = false;                                          //判断E后面跟的是正数还是负数

        e.type = current_process == '.' ? DecimalType : IntegerType;

        //往后预读一位，如果是数字或者.或者e或者E，就拼接
        //在转变为二进制时，如果存在以下字符，就将字符前的数字转为二进制
        while ((ret = file.Getnext(false)) != EndofFile) {
            if (isDigit(file.next_char)) {
                buffer += file.next_char;
                TmpBuffer += file.next_char;
            }
            else if (file.next_char == 'E' || file.next_char == 'e' || file.next_char == '.')
            {
                e.type = file.next_char == '.' ? DecimalType : e.type;
                if (TmpBuffer != "")
                {
                    e.index += Binary(TmpBuffer);
                    e.index += file.next_char;
                }
                buffer += file.next_char;
                TmpBuffer = "";
            }
            else if (file.next_char == '+')                            //如果E后面跟的是正数
            {
                buffer += file.next_char;
            }
            else if (file.next_char == '-')                            //如果E后面跟的是负数
            {
                buffer += file.next_char;
                is_minus = true;
            }
            else {
                file.Retract();
                if (!regex_match(buffer, reg)) {
                    e.index = "error!";
                    return LEXER_ERROR;

                }
                break;
            }
        }
        e.word = buffer;
        if (TmpBuffer != "" && e.index != "error!")
            if (is_minus)
            {
                //e.index += BinaryTwosComplement(TmpBuffer);
                is_minus = false;
            }
            else
                e.index += Binary(TmpBuffer);
        fileword.push_back(e);
    }
    else if (current_process == '(' || current_process == ')' || current_process == '{' || current_process == '}' || current_process == '+' ||
        current_process == '-' || current_process == '*' || current_process == ',' || current_process == ';')
    {
        WordType e;
        e.word = string(1, current_process);
        //cout << e.word << ' ';
        e.index = Index;
        for (int i = 0; i < DelimCount; ++i)
        {
            if (Delim[i] == e.word)
            {
                e.type = i + DelimBegin;
                break;
            }
        }
        fileword.push_back(e);
    }

    else if (current_process == '=' || current_process == '>' || current_process == '<' || current_process == '!' || current_process == '/')
    {
        WordType e;
        e.index = Index;
        e.word = "";
        if (current_process == '=') {  //分为=和==
            while ((ret = file.Getnext(false)) != EndofFile) {
                if (file.next_char == '=') {
                    e.word = "==";
                    break;
                }
                else {
                    file.Retract();
                    e.word = "=";
                    break;
                }
            }
        }
        else if (current_process == '>') { //分为>和>=
            while ((ret = file.Getnext(false)) != EndofFile) {
                if (file.next_char == '=') {
                    e.word = ">=";
                    break;
                }
                else {
                    file.Retract();
                    e.word = ">";
                    break;
                }
            }
        }
        else if (current_process == '<') { //分为<和<=
            while ((ret = file.Getnext(false)) != EndofFile) {
                if (file.next_char == '=') {
                    e.word = "<=";
                    break;
                }
                else {
                    file.Retract();
                    e.word = "<";
                    break;
                }
            }
        }
        else if (current_process == '!') {
            while ((ret = file.Getnext(false)) != EndofFile) {
                if (file.next_char == '=') {
                    e.word = "!=";
                    break;
                }
                else {
                    file.Retract();
                    e.word = "!";
                    //cout << "error!";
                    break;
                }
            }
        }
        //在这里将所有注释去掉
        else if (current_process == '/') {  // 可以是双斜杠// 可以是除号/  可以是/**/
            while ((ret = file.Getnext(false)) != EndofFile) {
                if (file.next_char == '/') {
                    //cout << "//";  //一直读直到换行
                    while ((ret = file.Getnext(false)) != EndofFile) {
                        if (file.next_char == '\n')
                            break;
                    }
                    break;
                }
                else if (file.next_char == '*') {  //这个需要一直往后读，不管换行，也要找到与之匹配的*/
                    while ((ret = file.Getnext(false)) != EndofFile) {
                        if (file.next_char == '*') {   //再往后读一个，看看是不是/
                            if (file.Getnext(false) != EndofFile && file.next_char == '/') {
                                //配对成功
                                //cout << "/**/";
                                break;
                            }
                            else if (file.next_char == EOF) {
                                //return LEXER_ERROR;
                                //cout << "error!";
                                break;
                            }
                            else {
                                continue;
                            }
                        }
                    }
                    break;
                }
                else {  //不是注释
                    file.Retract();
                    e.word = "/";
                    break;
                }
            }
        }
        bool isOpertaor = false;
        for (int i = 0; i < OpertaorCount; ++i)
        {
            if (Opertaor[i] == e.word)
            {
                e.type = i + OpertaorBegin;
                isOpertaor = true;
                break;
            }
        }
        e.index = isOpertaor ? e.index : "error!";
        //if (isOpertaor == false) {
        //    return LEXER_ERROR;
        //}
        //对于删除掉的注释，不加入字符集
        if (e.word != "")
            fileword.push_back(e);
    }
    else {
        //cout << "error";
        return LEXER_ERROR;
    }

    if (ret == EndofFile)
        return SCAN_TOEND;
    else
        return GOON;

}

//输出最后的字符和字符集
void Lexer::PrintLexer()
{
    for (unsigned int i = 0; i < fileword.size(); ++i)
    {
        cout << fileword[i].word << "," << fileword[i].type << endl;
    }
}

//void Lexer::printtoken()
//{
//    ofstream outfile;
//    outfile.open("token-tag.txt");
//    string s = "";
//    s = s + "[token]-[tag]\n";
//    for (int i = 0; i < all_token.size(); ++i)
//    {
//        s = s + "[" + all_token[i].value + "]-[" + findTagName(all_token[i].tag) + "]\n";
//    }
//    outfile << s;
//    outfile.close();
//}



void Lexer::printtoken() {
    ofstream outfile;
    outfile.open("token-tag.txt");
    outfile << "[token]   --     [tag]\n";
    outfile << "——————————————\n";
    for (int i = 0; i < all_token.size(); ++i) {
        string tokenValue = "[" + all_token[i].value + "]";
        string tagName = "[" + findTagName(all_token[i].tag) + "]";

        outfile << setw(10) << left << tokenValue << "--     " << tagName << "\n";
    }

    outfile.close();
}
