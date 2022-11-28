## TFLite : TensorFlow Lite


### Articles
- 2022/09/14 [Co-simulating ML with Springbok using Renode](https://opensource.googleblog.com/2022/09/co-simulating-ml-with-springbok-using-renode.html)
- 2022/06/28 [“TensorFlow Lite for Microcontrollers (TFLM): Recent Developments,” a Presentation from BDTI and Google](https://www.edge-ai-vision.com/2022/06/tensorflow-lite-for-microcontrollers-tflm-recent-developments-a-presentation-from-bdti-and-google/)
- 2022/05/10 [TensorFlow Lite Model Maker: Create Models for On-Device Machine Learning](https://learnopencv.com/tensorflow-lite-model-maker-create-models-for-on-device-machine-learning/)
- 2022/05/03 [TensorFlow Lite: Model Optimization for On-Device Machine Learning](https://learnopencv.com/tensorflow-lite-model-optimization-for-on-device-machine-learning/)
- 2022/04/24 [Get started with Tensorflow lite/micro by Sony Spresense](https://www.hackster.io/taroyoshino007/get-started-with-tensorflow-lite-micro-by-sony-spresense-e92bf1)
- 2022/02/23 [Using the Picamera2 library with TensorFlow Lite](https://www.raspberrypi.com/news/using-the-picamera2-library-with-tensorflow-lite/)
- 2022/01/02 [TFLite 와 TensorRT 간단 비교](https://wooono.tistory.com/407)
- 2021/11/22 [On-Device Deep Learning: PyTorch Mobile and TensorFlow Lite](https://www.kdnuggets.com/2021/11/on-device-deep-learning-pytorch-mobile-tensorflow-lite.html)
- 2020/07/18 [Tensorflow Lite(TFLite) with Golang](https://medium.com/analytics-vidhya/tensorflow-lite-tflite-with-golang-37a326c089ff)
- 2020/01/07 [Loading a TensorFlow-Lite model in Python with Custom Operators](https://medium.com/@bsramasubramanian/running-a-tensorflow-lite-model-in-python-with-custom-ops-9b2b46efd355)


### Information
- [A Quick / Short Example of using TFLite with Golang and a GANs Model](https://derekg.github.io/tflite.html)
- Primo AI: [Tensorflow Lite](http://primo.ai/index.php?title=TensorFlow_Lite)
- [TensorFlow Lite for Go](https://tflitego.nicolasbortolotti.com/)
- [AI Inference for Real-time Data Streams with WasmEdge and YoMo](https://www.secondstate.io/articles/yomo-wasmedge-real-time-data-streams/)
- [TensorFlow Hub]https://tfhub.dev/)


### Open Source (Golang)
- [derekg/tflite-golang-gan-example](https://github.com/derekg/tflite-golang-gan-example) - 
- [mattn/go-tflite](https://github.com/mattn/go-tflite) 
- [nbortolotti/tflitego](https://github.com/nbortolotti/tflitego) - tflitego provide a simple and clear solution to use TensorFlow lite in Go. Our objective is provide a cohesive API, simplicity related to TensorFlow Lite C API connection and maintainability.
- [nbortolotti/tflitego_examples](https://github.com/nbortolotti/tflitego_examples) - 
- [galeone/tfgo](https://github.com/galeone/tfgo)
- [https://github.com/mpromonet/gin-tflite](https://github.com/mpromonet/gin-tflite)


### Open Source (Python)
- [k5iogura/tensorflowlite-samples](https://github.com/k5iogura/tensorflowlite-samples)
- [iwatake2222/CNN_NumberDetector](https://github.com/iwatake2222/CNN_NumberDetector) 
- [cfzd/Ultra-Fast-Lane-Detection](https://github.com/cfzd/Ultra-Fast-Lane-Detection) - Ultra Fast Structure-aware Deep Lane Detection (ECCV 2020)

### Papers
- 2021 [tion Generative Adversarial Network (SRGANs) for Wheat Stripe Rust Classification](https://pubmed.ncbi.nlm.nih.gov/34883905/)




### Building & Installing
- Tensorflow on Apple ARM Macs
```
bazel build --config opt --cpu=darwin_arm64 --host_cpu=darwin_arm64 //tensorflow/tools/lib_package:libtensorflow
```

```
cd github.com/tensorflow/tensorflow
cmake lite/c 
make 
```
