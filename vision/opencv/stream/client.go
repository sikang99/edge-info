package main

import (
	"fmt"
	"net"

	"github.com/google/draco/go/dracogen"
	"github.com/google/draco/go/dracoutils"
	"github.com/google/draco/go/gltf"
)

func main() {
	// Connect to the server.
	conn, err := net.Dial("tcp", "localhost:8000")
	if err != nil {
		fmt.Println(err)
		return
	}
	defer conn.Close()

	// Decompress the 3D model using Draco.
	decompressor, err := dracogen.NewDecompressor()
	if err != nil {
		fmt.Println(err)
		return
	}
	defer decompressor.Close()

	// Continuously receive and display the 3D model as part of a live stream.
	for {
		// Receive the encoded data from the server.
		encoded := make([]byte, 1024)
		_, err := conn.Read(encoded)
		if err != nil {
			fmt.Println(err)
			return
		}

		// Decompress the encoded data.
		doc, err := decompressor.Decompress(encoded)
		if err != nil {
			fmt.Println(err)
			return
		}

		// Display the GLTF model.
		if err := dracoutils.GLTFToWindow(doc, "3D Model"); err != nil {
			fmt.Println(err)
			return
		}
	}
}

