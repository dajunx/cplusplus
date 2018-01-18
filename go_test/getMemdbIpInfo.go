package main

import (
	"fmt"
	"strings"

	"github.com/PuerkitoBio/goquery"
)

func getIpAndDirIndex(doc *goquery.Document, nodeInfo string) {
	doc.Find(nodeInfo).Each(func(i int, s *goquery.Selection) {
		tdAttr, exists := s.Attr("ondblclick")
		if exists && strings.Contains(tdAttr, "Copy") {
			allIps := strings.Split(tdAttr, "'")
			ips := strings.Split(allIps[1], "/") //拆分出ip1和ip2
			fmt.Printf("%s ", ips[0])
		}

		tdAttr, exists = s.Attr("align")
		if exists && strings.Contains(tdAttr, "center") {
			directoryText := s.Text()
			if strings.Contains(directoryText, "目录1") {
				fmt.Printf("1\n") //memdb 目录序号
			} else if strings.Contains(directoryText, "目录2") {
				fmt.Printf("2\n") //memdb 目录序号
			} else {
				fmt.Printf("err!\n")
			}
		}
	})
}

func getAllSidNum(Url *string) ([]string, error) {
	var ret []string
	doc, err := goquery.NewDocument(*Url)
	if err != nil {
		fmt.Print(err)
		return ret, err
	}

	doc.Find("body a").Each(func(i int, s *goquery.Selection) {
		attrName, exists := s.Attr("href")
		if exists && strings.Contains(attrName, "memdb.php?cid") {
			ret = append(ret, s.Text())
		}
	})
	return ret, err
}

func main() {
	var Url string = "http://www.xxxxx.com" //target url
	sids, errSids := getAllSidNum(&Url)
	if errSids != nil {
		fmt.Print(errSids)
		return
	}
	fmt.Println(sids)
	//遍历所有频道
	for _, v := range sids {
		url := Url + "?sid=" + v //拼凑单个语音组网页
		doc, err := goquery.NewDocument(url)
		if err != nil {
			fmt.Print(err)
		}
		//fmt.Printf("sid=%s\n", v) //sid号
		getIpAndDirIndex(doc, ".listtable .oum1 td")
		getIpAndDirIndex(doc, ".listtable .oum2 td")
	}
}