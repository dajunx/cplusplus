package main

import "fmt"

func fun1() *int {
    var i int = 999
    return &i
}

func main() {
    var pi *int
    pi = fun1()
    k := 888
    fmt.Println("k value:", k)
    fmt.Println("fun return value:", *pi)
}