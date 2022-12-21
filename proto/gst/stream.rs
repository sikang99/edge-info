extern crate gstreamer as gst;
extern crate gstreamer_video as gst_video;

use gst::prelude::*;

fn main() {
    // Initialize Gstreamer
    gst::init().unwrap();

    // Create pipeline and attach elements
    let pipeline = gst::Pipeline::new(None);
    let source = gst::ElementFactory::make("v4l2src", None).unwrap();
    let encoder = gst::ElementFactory::make("x264enc", None).unwrap();
    let payloader = gst::ElementFactory::make("rtph264pay", None).unwrap();
    let sink = gst::ElementFactory::make("udpsink", None).unwrap();

    pipeline.add_many(&[&source, &encoder, &payloader, &sink]).unwrap();
    source.link(&encoder).unwrap();
    encoder.link(&payloader).unwrap();
    payloader.link(&sink).unwrap();

    // Set properties of elements
    source
        .set_property("device", &"/dev/video0")
        .expect("Failed to set video source device");
    sink.set_property("host", &"127.0.0.1")
        .expect("Failed to set sink host");
    sink.set_property("port", &5000i32)
        .expect("Failed to set sink port");

    // Set up audio stream
    let encoder = gst::ElementFactory::make("opusenc", None).unwrap();
    let payloader = gst::ElementFactory::make("rtpopuspay", None).unwrap();
    let sink = gst::ElementFactory::make("udpsink", None).unwrap();

    pipeline.add_many(&[&encoder, &payloader, &sink]).unwrap();

    // Link audio elements
    let sink_pad = sink.get_static_pad("sink").unwrap();
    let payloader_pad = payloader.get_static_pad("src").unwrap();
    payloader_pad
        .link(&sink_pad)
        .expect("Failed to link audio payloader to sink");

    // Set audio sink properties
    sink.set_property("host", &"127.0.0.1")
        .expect("Failed to set sink host");
    sink.set_property("port", &5001i32)
        .expect("Failed to set sink port");

    // Set up caps filter to ensure compatibility with payloader
    let caps = gst::Caps::new_simple("audio/x-opus", &[]);
    encoder
        .link_filtered(&payloader, &caps)
        .expect("Failed to link encoder and payloader with caps filter");

    // Start streaming
    pipeline
        .set_state(gst::State::

