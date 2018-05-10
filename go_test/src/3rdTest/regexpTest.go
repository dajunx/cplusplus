package officialLibTest

import (
	"fmt"
	"regexp"
)

var defaultInputStr string = "Hello 世界！123 Go. 2018-05-09 11:03 fasdf"

//查找字符串中 日期，入 2018-05-10
func testRegexpFindOutTimeStr(inputStr string) {
	reg := regexp.MustCompile(`[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}`) //匹配时间
	fmt.Printf("%s\n", reg.FindString(inputStr))
}

//匹配汉字
func testFindOutChineseChr(inputStr string) {
	reg := regexp.MustCompile(`[\p{Han}]`)
	fmt.Printf("%s\n", reg.FindAllString(inputStr, -1)) //查找中文
	fmt.Printf("%s\n", reg.ReplaceAllString(inputStr, "")) //去除中文
}

// 测试go 正则表达式
func TestRegexp() {
	fmt.Printf("source string is: \n%s\n", defaultInputStr)
	testRegexpFindOutTimeStr(defaultInputStr)
	testFindOutChineseChr(defaultInputStr)
}

/* 输出：
source string is:
Hello 世界！123 Go. 2018-05-09 11:03 fasdf
2018-05-09 11:03
[世 界]
Hello ！123 Go. 2018-05-09 11:03 fasdf

 */