package officialLibTest

import (
	"fmt"
	"regexp"
)

func testRegexp() {
	text := "Hello 世界！123 Go. 2018-05-09 11:03 fasdf"
	//reg1 := regexp.MustCompile("[a-z]+|[A-Z]+")
	//reg2 := regexp.MustCompile(`[\p{Han}]+|[a-z]+|[A-Z]+`) //匹配汉字/字母
	//fmt.Printf("%q\n", reg1.FindAllString(text, -1))
	//fmt.Printf("%s\n", reg1.ReplaceAllString(text, ""))
	//fmt.Printf("%s\n", reg2.ReplaceAllString(text, ""))

	reg3 := regexp.MustCompile(`[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}`) //匹配时间
	fmt.Printf("%s\n", reg3.FindString(text))

	re := regexp.MustCompile("fo.?")
	fmt.Printf("%s\n", re.FindAllString("seafoodfood", -1))
}
