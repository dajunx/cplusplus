package main

import (
	"fmt"
	"Tools"
)

//init 放在任何包中都会首先被调用，在main之前
func init() {
	//fmt.Println("[main.go] test init call.")
}

func main() {
	//useMusic()
	//useDb()
	//fileManage.TestFileReadAndWrite()
	fileManage.TestUseJSON()

	//testAddDelRobot()
	//basicuse.TestUseArray()
	fmt.Println("process finished.")
}
