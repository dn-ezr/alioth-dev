#ifndef __morpheme__
#define __morpheme__

#include "node.hpp"
#include "eproto.hpp"
#include "definition.hpp"
#include "insblockimpl.hpp"
#include "constructimpl.hpp"
#include "expressionimpl.hpp"
#include <map>

/**
 * @namespace morpheme : 语素
 * @desc :
 *  语素命名空间用于容纳一系列用于构成其他语法结构却不能单独存在的小型语法结构
 */
namespace alioth::morpheme {

/**
 * @struct predi : predicate item 谓词单元
 * @desc :
 *  谓词单元与模板参数关联,描述关于模板参数选取的限制性规则
 */
struct predi {
    /**
     * @member rule : 限制规则
     * @desc :
     *  0 : 无效
     *  1 : 元素类型不能为变量                      T != obj
     *  2 : 元素类型不能为指针                      T != ptr
     *  3 : 元素类型不能为引用                      T != ref
     *  4 : 元素类型不能为右值                      T != obj
     *  5 : 元素类型只能为变量                      T == obj
     *  6 : 元素类型只能为指针                      T == ptr
     *  7 : 元素类型只能为引用                      T == ref
     *  8 : 元素类型只能为右值                      T == obj
     *  9 : 数据类型不能为指针                      T >> obj
     *  10: 数据类型必须为指针                      T >> ptr
     *  11: 数据类型不能基于参数(参数不能为指针)       T <> (dtype)
     */
    int rule = 0;

    /**
     * @member arg : 限制参数
     * @desc :
     *  11号限制规则需要一个数据类型作为参数
     */
    $typeuc arg;

    /**
     * @member index : 模板参数下标
     * @desc :
     *  谓词单元所修饰的模板参数在模板参数列表中的下标
     */
    int index;
};

/**
 * @class prede : predicate expression 谓词表达式
 * @desc :
 *  谓词表达式由谓词单元构成,当谓词表达式中所有谓词单元都成立
 *  谓词表达式成立
 */
using prede = chainz<predi>;

/**
 * @class preds : predicate expressions 谓词表达式集合
 * @desc :
 *  若所有谓词表达式都不成立,则模板类用例创建失败
 */
using preds = chainz<prede>;

/**
 * @struct dbra : definition branch 定义分支
 * @desc :
 *  定义分支将一系列定义与某个谓词表达式关联起来
 *  若对应的谓词表达式不成立,则定义分支所包含的
 *  一系列定义都将被抛弃
 */
struct dbra {

    /**
     * @member offset : 分支的起始
     */
    int offset;

    /**
     * @member count : 分支的范围
     */
    int count;

    /**
     * @member index : 分支对应的谓词表达式下标
     */
    token index;
};

/**
 * @class dbras : definition branchs 定义分支容器
 * @desc :
 *  方便容纳定义分支的容器
 */
using dbras = chainz<dbra>;

struct plist : public ConstructImpls {
    public:
        /**
         * @member vargs : 可变参数
         * @desc : 
         *  用于描述有关可变参数的信息
         *  若此成员为 ETC 表示方法采取动态可变参数策略接受可变参数
         *  若此成员为 LABEL 表示方法采取静态可变参数策略接受可变参数
         *  其他情况,视为方法不支持可变参数
         */
        token vargs;
};

struct opsig {

    public:
        /**
         * @member subtitle : 副标题
         * @desc :
         *  有些运算符携带副标题
         *  注意：as运算符的目标类型被合成在返回类型里。
         */
        token subtitle;

        /**
         * @member rproto : 返回值元素原型
         * @desc :
         *  描述运算符的返回值元素原型
         *  并不是所有的运算符都具有返回值，此时返回值原型是nullptr
         */
        $eproto rproto;

        /**
         * @member modifier : 修饰
         * @desc :
         *  运算符定义可以携带一个修饰符
         *  修饰符的取值范围: {rev,ism,prefix,suffix}
         */
        token modifier;

        /**
         * @member constraint : 约束
         * @desc :
         *  运算符是否被约束为不能修改this对象。
         */
        token constraint;
};

struct ctor : public map<token,$ExpressionImpl> {
    public:
        /**
         * @member bctor : 基类构造表达式
         * @desc :
         *  指定预先调用基类构造运算符时使用的构造表达式
         */
        chainz<$ExpressionImpl> bctor;

        /**
         * @member post : 后置过程
         * @desc :
         *  构造之后需要进行的过程。
         */
        $InsBlockImpl post;
};

}

#endif