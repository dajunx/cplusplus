package officialLibTest

import (
	"fmt"
	"strings"
)

func testReader(inputStr *string) {
	newReader := strings.NewReader(*inputStr)
	inputStrSize := newReader.Size()
	fmt.Printf("inputStr size:%d\n", inputStrSize)
	fmt.Printf("inputStr size:%d\n", len(*inputStr))

	// 只有通过 make 构造的 slice对象才能从 string.Reader中获取字符内容
	des := make([]byte, len(*inputStr))
	count,_ := newReader.Read(des)
	fmt.Printf("inputStr size:%d\n, content:%s", count, des)
}

func TestStringFunc() {
	var defaultStr string = "test strings."
	testReader(&defaultStr)
}