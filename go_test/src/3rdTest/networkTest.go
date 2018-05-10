package officialLibTest

import (
	"time"
	"net"
	"log"
	"io"
	"os"
)

func handleConn(c net.Conn){
	defer c.Close()
	for{
		//_,err := io.WriteString(c,time.Now().Format("15:04:05\r\n"))
		_,err := io.WriteString(c, "hello client,i'm server.\n")
		if err != nil{
			return
		}
		time.Sleep(1*time.Second)
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
		if err!= nil{
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//////////////////////////////////////////////////////

func client() {
	conn,err := net.Dial("tcp","localhost:8000")
	if err != nil{
		log.Fatal(err)
	}
	defer conn.Close()
	// 从连接中读取内容，并写到标准输出
	if _,err := io.Copy(os.Stdout, conn); err !=nil{
		log.Fatal(err)
	}
}

func TestNet() {
	go server()
	go client()
	time.Sleep(time.Second * 5)
}
