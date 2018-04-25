package main

import (
	"bufio"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"os"
)

func readLineByLineFromLocalFile2(filename *string) {
	dat, err := ioutil.ReadFile(*filename)
	if err != nil {
		panic(err)
	}
	fmt.Print(string(dat))
}

func readLineByLineFromLocalFile1(filename *string) []string {
	var fileContents []string
	file, err := os.Open(*filename)
	if err != nil {
		log.Fatal(err)
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

	return fileContents
}

//https://stackoverflow.com/questions/1821811/how-to-read-write-from-to-file-using-go
func wwwReadWriteLocalFileExample() {
	// open input file
	fi, err := os.Open("input.txt")
	if err != nil {
		panic(err)
	}
	// close fi on exit and check for its returned error
	defer func() {
		if err := fi.Close(); err != nil {
			panic(err)
		}
	}()

	// open output file
	fo, err := os.Create("output.txt")
	if err != nil {
		panic(err)
	}
	// close fo on exit and check for its returned error
	defer func() {
		if err := fo.Close(); err != nil {
			panic(err)
		}
	}()

	// make a buffer to keep chunks that are read
	buf := make([]byte, 1024)
	for {
		// read a chunk
		n, err := fi.Read(buf)
		if err != nil && err != io.EOF {
			panic(err)
		}
		if n == 0 {
			break
		}

		// write a chunk
		if _, err := fo.Write(buf[:n]); err != nil {
			panic(err)
		}
	}
}

func saveToLocalFile1(inputFileName string, outPutFilename string) {
	fileContent := readLineByLineFromLocalFile1(&inputFileName)

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
	for ; i < len(fileContent); i++ {
		buf = fileContent[i]
		if _, err := fo.Write([]byte(buf)); err != nil {
			panic(err)
		}
	}
}

func main() {
	saveToLocalFile1("input.txt", "output.txt")
	/*
		filename := "F:\\109_log_backup_history\\BatchRun_109_log_backup 2018-02-01 153000.log"
		readLineByLineFromLocalFile1(&filename)
		readLineByLineFromLocalFile2(&filename)
	*/
}
