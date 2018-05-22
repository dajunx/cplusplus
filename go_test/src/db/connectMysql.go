package dbMange

import (
	"database/sql"
	"fmt"

	_ "github.com/go-sql-driver/mysql"
)

var pMysqlDB *sql.DB
var connectMysqlDBStatus bool

func init() {
	initConnectMysql()
}

func initConnectMysql() {
	db, err := sql.Open("mysql", "root:@/test")
	if err != nil {
		fmt.Printf("failed connect mysql, err code:%s\n", err)
		connectMysqlDBStatus = false
		return
	} else {
		pMysqlDB = db
	}

	connectMysqlDBStatus = true
}

func executeMysql(sql string) bool {
	if connectMysqlDBStatus != true {
		return false
	}

	rows, err := pMysqlDB.Query(sql)
	if err != nil {
		fmt.Printf("failed execute sql, err code:%s, source sql:%s\n", err, sql)
		return false
	}

	for rows.Next() {
		var id int = 0
		var name string = ""
		var pwd int = 0

		/*
			奇怪，不填写完所有的字段提取不了值，比如使用如下方式：
				rows.Scan(&id, &name)
		*/
		rows.Scan(&id, &name, &pwd)
		fmt.Printf("id:%d, name:%s, pwd:%d\n", id, name, pwd)
	}

	return true
}

func TestMysqlUse() {
	var sqlArray []string
	sqlArray = append(sqlArray, "delete from test")
	sqlArray = append(sqlArray, `insert into test values(1, "111", 1)`)
	sqlArray = append(sqlArray, `insert into test values(2, "222", 2)`)
	sqlArray = append(sqlArray, `insert into test values(3, "333", 3)`)
	sqlArray = append(sqlArray, `insert into test values(4, "444", 4)`)
	sqlArray = append(sqlArray, "select * from test")

	var resExecuteSql bool = false
	for _, sql := range sqlArray {
		resExecuteSql = executeMysql(sql)
		if resExecuteSql == true {
			fmt.Printf("execute `%s` succ.\n", sql)
		}
	}
}
