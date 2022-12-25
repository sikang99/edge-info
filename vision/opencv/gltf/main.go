package main

import (
	"fmt"
	"gocv.io/x/gocv"
)

func main() {
	// Load the GLTF model
	model, err := gocv.IMRead3D("model.gltf")
	if err != nil {
		fmt.Println(err)
		return
	}
	defer model.Close()

	// Display the model
	window := gocv.NewWindow("Model")
	defer window.Close()

	for {
		window.IMShow(model)
		if window.WaitKey(1) >= 0 {
			break
		}
	}
}
