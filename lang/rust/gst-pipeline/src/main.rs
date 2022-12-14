extern crate gstreamer as gst;
use gst::prelude::*;

fn main() {
    // Initialize GStreamer
    gst::init().unwrap();

    // Create a new pipeline using a pipeline string
    let pipeline = gst::parse_launch("videotestsrc ! autovideosink").unwrap();

    // Set the pipeline to the PLAYING state
    pipeline.set_state(gst::State::Playing).unwrap();

    // Wait for the pipeline to complete
    let bus = pipeline.bus().unwrap();
    while let Some(msg) = bus.timed_pop(gst::ClockTime::from_seconds(10)) {
        use gst::MessageView;
        match msg.view() {
            MessageView::Eos(..) => break,
            MessageView::Error(err) => {
                println!("Error: {:?}", err.error() );
                break;
            }
            _ => (),
        }
    }

    // Set the pipeline back to the NULL state
    pipeline.set_state(gst::State::Null).unwrap();
}

