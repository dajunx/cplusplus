package main

import "fmt"

//返回指针版本 func fun1(t bool, i* int) *int {
func fun1(t bool, i* int) int {
    switch {
    case t == false:
        *i--;
        fmt.Println("call --")
    case t == true:
        *i++;
        fmt.Println("call ++")
    default:
        fmt.Println("nothing")
    }   

    //fmt.Println("i value:", *i)
    return *i
}

func main() {
    //defer 语句会延迟函数的执行直到上层函数返回。
    // 延迟调用的参数会立刻生成，但是在上层函数返回前函数都不会被调用
    // 延迟的函数调用被压入一个栈中。当函数返回时， 会按照后进先出的顺序调用被延迟的函数调用
    i := 1
    defer fmt.Println("i value:", fun1(false, &i), ", type --")
    fmt.Println("i value:", fun1(true, &i), ", type ++")
}
