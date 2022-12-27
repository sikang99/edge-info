extern crate gstreamer as gst;
extern crate websocket as ws;

use gst::prelude::*;
use std::error::Error;
use websocket::client::sync::Client;
use websocket::{Message, OwnedMessage};

fn main() -> Result<(), Box<dyn Error>> {
    // Initialize GStreamer
    gst::init()?;

    // Create a new pipeline using a pipeline string
    let pipeline = gst::parse_launch("v4l2src ! video/x-h264,width=1280,height=720 ! h264parse ! websocketclientsink")?;

    // Set the pipeline to the PLAYING state
    pipeline.set_state(gst::State::Playing)?;

    // Create a websocket client
    let mut client = Client::new("ws://localhost:9000")?;

    // Connect to the server
    client.connect()?;

    // Send messages to the server
    let mut count = 0;
    loop {
        let message = Message::binary(format!("{}", count));
        client.send_message(&message)?;
        count += 1;
        std::thread::sleep(std::time::Duration::from_millis(1000));
    }

    // Set the pipeline back to the NULL state
    pipeline.set_state(gst::State::Null)?;

    Ok(())
}

