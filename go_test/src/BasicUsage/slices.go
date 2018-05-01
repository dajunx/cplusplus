package basicuse

import "fmt"

//slice 由函数 make 创建。这会分配一个零长度的数组并且返回一个 slice 指向这个数组
func newSlice() {
	a := make([]int, 5)
	printSlice("a", a)
	b := make([]int, 0, 5)
	printSlice("b", b)
	c := b[:2]
	printSlice("c", c)
	d := c[2:5]
	printSlice("d", d)
}

//slice 的零值是 `nil`。一个 nil 的 slice 的长度和容量是 0
func nilSlice() {
	var z []int
	fmt.Println(z, len(z), cap(z))
	if z == nil {
		fmt.Println("nil!")
	}
}

func printSlice(s string, x []int) {
	fmt.Printf("%s len=%d cap=%d %v\n",
		s, len(x), cap(x), x)
}

//添加元素
func appendDataToSlice() {
	var a []int
	printSlice("a", a)

	// append works on nil slices.
	a = append(a, 0)
	printSlice("a", a)

	// the slice grows as needed.
	a = append(a, 1)
	printSlice("a", a)

	// we can add more than one element at a time.
	a = append(a, 2, 3, 4)
	printSlice("a", a)
}

//TestSlices 切片的使用
func TestSlices() {
	//一个 slice 会指向一个序列的值，并且包含了长度信息
	p := []int{2, 3, 5, 7, 11, 13}
	fmt.Println("p ==", p)

	// slice 下表从0开始，切片是左闭右开
	fmt.Println("p[1:4] ==", p[1:4])
	// 省略下标代表从 0 开始
	fmt.Println("p[:3] ==", p[:3])
	// 省略上标代表到 len(s) 结束
	fmt.Println("p[4:] ==", p[4:])

	for i := 0; i < len(p); i++ {
		fmt.Printf("p[%d] == %d\n", i, p[i])
	}

	fmt.Println("new slice")
	newSlice()

	fmt.Println("nil slice")
	nilSlice()

	fmt.Println("add itmes to slice")
	appendDataToSlice()
}
