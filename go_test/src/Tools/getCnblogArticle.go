package main

import (
	"fmt"
	"os"
	"regexp"
	"strconv"
	"strings"

	"github.com/PuerkitoBio/goquery"
	"db"
)

type cnblog struct {
	title      string
	contentURL string
	summary    string
	authorURL  string
	time       string
}

func converCnblogDataToSimpleString(contents *[]cnblog) string {
	var allContent string
	for i := 0; i < len(*contents); i++ {
		perContent := (*contents)[i]
		allContent = allContent + "\n" + perContent.title
		allContent = allContent + "\n" + perContent.contentURL
		allContent = allContent + "\n" + perContent.summary
		allContent = allContent + "\n" + perContent.authorURL
		allContent = allContent + "\n" + perContent.time
	}

	return allContent
}

func saveToLocalFile1(outPutFilename string, contents string) {
	// open output file
	fo, err := os.Create(outPutFilename)
	if err != nil {
		panic(err)
	}
	// close fo on exit and check for its returned error
	defer func() {
		if err := fo.Close(); err != nil {
			panic(err)
		}
	}()

	// write a chunk
	if _, err := fo.Write([]byte(contents)); err != nil {
		panic(err)
	}
}

//GetWebChildData 获取制定网址元素信息
func GetWebChildData(inputURL *string, cnblogData *[]cnblog) ([]string, error) {
	var ret []string
	doc, err := goquery.NewDocument(*inputURL)
	if err != nil {
		fmt.Print(err)
		return ret, err
	}

	doc.Find(".post_item_body").EachWithBreak(func(i int, s *goquery.Selection) bool {
		//文章列表 标题和跳转连接url
		title := s.Find("h3 a")
		documentURL, _ := title.Attr("href")
		summary := s.Find("p")
		authorINFO, _ := s.Find(".post_item_foot a").Attr("href")
		reg := regexp.MustCompile(`[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}`)
		time := reg.FindString(s.Find(".post_item_foot").Text())
		//fmt.Printf("标题: %s\n", strings.Trim(title.Text(), " "))
		//fmt.Printf("文章链接: %s\n", strings.Trim(documentURL, " "))
		//fmt.Printf("简介: [%s]\n", strings.TrimSpace(summary.Text()))
		//fmt.Printf("作者博客地址: %s\n", strings.Trim(authorINFO, " "))
		//fmt.Printf("时间: %s\n", time)
		//fmt.Println("----------------------------------------------")

		*cnblogData = append(*cnblogData,
			cnblog{strings.TrimSpace(title.Text()),
				strings.TrimSpace(documentURL),
				strings.TrimSpace(summary.Text()),
				strings.TrimSpace(authorINFO),
				strings.TrimSpace(time)})
		return true
	})
	return ret, err
}

func main() {
	//inputURL := "https://www.cnblogs.com/#p8"
	var cnblogData []cnblog
	inputURL := "https://www.cnblogs.com/sitehome/p/"
	for pageCount := 1; pageCount < 200; pageCount++ {
		URL := inputURL + strconv.Itoa(pageCount)
		GetWebChildData(&URL, &cnblogData)
	}

	//保存到磁盘文件中
	//content := converCnblogDataToSimpleString(&cnblogData)
	//os.Remove("output.log")
	//saveToLocalFile1("output.log", content)

	//保存到sqlit中
	dbMange.CreateSqliteTable()
	dbMange.ClearData()
	index := dbMange.QueryMaxid()
	//index := 0
	for _, singleData := range cnblogData {
		var data []string
		data = append(data, singleData.title)
		data = append(data, singleData.contentURL)
		data = append(data, singleData.summary)
		data = append(data, singleData.authorURL)
		data = append(data, singleData.time)
		dbMange.AddDataToTable(index, data)
		index++
	}
}
