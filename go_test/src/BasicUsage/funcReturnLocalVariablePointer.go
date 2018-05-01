package basicuse

import "fmt"

func getLocalVariablePorint() *int {
	i := 999
	return &i
}

//TestReturnFunctionLocalVariablePoint 测试返回函数本地变了指针
func TestReturnFunctionLocalVariablePoint() {
	var pi *int
	pi = getLocalVariablePorint()
	k := 888
	fmt.Println("k value:", k)
	fmt.Println("fun return value:", *pi)
}
