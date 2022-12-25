package main

import (
	"fmt"
	"net"
	"time"

	"github.com/google/draco/go/dracogen"
	"github.com/google/draco/go/dracoutils"
	"github.com/google/draco/go/gltf"

	"gocv.io/x/gocv"
)

func main() {
	// Bind to a local port.
	ln, err := net.Listen("tcp", ":8000")
	if err != nil {
		fmt.Println(err)
		return
	}
	defer ln.Close()

	// Accept incoming connections.
	for {
		conn, err := ln.Accept()
		if err != nil {
			fmt.Println(err)
			continue
		}
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	// Open the video capture device.
	webcam, err := gocv.VideoCaptureDevice(0)
	if err != nil {
		fmt.Println(err)
		return
	}
	defer webcam.Close()

	// Compress the 3D model using Draco.
	compressor, err := dracogen.NewCompressor()
	if err != nil {
		fmt.Println(err)
		return
	}
	defer compressor.Close()

	// Continuously transmit the 3D model as part of a live stream.
	ticker := time.NewTicker(time.Second)
	for range ticker.C {
		// Read a frame from the video capture device.
		mat := gocv.NewMat()
		if ok := webcam.Read(&mat); !ok {
			fmt.Println("error reading webcam")
			return
		}
		if mat.Empty() {
			continue
		}

		// Convert the frame to a GLTF model.
		doc, err := dracoutils.MatToGLTF(mat)
		if err != nil {
			fmt.Println(err)
			return
		}

		// Compress the GLTF model using Draco.
		encoded, err := compressor.Compress(doc)
		if err != nil {
			fmt.Println(err)
			return
		}

		// Send the encoded data to the client.
		_, err = conn.Write(encoded)
		if err != nil {
			fmt.Println(err)
			return
		}
	}
}

