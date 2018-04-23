package main

import (
	"fmt"
	"math/cmplx"
	)

//`var` 语句可以定义在包或函数级别，
// 注意函数内与包体内函数初始化 差异
// 函数外定义变量必须以 var开头，且不能使用 := 结构
//var c, python, java bool
var c, python, java bool = true, false, false

//定义并初始化多个变量
var (
	ToBe   bool       = false
	MaxInt uint64     = 1<<64 - 1
	z      complex128 = cmplx.Sqrt(-5 + 12i)
)

func main() {
	//var i int
	//i := 999 //短声明变量，用在明确变量类型的地方
	var i int = 999
	fmt.Println(i, c, python, java)
	const f = "%T(%v)\n"
	fmt.Printf(f, ToBe, ToBe)
	fmt.Printf(f, MaxInt, MaxInt)
	fmt.Printf(f, z, z)
}
