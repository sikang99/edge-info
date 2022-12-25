package main

// #cgo pkg-config: draco
import "C"

import (
	"fmt"
	"os"

	"github.com/qmuntal/gltf"
	"github.com/qmuntal/draco-go/draco"
)

func main() {
	// Open the input file.
	f, err := os.Open("model.gltf")
	if err != nil {
		fmt.Println(err)
		return
	}
	defer f.Close()

	// Decode the GLTF file.
	gltf, err := gltf.Decode(f)
	if err != nil {
		fmt.Println(err)
		return
	}

	// Load the Draco extension.
	err = draco.Load()
	if err != nil {
		fmt.Println(err)
		return
	}

	// Find the first mesh in the GLTF file.
	var mesh *gltf.Mesh
	for _, m := range gltf.Meshes {
		mesh = m
		break
	}
	if mesh == nil {
		fmt.Println("No meshes found")
		return
	}

	// Print some information about the mesh.
	fmt.Printf("Number of vertices: %d\n", mesh.NumVertices())
	fmt.Printf("Number of faces: %d\n", mesh.NumFaces())
}

