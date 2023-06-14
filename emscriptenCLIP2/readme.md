This is how I converted the models:

tensorflowjs_converter --input_format tf_saved_model clip-text-vit-32-tf clip-text-vit-f16-tfjs --quantize_float16

tensorflowjs_converter --input_format tf_saved_model clip-image-vit-32-tf clip-image-vit-uint8-tfjs --quantize_uint8

https://github.com/josephrocca/openai-clip-js
