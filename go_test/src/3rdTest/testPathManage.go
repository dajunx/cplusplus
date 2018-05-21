package officialLibTest

import (
	"path/filepath"
	"os"
	"log"
	"fmt"
	"io/ioutil"
)

/*
	显示当前代码所在的目录
	参考链接:https://stackoverflow.com/questions/18537257/how-to-get-the-directory-of-the-currently-running-file
	注意：直接打印dir目录是一个临时目录，可以 build成程序，然后打印出来的目录就是当前程序所在目录
 */
func getCurrentDirectory() {

	dir, err := filepath.Abs(filepath.Dir(os.Args[0]))
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(dir)
}

/*
	滚动显示指定目录文件的两种实现方式:
*/
func listDirectoryFiles1(directoryName string) {
	file, _ := os.Open(directoryName)
	fileinfos, _ := file.Readdir(-1)
	for _, files := range fileinfos {
		fmt.Printf("%s\n", files.Name())
	}
	fmt.Printf("file count:%d\n", len(fileinfos))
}

func listDirectoryFiles2(directoryName string) {
	files, err := ioutil.ReadDir(directoryName)
	fmt.Printf("显示目录：%s 文件，文件数目:%d\n", directoryName, len(files))
	if err != nil {
		fmt.Println(err)
	} else {
		for _, filename := range files {
			if filename.IsDir() {
				fmt.Printf("目录名:%s\n", filename.Name())
			} else {
				//fmt.Printf("文件名:%s\n", filename.Name())
			}

		}
	}
}

func TestOSFunc() {
	getCurrentDirectory()

	var directoryName string = "f:\\pdf"
	listDirectoryFiles1(directoryName)
	listDirectoryFiles2(directoryName)
}