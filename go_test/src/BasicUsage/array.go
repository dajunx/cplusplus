package basicuse

import "fmt"

//多维数组测试
func MultiArray() {
	var aa [2][2]int
	aa = [2][2]int{{1,2}, {3,4}}
	for _,i := range aa {
		for _,j := range i {
			fmt.Println(j)
		}
	}
}

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
