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
sink = Gst.ElementFactory.make("webtransportsink", None)

pipeline.add(source, encoder, payloader, sink)
source.link(encoder)
encoder.link(payloader)
payloader.link(sink)

# Set properties of elements
source.set_property("device", "/dev/video0")
sink.set_property("location

