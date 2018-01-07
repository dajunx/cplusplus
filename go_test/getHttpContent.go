package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

func main() {
	var baiduMainUrl string = "http://localhost/test/"
	//"https://pan.baidu.com/disk/home"
	resp, err := http.Get(baiduMainUrl)
	if err != nil {
		fmt.Println("get baidu content meet err.", err)
	}

	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	fmt.Printf("body=%s\n", body)
}
