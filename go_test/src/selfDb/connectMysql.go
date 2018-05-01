package dbMange

import (
	"database/sql"
	"fmt"

	_ "github.com/go-sql-driver/mysql"
)

func init() {
	//fmt.Println("main package's int func.")
}

//ConnectLocalDb 链接本地数据库
func ConnectLocalDb() {
	db, err := sql.Open("mysql", "root:@/test")
	if err != nil {
		fmt.Println("connect mysql err.")
	}

	rows, err := db.Query("select * from test1")
	if err != nil {
		fmt.Println("Query select err.")
	}

	for rows.Next() {
		var id int
		var name string
		rows.Scan(&id, &name)
		fmt.Println(id)
		fmt.Println(name)
	}
}
