package basicuse

import "fmt"

//TestUseArray 数组的使用
func TestUseArray() {
	// 表达式 var a [10]int
	// 数组的长度是其类型的一部分，因此数组不能改变大小
	var a [2]string
	a[0] = "Hello"
	a[1] = "World"
	fmt.Println(a[0], a[1])
	fmt.Println(a)
}
