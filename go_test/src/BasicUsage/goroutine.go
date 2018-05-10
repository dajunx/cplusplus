package basicuse

import (
	"fmt"
	"time"
)

func thread1() {
	for i := 1; i < 10; i++ {
		fmt.Printf("thread1: i:%d\n", i)
		time.Sleep(time.Millisecond * 10)
	}
}

func thread2() {
	for i := 1; i < 10; i++ {
		fmt.Printf("thread2: i:%d\n", i)
		time.Sleep(time.Millisecond * 10)
	}
}

// go的并发测试 叫做 goroutine TODO
//URL: http://www.cnblogs.com/zhaof/p/8393091.html
func TestThread() {
	go thread1()
	go thread2()
	time.Sleep(time.Second * 2)
}

