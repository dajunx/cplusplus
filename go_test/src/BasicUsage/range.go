package basicuse

import "fmt"

var powRange = []int{1, 2, 4, 8, 16, 32, 64, 128}

//TestRangeUsage range的使用
func TestRangeUsage() {
	//for 循环的 range 格式可以对 slice 或者 map 进行迭代循环
	for i, v := range powRange {
		fmt.Printf("2**%d = %d\n", i, v)
	}

	//可以通过赋值给 _ 来忽略序号和值。如果只需要索引值，去掉“, value”的部分即可
	for i := range powRange {
		fmt.Println("pos value:", i)
	}
	for _, value := range powRange {
		fmt.Printf("%d\n", value)
	}
}
