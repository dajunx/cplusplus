package main

import "fmt"

type Vertex struct {
    Lat, Long float64
}
//map 定义
var m map[string]Vertex

var n = map[string]Vertex{
    "Bell Labs": Vertex{
        40.68433, -74.39967,
    },  
    "Google": Vertex{
        37.42202, -122.08408,
    },  
}

//修改map中的值
func modifyMapItem(m map[string]Vertex) {
    m["Answer"] = Vertex{42,0}
    fmt.Println("The value:", m["Answer"])

    //持续修改值
    m["Answer"] = Vertex{48,0}
    fmt.Println("The value:", m["Answer"])

    //删除
    delete(m, "Answer")
    fmt.Println("The value:", m["Answer"])

    v, ok := m["Answer"]
    fmt.Println("The value:", v, "Present?", ok) 
}

func main() {
    //map 在使用之前必须用 make 而不是 new 来创建；值为 nil 的 map 是空的，并且不能赋值
    m = make(map[string]Vertex)
    m["Bell Labs"] = Vertex{
        40.68433, -74.39967,
    }   
    fmt.Println(m["Bell Labs"])

    //map 的文法跟结构体文法相似，不过必须有键名
    fmt.Println(n)

    fmt.Println("modify m map data")
    modifyMapItem(m)
}