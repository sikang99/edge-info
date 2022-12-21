package main

import (
	"fmt"

	"github.com/ziutek/gst"
)

func main() {
	// Initialize Gstreamer
	gst.Init(nil)

	// Create pipeline and attach elements
	pipeline := gst.NewPipeline("streaming-pipeline")
	source := gst.ElementFactoryMake("v4l2src", "video-source")
	encoder := gst.ElementFactoryMake("x264enc", "h264-encoder")
	payloader := gst.ElementFactoryMake("rtph264pay", "h264-payloader")
	sink := gst.ElementFactoryMake("udpsink", "udp-sink")

	pipeline.Add(source, encoder, payloader, sink)
	source.Link(encoder)
	encoder.Link(payloader)
	payloader.Link(sink)

	// Set properties of elements
	source.SetProperty("device", "/dev/video0")
	sink.SetProperty("host", "127.0.0.1")
	sink.SetProperty("port", 5000)

	// Set up audio stream
	encoder = gst.ElementFactoryMake("opusenc", "opus-encoder")
	payloader = gst.ElementFactoryMake("rtpopuspay", "opus-payloader")
	sink = gst.ElementFactoryMake("udpsink", "udp-sink-audio")

	pipeline.Add(encoder, payloader, sink)

	// Link audio elements
	sinkPad := sink.GetStaticPad("sink")
	payloaderPad := payloader.GetStaticPad("src")
	err := payloaderPad.Link(sinkPad)
	if err != nil {
		fmt.Errorf("Failed to link audio payloader to sink: %v", err)
	}

	// Set audio sink properties
	sink.SetProperty("host", "127.0.0.1")
	sink.SetProperty("port", 5001)

	// Set up caps filter to ensure compatibility with payloader
	caps := gst.NewCaps("audio/x-opus")
	encoder.LinkFiltered(payloader, caps)

	// Start streaming
	pipeline.SetState(gst.StatePlaying)

	// Run main loop
	bus := pipeline.GetBus()
	for {
		msg := bus.TimedPopFiltered(gst.CLOCK_TIME_NONE, gst.MessageError|gst.MessageEos)
		switch msg.Type {
		case gst.MessageError:
			err, debug := msg.ParseError()
			fmt.Printf("Error: %v (%v)\n", err, debug)
			return
		case gst.MessageEos:
			return
		}
		msg.Unref()
	}
}

