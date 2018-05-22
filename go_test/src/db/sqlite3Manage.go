package dbMange

import (
	"database/sql"
	"fmt"
	_ "github.com/mattn/go-sqlite3" //
	"log"
)

var pSqliteDB *sql.DB
var connSqliteDBStatus bool = false

func init() {
	db, err := sql.Open("sqlite3", "F:/github_git/go_test/src/temp/test.db")
	if err != nil {
		fmt.Printf("init sqlite3 failed, err code:%s\n", err)
		connSqliteDBStatus = false
		return
	}

	pSqliteDB = db
	connSqliteDBStatus = true
}

//AddDataToTable x
func AddDataToTable(index int, data []string) {
	if connSqliteDBStatus == false {
		return
	}

	defer pSqliteDB.Close()

	tx, err := pSqliteDB.Begin()
	if err != nil {
		log.Fatal(err)
	}
	stmt, err := tx.Prepare("insert into cnblogData(id, title, contentURL, summary, authorURL, time) values(?, ?, ?, ?, ?, ?)")
	if err != nil {
		log.Fatal(err)
	}
	defer stmt.Close()

	_, err = stmt.Exec(index, data[0], data[1], data[2], data[3], data[4])
	if err != nil {
		log.Fatal(err)
	}

	tx.Commit()
}

func ExecutionSql(inputSql string) bool {
	if connSqliteDBStatus == false {
		return false
	}

	_, err := pSqliteDB.Exec(inputSql)
	if err != nil {
		fmt.Printf("ExecutionSql failed, err code:%s\n", err)
	}

	return true
}

func QueryWithSql(inputSql string) int {
	if connSqliteDBStatus == false {
		return 0
	}

	rows, err := pSqliteDB.Query(inputSql)
	if err != nil {
		log.Fatal(err)
	}
	defer rows.Close()

	maxID := 0
	for rows.Next() {
		err = rows.Scan(&maxID)
		if err != nil {
			log.Fatal(err)
		}
	}

	err = rows.Err()
	if err != nil {
		log.Fatal(err)
	}

	return maxID
}

func TestSqliteUse() {
	if connSqliteDBStatus == false {
		return
	}

	var inputSql []string
	inputSql = append(inputSql, `create table if not exists test (
		id integer not null primary key,
		name text,
		pwd integer
		);`)
	inputSql = append(inputSql, `delete from test`)
	inputSql = append(inputSql, `insert into test values(1, "111", 1)`)
	inputSql = append(inputSql, `insert into test values(2, "222", 2)`)
	inputSql = append(inputSql, `insert into test values(3, "333", 3)`)
	for _, sql := range inputSql {
		err := ExecutionSql(sql)
		if err == true {
			fmt.Println("execution sql succ.")
		}
	}

	defer pSqliteDB.Close()
}
