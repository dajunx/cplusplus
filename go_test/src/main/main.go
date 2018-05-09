package main

import (
	"fmt"
	"selfDb"
	"selfTools"
	"strconv"
	"testDir"
	"time"
)

//init 放在任何包中都会首先被调用，在main之前
func init() {
	//fmt.Println("[main.go] test init call.")
}

//获取网易歌曲含有评论的连接地址
func useMusic() {
	//netManage.TestGetURL()
	// 网易歌曲原始播放地址，含有评论
	songURL := "https://music.163.com/#/song?id="

	//遍历歌曲json API，获取有评论数的歌曲链接
	songIDBegin := 306665
	songIDEnd := songIDBegin + 100
	for songID := songIDBegin; songID <= songIDEnd; songID++ {
		JSONContent := fileManage.DownloadSongJSON(songID)
		ret := fileManage.ParseJSON(JSONContent)
		if ret == true {
			fmt.Println(songURL + strconv.Itoa(songID))
		}
		time.Sleep(time.Millisecond * 100)
	}
}

//测试db链接
func useDb() {
	dbMange.ConnectLocalDb()
}

func totalTest() {
	testMain.Test()
}

// 给频道添加/删除机器人
func testAddDelRobot() {
	testMain.AddRobotUser()
	testMain.ExitRobotUser()
}

func testGoEvi() {
	fmt.Println("hello world")
}

func main() {
	//useMusic()
	//useDb()
	//fileManage.SaveToLocalFile1("input.txt", "output.txt")

	//testAddDelRobot()
	//basicuse.TestUseArray()
	testGoEvi()
	fmt.Println("process finished.")
}
