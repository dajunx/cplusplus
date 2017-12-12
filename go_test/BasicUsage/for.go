package main

import "fmt"

//常规for的使用，类似c
func normalCalcTotalNum1(sum int) {
	for i := 0; i < 10; i++ {
		sum += i
	}
	fmt.Println(sum)
}

//只包含判断条件的 for循环
func normalCalcTotalNum2(sum int) {
	i := 0
	for i < 10 {
		sum += i
		i++
	}
	fmt.Println(sum)
}

//无判断条件的 for循环
func normalCalcTotalNum3(sum int) {
	i := 0
	for {
		if i >= 10 {
			break
		}
		sum += i
		i++
	}
	fmt.Println(sum)
}

func main() {
	normalCalcTotalNum1(0)
	normalCalcTotalNum2(1)
	normalCalcTotalNum3(2)
}