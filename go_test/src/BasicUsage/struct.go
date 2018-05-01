package basicuse

import "fmt"

//一个结构体（`struct`）就是一个字段的集合
type VertexStr struct {
	X int
	Y int
}

//结构体文法表示通过结构体字段的值作为列表来新分配一个结构体。
//使用 Name: 语法可以仅列出部分字段。(字段名的顺序无关。)
var (
	v1 = VertexStr{1, 2}  // 类型为 VertexStr
	v2 = VertexStr{X: 1}  // Y:0 被省略
	v3 = VertexStr{}      // X:0 和 Y:0
	p  = &VertexStr{1, 2} // 类型为 *VertexStr
)

//TestStruct 结构体的使用
func TestStruct() {
	fmt.Println(VertexStr{1, 2})
	v := VertexStr{1, 2}
	v.X = 4 //结构体字段使用点号来访问
	fmt.Println(v.X)

	//结构体字段可以通过 结构体指针来访问
	p := &v
	p.X = 1e9
	fmt.Println(v)

	fmt.Println(v1, p, v2, v3)
}
