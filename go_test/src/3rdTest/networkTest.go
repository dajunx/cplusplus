package officialLibTest

import (
	"net"
	"fmt"
	"log"
	"time"
)

/*
	网络编程小例子，服务端和客户端实现代码
 */

func handleConn(c net.Conn){
	defer c.Close()
	sendClientData := "ok."
	data := make([]byte, 30)
	for{
		count ,err := c.Read(data)
		if err != nil{
			return
		}
		fmt.Printf("[server] read client data count:%d, data value:%s\n", count, data)
		copy(data, sendClientData)
		c.Write(data)
	}
}

func server() {
	// 监听本地tcp的8000端口
	listener,err := net.Listen("tcp","localhost:8000")
	if err != nil{
		log.Fatal(err)
	}
	for {
		conn,err := listener.Accept()
		fmt.Printf("[sever] new connect, ip info:%s\n", conn.RemoteAddr().String())
		if err!= nil{
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

func client() {
	conn,err := net.Dial("tcp","localhost:8000")
	if err != nil{
		log.Fatal(err)
	}
	defer conn.Close()

	inputStr := "a client message."
	data := make([]byte, 30)
	copy(data, inputStr)
	conn.Write(data) // 发送数据

	_,err = conn.Read(data) //接收数据
	fmt.Printf("[client] receive data from server, data:%s\n", data)
}

func TestNet() {
	go server()
	go client()
	time.Sleep(time.Second * 5)
}
