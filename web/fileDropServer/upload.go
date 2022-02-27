package main

import (
	"bufio"
	"fmt"
	"io"
	"net/http"
	"os"
	"time"
)

func uploadFile(w http.ResponseWriter, r *http.Request) {
	fmt.Println("File Upload Endpoint Hit")
	fmt.Fprintf(w, "Listening for file\n")

	r.ParseMultipartForm(10 << 27) // Max 1284 MiB
	file, handler, err := r.FormFile("f")
	if err != nil {
		fmt.Println("Error Retrieving the File")
		fmt.Println(err)
		return
	}
	defer file.Close()
	fmt.Printf("Uploaded File: %+v\n", handler.Filename)
	fmt.Printf("File Size: %+v\n", handler.Size)
	fmt.Printf("MIME Header: %+v\n", handler.Header)

	folderPath := "/usr/share/nginx/html/fileServer/uploads/" + time.Now().Format("2006-01-02/")
	if err := os.MkdirAll(folderPath, 0777); err != nil {
		fmt.Println("Failed createing directory")
		fmt.Println(err)
		return
	}
	outFile, err := os.Create(folderPath + handler.Filename)
	defer outFile.Close()
	if err != nil {
		fmt.Println("Failed to create file")
		fmt.Println(err)
		return
	}

	fileWriter := bufio.NewWriter(outFile)
	buf := make([]byte, 2048)

	for {
		n, err := file.Read(buf)
		if err != nil && err != io.EOF {
			print("Error reading from buffer")
			print(err)
			return
		}
		if n == 0 {
			break
		}

		// write a chunk
		if _, err := fileWriter.Write(buf[:n]); err != nil {
			print("Error writing to file")
			print(err)
			return
		}
	}

	if err = fileWriter.Flush(); err != nil {
		print("Error flushing writer")
		print(err)
		return
	}

	fmt.Fprintf(w, "Successfully Uploaded File\n")
}

func main() {
	print("Hello from go upload server")
	http.HandleFunc("/file", uploadFile)
	http.HandleFunc("/health", func(w http.ResponseWriter, _ *http.Request) {
		fmt.Fprintf(w, "OK")
	})
	http.ListenAndServe("172.0.0.1:8080", nil)
}
