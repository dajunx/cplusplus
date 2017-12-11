package main

import "fmt"

//一个结构体（`struct`）就是一个字段的集合
type Vertex struct {
	X int
	Y int
}

//结构体文法表示通过结构体字段的值作为列表来新分配一个结构体。
//使用 Name: 语法可以仅列出部分字段。(字段名的顺序无关。)
var (
	v1 = Vertex{1, 2}  // 类型为 Vertex
	v2 = Vertex{X: 1}  // Y:0 被省略
	v3 = Vertex{}      // X:0 和 Y:0
	p  = &Vertex{1, 2} // 类型为 *Vertex
)

func main() {
	fmt.Println(Vertex{1, 2})
	v := Vertex{1, 2}
	v.X = 4 //结构体字段使用点号来访问
	fmt.Println(v.X)

	//结构体字段可以通过 结构体指针来访问
	p := &v
	p.X = 1e9
	fmt.Println(v)

	fmt.Println(v1, p, v2, v3)
}