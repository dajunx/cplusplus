package dbMange

import (
	"database/sql"
	"log"
	"os"

	_ "github.com/mattn/go-sqlite3" //
)

func openDB() *sql.DB {
	db, err := sql.Open("sqlite3", "F:/github_git/go_test/src/temp/cnblogData.db")
	if err != nil {
		log.Fatal(err)
	}
	//defer db.Close()
	return db
}

//CreateSqliteTable 创建表语句
func CreateSqliteTable() {

	if _, err := os.Stat("F:/github_git/go_test/src/temp/cnblogData.db"); os.IsNotExist(err) == false {
		//log.Fatal(err)
		return
	}

	db := openDB()
	defer db.Close()

	sqlStmt := `
	create table cnblogData (
		id integer not null primary key, 
		title text,
		contentURL text,
		summary text,
		authorURL text,
		time text
		);
	delete from cnblogData;
	`
	_, err1 := db.Exec(sqlStmt)
	if err1 != nil {
		log.Printf("%q: %s\n", err1, sqlStmt)
		return
	}
}

//AddDataToTable x
func AddDataToTable(index int, data []string) {
	db := openDB()
	defer db.Close()

	tx, err := db.Begin()
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

//QueryMaxid 1
func QueryMaxid() int {
	db := openDB()
	defer db.Close()

	maxID := 0
	rows, err := db.Query("select count(*) from cnblogData")
	if err != nil {
		log.Fatal(err)
	}
	defer rows.Close()
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

//ClearData 1
func ClearData() {
	db := openDB()
	defer db.Close()

	_, err := db.Exec("delete from cnblogData")
	if err != nil {
		log.Fatal(err)
	}
}
