package testMain

import (
	"Tools"
	"strconv"
	"time"
)

//AddRobotUser 给频道增加机器人
func AddRobotUser() {
	URLTemp1 := "http://172.20.24.105:1808/?passwrod=1234567890&cmd=robot_enter&top_channel_id=104&sub_channel_id=1&sex=1&nickname=1&sid=1&uid="
	URLTemp2 := "&ct=1&signature=1&score=1&rank=50&partner_id=1&platform_member=2&seal_id=1&seal_duration=1&seal_mode=1&main_channel_id=1&is_audio_hidden_user=0"
	var URL string
	var index = 1
	for ; index < 60; index++ {
		URL = URLTemp1 + strconv.Itoa(index) + URLTemp2
		fileManage.WebGetAction(URL)
		time.Sleep(time.Millisecond * 100)
	}
}

//ExitRobotUser 清理频道机器人
func ExitRobotUser() {
	//http://172.20.24.105:1808/?passwrod=1234567890&cmd=robot_exit&top_channel_id=104&uid=1&ct=1
	URLTemp1 := "http://172.20.24.105:1808/?passwrod=1234567890&cmd=robot_exit&top_channel_id=104&uid="
	URLTemp2 := "&ct=1"
	var URL string
	var index = 1
	for ; index < 120; index++ {
		URL = URLTemp1 + strconv.Itoa(index) + URLTemp2
		fileManage.WebGetAction(URL)
		time.Sleep(time.Millisecond * 100)
	}
}
