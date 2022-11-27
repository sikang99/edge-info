package main
import (
	rti "github.com/rticommunity/rticonnextdds-connector-go"
	"log"
)

func main() {
	connector, err := rti.NewConnector("MyParticipantLibrary::Zero", "./ShapeExample.xml")
	if err != nil {
		log.Panic(err)
	}
	defer connector.Delete()
	input, err := connector.GetInput("MySubscriber::MySquareReader")
	if err != nil {
		log.Panic(err)
	}

	for {
		connector.Wait(-1)
		input.Take()
		numOfSamples, _ := input.Samples.GetLength()
		for j := 0; j < numOfSamples; j++ {
			valid, _ := input.Infos.IsValid(j)
			if valid {
				json, err := input.Samples.GetJSON(j)
				if err != nil {
					log.Println(err)
				} else {
					log.Println(string(json))
				}
			}
		}
	}
}
