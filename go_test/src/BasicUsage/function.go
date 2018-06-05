package basicuse

import "fmt"

//常规函数
func add1(x int, y int) int {
	return x + y
}

//省略函数参数类型
func add2(x, y int) int {
	return x - y
}

//多返回值
func swap(x, y string) (string, string) {
	return y, x
}

//命名返回，return 语句返回结果的当前值
func split(sum int) (x, y int) {
	x = sum * 4 / 9
	y = sum - x
	return
}

//TestUseNormalFunction 简单函数的使用
func TestUseNormalFunction() {

	//函数的特殊使用方法，如下：
	func (msg string) {
		fmt.Println(msg)
	}("lin")

	f := func (msg string) {
		fmt.Println(msg)
	}
	f("lin")

	fmt.Println(add1(42, 13))
	fmt.Println(add2(42, 13))
	fmt.Println(swap("42", "13"))
	fmt.Println(split(42))
}
