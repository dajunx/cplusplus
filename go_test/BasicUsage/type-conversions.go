package main

import (
	"fmt"
	"math"
)

func main() {
	var x, y int = 3, 4
	var f float64 = math.Sqrt(float64(x*x + y*y))
	// 类型转换，必须显示转换，表达式 T(v) 将值 v 转换为类型 `T`
	var z int = int(f)
	fmt.Println(x, y, z)
}