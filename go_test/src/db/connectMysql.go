package dbMange

import (
	"database/sql"
	"fmt"

	_ "github.com/go-sql-driver/mysql"
)

var pMysqlDB *sql.DB
var connectMysqlDBStatus bool = false

func init() {
	//initConnectMysql()
}

func initConnectMysql(connInfo string) bool {
	db, err := sql.Open("mysql", connInfo)
	if err != nil {
		fmt.Printf("failed connect mysql, err code:%s\n", err)
		connectMysqlDBStatus = false
	} else {
		pMysqlDB = db
		connectMysqlDBStatus = true
	}

	return connectMysqlDBStatus
}

func executeMysql(sql string) bool {
	if connectMysqlDBStatus != true {
		return false
	}

	rows, err := pMysqlDB.Query(sql)
	if err != nil {
		fmt.Printf("failed execute sql, err info:[%s], source sql:[%s]\n", err, sql)
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

/*
	参数说明：connInfo 形如："DBUserName:DBPWD@tcp(IP:Port)/DatabaseName"
 */
func TestConnectJDClondMysql(connInfo string) {
	if initConnectMysql(connInfo) == false { // 连接数据库异常
		return
	}

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
			fmt.Printf("[JDClond]execute `%s` succ.\n", sql)
		} else {
			fmt.Printf("execute mysql meet error, return.\n")
			break
		}
	}
}
