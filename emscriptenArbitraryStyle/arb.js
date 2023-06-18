setupArbitraryStyle__deps = ['$GL'];
loadStyleImage__deps = ['$GL'];

var destinationCanvas = document.createElement('canvas');    
var fb1, fb2;

function setupArbitraryStyle (styleTextureID, contentTextureID, destinationTextureID, contentTextureWidth, contentTextureHeight) {
	fb1 = GLctx.createFramebuffer();
	fb2 = GLctx.createFramebuffer();
	tf.loadGraphModel('https://huggingface.co/Jona0123456789/arbitrary-style-tfjs/resolve/main/model.json').then( (model) => {
		arbitrayStyleModel = model;
		Module.arbitraryStyleModelLoaded();
		runArbitraryStyle (styleTextureID, contentTextureID, destinationTextureID, contentTextureWidth, contentTextureHeight);
	});
}
    
function runArbitraryStyle (styleTextureID, contentTextureID, destinationTextureID, contentTextureWidth, contentTextureHeight) {
	// console.log(`numBytesInGPUAllocated: ${tf.memory().numBytesInGPUAllocated}`);
	// console.log(`numBytesInGPUAllocated: ${tf.memory().numBytesInGPU}`);
	// console.log(`numTensors: ${tf.memory().numTensors}`);
	const w = contentTextureWidth, h = contentTextureHeight;
		
	const styleTexture = GL.textures[styleTextureID];
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, fb1);
	GLctx.framebufferTexture2D(GLctx.FRAMEBUFFER, GLctx.COLOR_ATTACHMENT0, GLctx.TEXTURE_2D, styleTexture, 0);
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, null);
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, fb1);
	const data1 = new Uint8Array(w * h * 4);
	const imageData1 = new ImageData(new Uint8ClampedArray(data1.buffer), w, h);
	GLctx.readPixels(0, 0, w, h, GLctx.RGBA, GLctx.UNSIGNED_BYTE, data1);
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, null);
		
	const contentTexture = GL.textures[contentTextureID];
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, fb2);
	GLctx.framebufferTexture2D(GLctx.FRAMEBUFFER, GLctx.COLOR_ATTACHMENT0, GLctx.TEXTURE_2D, contentTexture, 0);
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, null);
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, fb2);
	const data2 = new Uint8Array(w * h * 4);
	const imageData2 = new ImageData(new Uint8ClampedArray(data2.buffer), w, h);
	GLctx.readPixels(0, 0, w, h, GLctx.RGBA, GLctx.UNSIGNED_BYTE, data2);
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, null);
	
	tf.tidy(() =>{
		const VGG_MEAN = tf.tensor([103.939, 116.779, 123.68]);
		var inputs1 = tf.browser.fromPixels(imageData1).toFloat().expandDims();
		inputs1 = tf.reverse(inputs1, axis=[-1]);
		inputs1 = tf.sub(inputs1, VGG_MEAN);
		var inputs2 = tf.browser.fromPixels(imageData2).toFloat().expandDims();
		inputs2 = tf.reverse(inputs2, axis=[-1]);
		inputs2 = tf.sub(inputs2, VGG_MEAN);
		const alpha = tf.tensor2d([[1]]);;
	
		const features = arbitrayStyleModel.predict([inputs2, inputs1, alpha]);
		var stylized = features[3];
		stylized = stylized.clipByValue(0, 255);
		if (destinationCanvas.width != w || destinationCanvas.height != h) {
			destinationCanvas.width = w;
			destinationCanvas.height = h;
		}
		tf.browser.toPixels(stylized.toInt().squeeze(), destinationCanvas).then(()=>{
			GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[destinationTextureID]);
			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, destinationCanvas);
			GLctx.bindTexture(GLctx.TEXTURE_2D, null);
        	});
        });
}
