#include <gst/gst.h>

int main(int argc, char *argv[]) {
  GstElement *pipeline, *source, *encoder, *payloader, *sink;
  GstCaps *caps;
  GstPad *pad;
  GstPadLinkReturn ret;

  /* Initialize Gstreamer */
  gst_init(&argc, &argv);

  /* Create pipeline and attach elements */
  pipeline = gst_pipeline_new("streaming-pipeline");
  source = gst_element_factory_make("v4l2src", "video-source");
  encoder = gst_element_factory_make("x264enc", "h264-encoder");
  payloader = gst_element_factory_make("rtph264pay", "h264-payloader");
  sink = gst_element_factory_make("udpsink", "udp-sink");

  gst_bin_add_many(GST_BIN(pipeline), source, encoder, payloader, sink, NULL);
  gst_element_link(source, encoder);
  gst_element_link(encoder, payloader);
  gst_element_link(payloader, sink);

  /* Set properties of elements */
  g_object_set(G_OBJECT(source), "device", "/dev/video0", NULL);
  g_object_set(G_OBJECT(sink), "host", "127.0.0.1", "port", 5000, NULL);

  /* Set up audio stream */
  encoder = gst_element_factory_make("opusenc", "opus-encoder");
  payloader = gst_element_factory_make("rtpopuspay", "opus-payloader");
  sink = gst_element_factory_make("udpsink", "udp-sink-audio");

  gst_bin_add_many(GST_BIN(pipeline), encoder, payloader, sink, NULL);

  /* Link audio elements */
  pad = gst_element_get_static_pad(payloader, "src");
  ret = gst_pad_link(pad, sink_pad);
  if (GST_PAD_LINK_FAILED(ret)) {
    g_error("Failed to link audio payloader to sink");
  }
  gst_object_unref(pad);

  /* Set audio sink properties */
  g_object_set(G_OBJECT(sink), "host", "127.0.0.1", "port", 5001, NULL);

  /* Set up caps filter to ensure compatibility with payloader */
  caps = gst_caps_new_simple("audio/x-opus", NULL);
  gst_element_link_filtered(encoder, payloader, caps);
  gst_caps_unref(caps);

  /* Start streaming */
  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  /* Run main

