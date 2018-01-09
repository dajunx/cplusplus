package main

import (
	"fmt"
	"strings"

	"github.com/PuerkitoBio/goquery"
)

func getIpAndIndex(doc *goquery.Document, nodeInfo string) {
	doc.Find(nodeInfo).Each(func(i int, s *goquery.Selection) {
		//band := s.Find("td").Text()
		band := s.Text()
		if i == 1 {
			//fmt.Print(band)
			content, err := s.Attr("ondblclick")
			if err != true {
				fmt.Print("get ip err")
			}
			allIps := strings.Split(content, "'")
			ips := strings.Split(allIps[1], "/")
			//fmt.Printf(" ip1:%s, ip2:%s", ips[0], ips[1]) //memdb主ip地址
			fmt.Printf("%s ", ips[0])
		}

		if i == 24 {
			document1 := strings.Contains(band, "目录1")
			document2 := strings.Contains(band, "目录2")
			if document1 {
				fmt.Printf("1\n") //memdb 目录序号
			}
			if document2 {
				fmt.Printf("2\n") //memdb 目录序号
			}
		}
	})
}

func getAllSidNum(Url string) []string {
	var ret []string
	doc, err := goquery.NewDocument(Url)
	if err != nil {
		fmt.Print("err")
	}

	doc.Find("body a").Each(func(i int, s *goquery.Selection) {
		attrName, exists := s.Attr("href")
		if exists && strings.Contains(attrName, "memdb.php?cid") {
			ret = append(ret, s.Text())
		}
	})
	return ret
}

func main() {
	var Url string = "http://serverreport.ishow.cn/dbapi/stateinfo/audio_memdb_state.php"
	sids := getAllSidNum(Url)
	fmt.Println(sids)
	//遍历所有频道
	for _, v := range sids {
		url := Url + "?sid=" + v //拼凑单个语音组网页
		//fmt.Println(url)
		doc, err := goquery.NewDocument(url)
		if err != nil {
			fmt.Print("err")
		}
		//fmt.Printf("sid=%s\n", v) //sid号
		getIpAndIndex(doc, ".listtable .oum1 td")
		getIpAndIndex(doc, ".listtable .oum2 td")
	}
}
