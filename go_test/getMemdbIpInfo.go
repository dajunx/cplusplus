package main

import (
	"fmt"
	"strings"

	"github.com/PuerkitoBio/goquery"
)

func getContentAndIp(doc *goquery.Document, nodeInfo string) {
	doc.Find(nodeInfo).Each(func(i int, s *goquery.Selection) {
		//band := s.Find("td").Text()
		band := s.Text()
		if i == 1 {
			fmt.Print(band)
			content, err := s.Attr("ondblclick")
			if err != true {
				fmt.Print("get ip err")
			}
			allIps := strings.Split(content, "'")
			ips := strings.Split(allIps[1], "/")
			fmt.Printf(" ip1:%s, ip2:%s", ips[0], ips[1])
		}

		if i == 24 {
			document1 := strings.Contains(band, "Ŀ¼1")
			document2 := strings.Contains(band, "Ŀ¼2")
			if document1 {
				fmt.Printf(" Ŀ¼1 \n")
			}
			if document2 {
				fmt.Printf(" Ŀ¼2 \n")
			}
		}
	})
}

func main() {
	var Url string = "http://serverreport.ishow.cn/dbapi/stateinfo/audio_memdb_state.php?sid=151"
	doc, err := goquery.NewDocument(Url)
	if err != nil {
		fmt.Print("err")
	}

	// Find the review items
	getContentAndIp(doc, ".listtable .oum1 td")
	getContentAndIp(doc, ".listtable .oum2 td")
}
