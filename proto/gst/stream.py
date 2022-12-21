import gi
gi.require_version('Gst', '1.0')
from gi.repository import Gst

# Initialize Gstreamer
Gst.init(None)

# Create pipeline and attach elements
pipeline = Gst.Pipeline.new(None)
source = Gst.ElementFactory.make("v4l2src", None)
encoder = Gst.ElementFactory.make("x264enc", None)
payloader = Gst.ElementFactory.make("rtph264pay", None)
sink = Gst.ElementFactory.make("udpsink", None)

pipeline.add(source, encoder, payloader, sink)
source.link(encoder)
encoder.link(payloader)
payloader.link(sink)

# Set properties of elements
source.set_property("device", "/dev/video0")
sink.set_property("host", "127.0.0.1")
sink.set_property("port", 5000)

# Set up audio stream
encoder = Gst.ElementFactory.make("opusenc", None)
payloader = Gst.ElementFactory.make("rtpopuspay", None)
sink = Gst.ElementFactory.make("udpsink", None)

pipeline.add(encoder, payloader, sink)

# Link audio elements
sink_pad = sink.get_static_pad("sink")
payloader_pad = payloader.get_static_pad("src")
payloader_pad.link(sink_pad)

# Set audio sink properties
sink.set_property("host", "127.0.0.1")
sink.set_property("port", 5001)

# Set up caps filter to ensure compatibility with payloader
caps = Gst.Caps.new_simple("audio/x-opus", None)
encoder.link_filtered(payloader, caps)

# Start streaming
pipeline.set_state(Gst.State.PLAYING)

# Run main loop
loop = Gst.MainLoop.new(None, False)
loop.run()

