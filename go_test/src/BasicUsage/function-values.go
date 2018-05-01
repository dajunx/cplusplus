package basicuse

import (
	"fmt"
	"math"
)

func adder() func(int) int {
	sum := 0
	return func(x int) int {
		sum += x
		return sum
	}
}

//TestReturnFunction 函数返回函数，函数也是一种值
func TestReturnFunction() {
	//函数也是值
	hypot := func(x, y float64) float64 {
		return math.Sqrt(x*x + y*y)
	}

	fmt.Println(hypot(3, 4))

	// 函数的闭包
	//TODO 函数的闭包的理解
	fmt.Println(" function-value 闭包")
	pos, neg := adder(), adder()
	for i := 0; i < 10; i++ {
		fmt.Println(
			pos(i),
			neg(-2*i),
		)
	}
}
