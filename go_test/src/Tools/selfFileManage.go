package fileManage

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"log"
	"os"
)

//ReadLineByLineFromLocalFile2 读取本地文件内容，整个内容一起读取
func ReadLineByLineFromLocalFile2(filename *string) {
	dat, err := ioutil.ReadFile(*filename)
	if err != nil {
		panic(err)
	}
	fmt.Print(string(dat))
}

//ReadLineByLineFromLocalFile1 读取本地文件内容，一行一行
func ReadLineByLineFromLocalFile1(filename *string) ([]string, bool) {
	var fileContents []string
	file, err := os.Open(*filename)
	if err != nil {
		log.Fatal(err)
		return fileContents, false
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		fmt.Println(scanner.Text())
		fileContents = append(fileContents, scanner.Text())
		fileContents = append(fileContents, "\n")
	}

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	return fileContents, true
}

//SaveToLocalFile 参考: https://stackoverflow.com/questions/1821811/how-to-read-write-from-to-file-using-go
//功能：保存内容到outPutFilename 文件中
func SaveToLocalFile(outPutFilename string, content []string) bool {
	result := false
	// open output file
	fo, err := os.Create(outPutFilename)
	if err != nil {
		panic(err)
	}
	// close fo on exit and check for its returned error
	defer func() {
		if err := fo.Close(); err != nil {
			panic(err)
		}
	}()

	// write a chunk
	i := 0
	var buf string
	for ; i < len(content); i++ {
		buf = content[i]
		if _, err := fo.Write([]byte(buf)); err != nil {
			panic(err)
		}
	}
	result = true

	return result
}

//TestFileReadAndWrite 测试用例
/*
	创建文件, 填充内容, 打印填充的内容, 删除新建的文件
*/
func TestFileReadAndWrite() {
	var contentSrc []string
	filename := "testWriteFileAndReadFile.log"
	defer os.Remove(filename)

	for i := 100; i < 200; i++ {
		contentSrc = append(contentSrc, fmt.Sprintf("%d\n", i))
	}

	SaveToLocalFile(filename, contentSrc)
	contentFromDiskFile, err := ReadLineByLineFromLocalFile1(&filename)
	if err != true {
		fmt.Println(err)
		return
	}
	for _, singleLineContent := range contentFromDiskFile {
		fmt.Print(singleLineContent)
	}
}
