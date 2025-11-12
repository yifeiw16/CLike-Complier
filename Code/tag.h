#ifndef TAG_H
#define TAG_H

#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
using namespace std;

enum class Tag
{
    //缁堢粨绗?
    epsilon = 0,	//绌?
    the_end,		//#,琛ㄧず缁堟
    id,				//鏍囪瘑绗?
    num,			//鏁板瓧,鍖呮嫭鏁存暟鍜屽皬鏁?
    //绗﹀彿 symbol
    sb_add,			//+
    sb_sub,			//-
    sb_time,		//*
    sb_divide,		///
    sb_assign,		//=
    sb_equal,		//==
    sb_g,			//>
    sb_ge,			//>=
    sb_l,			//<
    sb_le,			//<=
    sb_ne,			//!=
    sb_board,		//鐣岀 ;
    sb_separator,	//鍒嗛殧绗?,
    sb_llb,			//宸﹀皬鎷彿 left little bracket
    sb_rlb,			//鍙冲皬鎷彿
    sb_lbb,			//宸﹀ぇ鎷彿 left big bracket
    sb_rbb,			//鍙冲ぇ鎷彿
    //鍏抽敭瀛?keyword
    kw_int,			//int类型
    kw_void,
    kw_if,			//if语句关键字
    kw_else,
    kw_while,		//while关键字
    kw_return,		//return关键字

    kw_signed,
    kw_char,
    kw_double,
    kw_unsigned,
    kw_const,
    kw_goto,
    kw_for,
    kw_float,
    kw_break,
    kw_class,
    kw_case,
    kw_do,
    kw_long,
    kw_typedef,
    kw_static,
    kw_friend,
    kw_new,
    kw_enum,
    kw_try,
    kw_short,
    kw_continue,
    kw_sizeof,
    kw_switch,
    kw_private,
    kw_catch,
    kw_delete,
    kw_public,
    kw_struct,


    //缁堢粨绗﹀拰闈炵粓缁撶鐨勫垎鐣岀嚎
    //姝ゆ灇涓惧�间粎鐢ㄤ綔鍖哄垎缁堢粨绗﹀拰闈炵粓缁撶
    vtnboundary,


    //闈炵粓缁撶
//    E,
//    T,
//    F,
//    E1,
//    T1,
//    S1,
//    S,
//    BB,
//    B,

S0,				//鎷撳箍鏂囨硶寮�濮嬬鍙?
program,
dec_list,		//declaration_list,澹版槑鍒楄〃
dec,
dec_type,
func_dec,
var_dec,
params,
stat_block,
param_list,
param,
pre_param,
in_dec,
stat_str,
in_var_dec,
pre_in_var_dec,
stat,
if_stat,
while_stat,
return_stat,
assign_stat,
expr,
add_expr,
comp_add_expr,
comp_operator,
term,
pre_term,
factor,
pre_factor,
func_type,
call,
argus,
argu_list,
pre_expr,
};

const std::map<std::string, Tag> STRtoTAG = {
    {"epsilon"			,Tag::epsilon},
    {"the_end"			,Tag::the_end},
    {"id"				,Tag::id},
    {"num"				,Tag::num},
    {"+"				,Tag::sb_add},
    {"-"				,Tag::sb_sub},
    {"*"				,Tag::sb_time},
    {"/"				,Tag::sb_divide},
    {"="				,Tag::sb_assign},
    {"=="				,Tag::sb_equal},
    {">"				,Tag::sb_g},
    {">="				,Tag::sb_ge},
    {"<"				,Tag::sb_l},
    {"<="				,Tag::sb_le},
    {"!="				,Tag::sb_ne},
    {";"				,Tag::sb_board},
    {","				,Tag::sb_separator},
    {"("				,Tag::sb_llb},
    {")"				,Tag::sb_rlb},
    {"{"				,Tag::sb_lbb},
    {"}"				,Tag::sb_rbb},
    {"int"				,Tag::kw_int},
    {"void"				,Tag::kw_void},
    {"if"				,Tag::kw_if},
    {"else"				,Tag::kw_else},
    {"while"			,Tag::kw_while},
    {"return"			,Tag::kw_return},
    {"signed"			,Tag::kw_signed},
    {"char"			    ,Tag::kw_char},
    {"double"			,Tag::kw_double},
    {"unsigned"	     	,Tag::kw_unsigned},
    {"const"	        ,Tag::kw_const},
    {"goto"			    ,Tag::kw_goto},
    {"for"				,Tag::kw_for},
    {"float"			,Tag::kw_float},
    {"break"			,Tag::kw_break},
    {"class"			,Tag::kw_class},
    {"case"			    ,Tag::kw_case},
    {"do"			    ,Tag::kw_do},
    {"long"				,Tag::kw_long},
    {"typedef"			,Tag::kw_typedef},
    {"static"			,Tag::kw_static},
    {"friend"			,Tag::kw_friend},
    {"try"			    ,Tag::kw_try},
    {"short"			,Tag::kw_short},
    {"continue"			,Tag::kw_continue},
    {"sizeof"			,Tag::kw_sizeof},
    {"switch"			,Tag::kw_switch},
    {"private"			,Tag::kw_private},
    {"catch"			,Tag::kw_catch},
    {"delete"			,Tag::kw_delete},
    {"public"			,Tag::kw_public},
    {"struct"			,Tag::kw_struct},
    //    {"i"			    ,Tag::kw_i},
    //    {"a"			    ,Tag::kw_a},
    //    {"b"			    ,Tag::kw_b},

    //    {"E"			    ,Tag::E},
    //    {"T"			    ,Tag::T},
    //    {"F"			    ,Tag::F},
    //    {"E1"			    ,Tag::E1},
    //    {"T1"			    ,Tag::T1},
        //
        //{Tag::S1		    ,"S1"},
        //{Tag::S				,"S"},
        //{Tag::BB		    ,"BB"},
        //{Tag::B				,"B"},

    //    {"S1"			    ,Tag::S1},
    //    {"S"			    ,Tag::S},
    //    {"BB"			    ,Tag::BB},
    //    {"B"			    ,Tag::B},

        {"program"			,Tag::program},
        {"dec_list"			,Tag::dec_list},
        {"dec"				,Tag::dec},
        {"dec_type"			,Tag::dec_type},
        {"func_dec"			,Tag::func_dec},
        {"var_dec"			,Tag::var_dec},
        {"params"			,Tag::params},
        {"stat_block"		,Tag::stat_block},
        {"param_list"		,Tag::param_list},
        {"param"			,Tag::param},
        {"pre_param"		,Tag::pre_param},
        {"in_dec"			,Tag::in_dec},
        {"stat_str"			,Tag::stat_str},
        {"in_var_dec"		,Tag::in_var_dec},
        {"pre_in_var_dec"	,Tag::pre_in_var_dec},
        {"stat"				,Tag::stat},
        {"if_stat"			,Tag::if_stat},
        {"while_stat"		,Tag::while_stat},
        {"return_stat"		,Tag::return_stat},
        {"assign_stat"		,Tag::assign_stat},
        {"expr"				,Tag::expr},
        {"add_expr"			,Tag::add_expr},
        {"comp_add_expr"	,Tag::comp_add_expr},
        {"comp_operator"	,Tag::comp_operator},
        {"term"				,Tag::term},
        {"pre_term"			,Tag::pre_term},
        {"factor"			,Tag::factor},
        {"pre_factor"		,Tag::pre_factor},
        {"func_type"		,Tag::func_type},
        {"call"				,Tag::call},
        {"argus"			,Tag::argus},
        {"argu_list"		,Tag::argu_list},
        {"pre_expr"			,Tag::pre_expr},


};

const std::map<Tag, string> TAG2STR = {
    {Tag::epsilon		,"epsilon"},
    {Tag::the_end		,"the_end"},
    {Tag::id			,"id"},
    {Tag::num			,"num"},
    {Tag::sb_add		,"+"},
    {Tag::sb_sub		,"-"},
    {Tag::sb_time		,"*"},
    {Tag::sb_divide		,"/"},
    {Tag::sb_assign		,"="},
    {Tag::sb_equal		,"=="},
    {Tag::sb_g			,">"},
    {Tag::sb_ge			,">="},
    {Tag::sb_l			,"<"},
    {Tag::sb_le			,"<="},
    {Tag::sb_ne			,"!="},
    {Tag::sb_board		,";"},
    {Tag::sb_separator	,","},
    {Tag::sb_llb		,"("},
    {Tag::sb_rlb		,")"},
    {Tag::sb_lbb		,"{"},
    {Tag::sb_rbb		,"}"},
    {Tag::kw_int		,"int"},
    {Tag::kw_void		,"void"},
    {Tag::kw_if			,"if"},
    {Tag::kw_else		,"else"},
    {Tag::kw_while		,"while"},
    {Tag::kw_return		,"return"},
    {Tag::kw_signed		,"signed"},
    {Tag::kw_char	    ,"char"},
    {Tag::kw_double		,"double"},
    {Tag::kw_unsigned	,"unsigned"},
    {Tag::kw_const	    ,"const"},
    {Tag::kw_goto	    ,"goto"	},
    {Tag::kw_for		,"for"},
    {Tag::kw_float		,"float"},
    {Tag::kw_break		,"break"},
    {Tag::kw_class		,"class"},
    {Tag::kw_case		,"case"	},
    {Tag::kw_do			,"do"},
    {Tag::kw_long	    ,"long"	},
    {Tag::kw_typedef	,"typedef"},
    {Tag::kw_static		,"static"},
    {Tag::kw_friend		,"friend"},
    {Tag::kw_try	    ,"try"},
    {Tag::kw_short		,"short"},
    {Tag::kw_continue	,"continue"},
    {Tag::kw_sizeof		,"sizeof"},
    {Tag::kw_switch		,"switch"},
    {Tag::kw_private	,"private"},
    {Tag::kw_catch		,"catch"},
    {Tag::kw_delete		,"delete"},
    {Tag::kw_public		,"public"},
    {Tag::kw_struct  	,"struct"},
    //    {Tag::kw_i		    ,"i"},
    //    {Tag::kw_a		    ,"a"},
    //    {Tag::kw_b		    ,"b"},

    //    {Tag::E		        ,"E"},
    //    {Tag::T		        ,"T"},
    //    {Tag::F		        ,"F"},
    //    {Tag::E1		    ,"E1"},


    //    {Tag::S1		    ,"S1"},
    //    {Tag::S				,"S"},
    //    {Tag::BB		    ,"BB"},
    //    {Tag::B				,"B"},


        {Tag::S0            ,"S0"},
        {Tag::program		,"program"},
        {Tag::dec_list		,"dec_list"},
        {Tag::dec			,"dec"},
        {Tag::dec_type		,"dec_type"},
        {Tag::func_dec		,"func_dec"},
        {Tag::var_dec		,"var_dec"},
        {Tag::params		,"params"},
        {Tag::stat_block	,"stat_block"},
        {Tag::param_list	,"param_list"},
        {Tag::param			,"param"},
        {Tag::pre_param		,"pre_param"},
        {Tag::in_dec		,"in_dec"},
        {Tag::stat_str		,"stat_str"},
        {Tag::in_var_dec	,"in_var_dec"},
        {Tag::pre_in_var_dec,"pre_in_var_dec"},
        {Tag::stat			,"stat"},
        {Tag::if_stat		,"if_stat"},
        {Tag::while_stat	,"while_stat"},
        {Tag::return_stat	,"return_stat"},
        {Tag::assign_stat	,"assign_stat"},
        {Tag::expr			,"expr"},
        {Tag::add_expr		,"add_expr"},
        {Tag::comp_add_expr	,"comp_add_expr"},
        {Tag::comp_operator	,"comp_operator"},
        {Tag::term			,"term"},
        {Tag::pre_term		,"pre_term"},
        {Tag::factor		,"factor"},
        {Tag::pre_factor	,"pre_factor"},
        {Tag::func_type		,"func_type"},
        {Tag::call			,"call"},
        {Tag::argus			,"argus"},
        {Tag::argu_list		,"argu_list"},
        {Tag::pre_expr		,"pre_expr"},
};

// Function to find the Tag for a given string
Tag findTag(const std::string& str);

std::string findTagName(const Tag& tag);

bool isTerminal(Tag t);

struct Grammar {  //存储单个产生式
    Tag left;
    vector<Tag> right; //产生式右部不止一个，故要用数组
};

extern map<Tag, set<Tag>> firstSets;

extern vector<Grammar> grammars;

struct ProjectClosure {
    int p_grammar;      // 对应产生式对应 vector 下标
    int point;          // 点的位置坐标    S->.E  point=0
    set<Tag> forward_tag;   // 逗号后面跟的，向前望的一个

    bool operator<(const ProjectClosure& other) const {
        // 定义比较函数使得 ProjectClosure 可以放入 set 或 map 中。注意必须使得排序固定
        if (this->p_grammar < other.p_grammar)
            return true;
        else if (this->p_grammar > other.p_grammar)
            return false;
        else if (this->point < other.point)
            return true;
        else if (this->point > other.point)
            return false;
        else if (this->forward_tag.size() < other.forward_tag.size())
            return true;
        else if (this->forward_tag.size() > other.forward_tag.size())
            return false;
        else {
            for (auto it1 = this->forward_tag.cbegin(), it2 = other.forward_tag.cbegin();
                it1 != this->forward_tag.cend();
                ++it1, ++it2) {
                if (*it1 < *it2)
                    return true;
                else if (*it1 > *it2)
                    return false;
            }
            return false;
        }
    }
};

enum class Action		//LR1分析时的动作枚举值
{
    shift_in,	//移进
    reduction,	//归约
    accept,		//接受
    //由于使用map存储动作,故出错状态不再存储
};

enum ScanStatus	//返回值类型
{
    LEXER_ERROR = -1,
    SCAN_TOEND = 0,
    GOON = 1
};

struct Movement		//LR分析表中的一项动作
{
    Action action;
    int go;		//当action为s时,表示转到的状态;当action为r时,表示使用第几个产生式归约

};

extern vector<set<ProjectClosure>> project_set_vector;	//项目集
extern map<int, map<Tag, Movement>> action_go_map;		//表！每个状态 对应一个map数组，

struct TNode	//树结点
{
    Tag ntag;	//tag值
    int p;		//指针
    list<int> childs;	//孩子结点集
};

struct PTree	//语法树
{
    vector<TNode> TNode_List;	//结点集合
    int RootNode = -1;				//根结点指针
};

extern PTree pTree;

extern bool flags_tag;

struct Token	//词法单元,由类型和属性值构成
{
    Tag tag;		//词法单元类型
    string value;
};

#endif // TAG_H
