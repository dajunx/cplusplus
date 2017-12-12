package main

import (
    "fmt"
    "runtime"
    "time"
)

func testSwitch1() {
    // switch 其实就是一个字段的集合
    // 执行顺序是从上到下的执行，当匹配成功的时候停止
    switch os := runtime.GOOS; os {
    case "darwin":
        fmt.Println("OS X.")
    case "linux":
        fmt.Println("Linux.")
    case "windows":
        fmt.Println("windows.")
	fallthrough //阻止分支自动结束
    default:
        // freebsd, openbsd,
        // plan9, windows...
        //fmt.Printf("%s.", os)
        fmt.Printf("step into default case.")
    }  
}

//没有条件的 switch 同 `switch true` 一样，
// 用来比对所有case的条件, c当中应该没没有如此使用方式
func noInputSwitchTest() {
	t := time.Now()
	switch {
	case t.Hour() < 12:
		fmt.Println("Good morning!")
	case t.Hour() < 17:
		fmt.Println("Good afternoon.")
	default:
		fmt.Println("Good evening.")
	}
}

func main() {
    fmt.Print("Go runs on ")
    testSwitch1()
    fmt.Println("test no input switch.")
    noInputSwitchTest()
}