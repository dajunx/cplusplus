package main

import (
	"fmt"
	"math"
)

// 和c中if很类似，只是没有小括号
func sqrt(x float64) string {
	if x < 0 {
		return sqrt(-x) + "i"
	}
	return fmt.Sprint(math.Sqrt(x))
}

//可以在条件之前执行一个简单的语句。
// 由这个语句定义的变量的作用域仅在 if 范围之内。
// 在 if 的便捷语句定义的变量同样可以在任何对应的 else 块中使用
func pow(x, n, lim float64) float64 {
	if v := math.Pow(x, n); v < lim {
		return v
	} else {
		fmt.Printf("%g >= %g\n", v, lim)
	}
	// 变量 v 已经无法使用
	return lim
}

func main() {
	fmt.Println(sqrt(2), sqrt(-4))
	fmt.Println(
		pow(3, 2, 10),
		pow(3, 3, 20),
	)
}