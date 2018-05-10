package fileManage

import (
	"bytes"
	"compress/gzip"
	"encoding/json"
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"strconv"
	"strings"
	"time"
)

//WebGetAction 模拟GEt
func WebGetAction(url string) []byte {
	client := &http.Client{
		CheckRedirect: nil,
	}
	reqest, _ := http.NewRequest("GET", url, nil)

	reqest.Header.Set("User-Agent", " Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/31.0.1650.63 Safari/537.36")
	reqest.Header.Set("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8")
	reqest.Header.Set("Accept-Charset", "GBK,utf-8;q=0.7,*;q=0.3")
	reqest.Header.Set("Accept-Encoding", "gzip,deflate,sdch")
	reqest.Header.Set("Accept-Language", "zh-CN,zh;q=0.8")
	reqest.Header.Set("Cache-Control", "max-age=0")
	reqest.Header.Set("Connection", "keep-alive")
	reqest.Header.Set("Referer", url)

	//Cookie
	//if len(cookie) > 0 {
	//	fmt.Println("dealing with cookie:" + cookie)
	//	array := strings.Split(cookie, ";")
	//	for item := range array {
	//		array2 := strings.Split(array[item], "=")
	//		if len(array2) == 2 {
	//			cookieObj := http.Cookie{}
	//			cookieObj.Name = array2[0]
	//			cookieObj.Value = array2[1]
	//			reqest.AddCookie(&cookieObj)
	//		} else {
	//			fmt.Println("error,index out of range:" + array[item])
	//		}
	//	}
	//}

	resp, err := client.Do(reqest)
	if err != nil {
		fmt.Println(url, err)
		return nil
	}

	defer resp.Body.Close()
	var reader io.ReadCloser
	switch resp.Header.Get("Content-Encoding") {
	case "gzip":
		reader, err = gzip.NewReader(resp.Body)
		if err != nil {
			fmt.Println(url, err)
			return nil
		}
		defer reader.Close()
	default:
		reader = resp.Body
	}

	if reader != nil {
		body, err := ioutil.ReadAll(reader)
		if err != nil {
			fmt.Println(url, err)
			return nil
		}
		return body
	}
	return nil
}

func PostHttpRequest(url string, cookie string, postStr string) []byte {
	fmt.Println("let's post :" + url)

	client := &http.Client{
		CheckRedirect: nil,
	}

	postBytesReader := bytes.NewReader([]byte(postStr))
	reqest, _ := http.NewRequest("POST", url, postBytesReader)

	reqest.Header.Set("User-Agent", " Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/31.0.1650.63 Safari/537.36")
	reqest.Header.Set("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8")
	reqest.Header.Set("Accept-Charset", "GBK,utf-8;q=0.7,*;q=0.3")
	reqest.Header.Set("Accept-Encoding", "gzip,deflate,sdch")
	//	reqest.Header.Add("Content-Type", "application/x-www-form-urlencoded")
	reqest.Header.Add("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8")
	reqest.Header.Set("Accept-Language", "zh-CN,zh;q=0.8")
	reqest.Header.Set("Cache-Control", "max-age=0")
	reqest.Header.Set("Connection", "keep-alive")
	reqest.Header.Set("Referer", url)

	if len(cookie) > 0 {
		fmt.Println("dealing with cookie:" + cookie)
		array := strings.Split(cookie, ";")
		for item := range array {
			array2 := strings.Split(array[item], "=")
			if len(array2) == 2 {
				cookieObj := http.Cookie{}
				cookieObj.Name = array2[0]
				cookieObj.Value = array2[1]
				reqest.AddCookie(&cookieObj)
			} else {
				fmt.Println("error,index out of range:" + array[item])
			}
		}
	}

	resp, err := client.Do(reqest)

	if err != nil {
		fmt.Println(url, err)
		return nil
	}

	defer resp.Body.Close()

	var reader io.ReadCloser
	switch resp.Header.Get("Content-Encoding") {
	case "gzip":
		reader, err = gzip.NewReader(resp.Body)
		if err != nil {
			fmt.Println(url, err)
			return nil
		}
		defer reader.Close()
	default:
		reader = resp.Body
	}

	if reader != nil {
		body, err := ioutil.ReadAll(reader)
		if err != nil {
			fmt.Println(url, err)
			return nil
		}
		return body
	}
	return nil
}

// NormalGetURL 常规方式获取url内容
func NormalGetURL(URLTemp string) {
	//URLTemp := "http://localhost/test/a_json_file.php" //本地wampserver启动服务提供json文件访问
	//URLTemp := "http://127.0.0.1:9999/ReadMe.txt" //本地c++ 编写的web服务器
	//URLTemp := "https://music.163.com/#/song?id=306752"
	resp, err := http.Get(URLTemp)
	if err != nil {
		//
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		//
	}
	fmt.Println(string(body))
}

func downloadSongJSON(songID int) []byte {
	URLTemp := "http://music.163.com/api/v1/resource/comments/R_SO_4_"
	URLTemp += strconv.Itoa(songID)
	//fmt.Println(URLTemp)

	res := WebGetAction(URLTemp) //模拟web get 操作
	//fmt.Println(string(res))

	//如下方式会被正式网站判定非法请求
	/*
		resp, err := http.Get(URLTemp)
		if err != nil {
			ret = false
		}
		defer resp.Body.Close()
		body, err := ioutil.ReadAll(resp.Body)
		if err != nil {
			ret = false
		}
		fmt.Println(string(body))
	*/
	return res
}

func parseJSON(res []byte) bool {
	ret := false
	//解析json
	var dat map[string]interface{}
	if err := json.Unmarshal(res, &dat); err != nil {
		//panic(err)
	}

	//使用total字段判断 <评论条数>
	musicCommentsCount := dat["total"].(float64)
	if musicCommentsCount > 0 {
		ret = true
	}
	//fmt.Println(dat)
	return ret
}

//获取网易歌曲含有评论的连接地址
func get163MusicComment() {
	// 网易歌曲原始播放地址，含有评论
	songURL := "https://music.163.com/#/song?id="

	//遍历歌曲json API，获取有评论数的歌曲链接
	songIDBegin := 306665
	songIDEnd := songIDBegin + 100
	for songID := songIDBegin; songID <= songIDEnd; songID++ {
		JSONContent := downloadSongJSON(songID)
		ret := parseJSON(JSONContent)
		if ret == true {
			fmt.Println(songURL + strconv.Itoa(songID))
		}
		time.Sleep(time.Millisecond * 100)
	}
}
