package basicuse

import (
	"fmt"
	"math"
)

type vertexMethod struct {
	X, Y float64
}

//方法
//方法接收者 出现在 func 关键字和方法名之间的参数中
func (v *vertexMethod) Abs1() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

//可以对包中的 任意 类型定义任意方法，而不仅仅是针对结构体
//但是，不能对来自其他包的类型或基础类型定义方法
type myFloatMethod float64

func (f myFloatMethod) Abs2() float64 {
	if f < 0 {
		return float64(-f)
	}
	return float64(f)
}

//TestMethod 方法
func TestMethod() {
	v := &vertexMethod{3, 4}
	fmt.Println(v.Abs1())
	f := myFloatMethod(-math.Sqrt2)
	fmt.Println(f.Abs2())
}
