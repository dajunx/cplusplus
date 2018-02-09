//类型转换操作符 operator int() const
/*
类型转换操作符
是一种特殊的类成员函数，它定义将类类型值转变为其他类型值的转换。转换操作符在类定义体内声明，在保留字
operator 之后跟着转换的目标类型。
1.
转换函数必须是成员函数，不能指定返回类型，并且形参表必须为空；返回值是隐含的，返回值是与转换的类型相同的，即为上面原型中的T2；
2. T2表示内置类型名（built-in type）、类类型名（class
type）或由类型别名（typedef）定义的名字； 对任何可作为函数返回类型的类型（除了
void
之外）都可以定义转换函数，一般而言，不允许转换为数组或函数类型，转换为指针类型（数据和函数指针）以及引用类型是可以的；
3. 转换函数一般不应该改变被转换的对象，因此转换操作符通常应定义为 const 成员；
4. 支持继承，可以为虚函数；
5. 只要存在转换，编译器将在可以使用内置转换的地方自动调用它；
可改写为 模板函数
*/

#include <iostream>
using namespace std;
class D {
public:
  D(double d) : d_(d) {}
  operator int() const {
    cout << "(int)d called!!" << endl;
    return static_cast<int>(d_);
  }
  /*
  template <typename T>
  operator T()const
  {
    cout << "(int)d called!!" << endl;
    return static_cast<T>(d_);
  }
  */
private:
  double d_;
};

int add(int a, int b) { return a + b; }

int main() {
  D d1 = 1.1;
  D d2 = 2.2;
  cout << "add(d1,d2)=" << add(d1, d2) << endl;
  return 0;
}