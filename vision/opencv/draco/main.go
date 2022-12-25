package main

import (
	"fmt"

	"gocv.io/x/gocv"
)

func main() {
	// Load the input file as an image.
	img := gocv.IMRead("model.drc", gocv.IMReadUnchanged)
	if img.Empty() {
		fmt.Println("Failed to load image")
		return
	}
	defer img.Close()

	// Decode the 3D model from the image.
	mesh, err := img.ToMesh()
	if err != nil {
		fmt.Println(err)
		return
	}

	// Print some information about the mesh.
	fmt.Printf("Number of vertices: %d\n", mesh.NumVertices())
	fmt.Printf("Number of faces: %d\n", mesh.NumFaces())
}

