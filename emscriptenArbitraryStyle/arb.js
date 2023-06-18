setupArbitraryStyle__deps = ['$GL'];
loadStyleImage__deps = ['$GL'];

var destinationCanvas = document.createElement('canvas');    
var fb1, fb2;

function setupArbitraryStyle (styleTextureID, contentTextureID, destinationTextureID, contentTextureWidth, contentTextureHeight) {
	tf.loadGraphModel('https://huggingface.co/Jona0123456789/arbitrary-style-tfjs/resolve/main/model.json').then( (model) => {
		arbitrayStyleModel = model;
		Module.arbitraryStyleModelLoaded();
		runArbitraryStyle (styleTextureID, contentTextureID, destinationTextureID, contentTextureWidth, contentTextureHeight);
	});
}
    
function runArbitraryStyle (styleTextureID, contentTextureID, destinationTextureID, contentTextureWidth, contentTextureHeight) {
	var styleTexture = GL.textures[styleTextureID];
	fb1 = GLctx.createFramebuffer();
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, fb1);
	GLctx.framebufferTexture2D(GLctx.FRAMEBUFFER, GLctx.COLOR_ATTACHMENT0, GLctx.TEXTURE_2D, styleTexture, 0);
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, null);
			
	var contentTexture = GL.textures[contentTextureID];
	fb2 = GLctx.createFramebuffer();
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, fb2);
	GLctx.framebufferTexture2D(GLctx.FRAMEBUFFER, GLctx.COLOR_ATTACHMENT0, GLctx.TEXTURE_2D, contentTexture, 0);
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, null);
	
	var w = contentTextureWidth, h = contentTextureHeight;

	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, fb1);
	var data = new Uint8Array(w * h * 4);
	GLctx.readPixels(0, 0, w, h, GLctx.RGBA, GLctx.UNSIGNED_BYTE, data);
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, null);
	var imageData = new ImageData(new Uint8ClampedArray(data.buffer), w, h);
	const VGG_MEAN = tf.tensor([103.939, 116.779, 123.68]);
	var inputs1 = tf.browser.fromPixels(imageData).toFloat().expandDims();
	inputs1 = tf.reverse(inputs1, axis=[-1]);
	inputs1 = tf.sub(inputs1, VGG_MEAN);
	
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, fb2);
    	var data2 = new Uint8Array(w * h * 4);
	GLctx.readPixels(0, 0, w, h, GLctx.RGBA, GLctx.UNSIGNED_BYTE, data2);
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, null);
	var imageData2 = new ImageData(new Uint8ClampedArray(data2.buffer), w, h);
	var inputs2 = tf.browser.fromPixels(imageData2).toFloat().expandDims();
	inputs2 = tf.reverse(inputs2, axis=[-1]);
	inputs2 = tf.sub(inputs2, VGG_MEAN);
	
	const features = tf.tidy(() =>{
		var alpha = tf.tensor2d([[1]]);
		return arbitrayStyleModel.predict([inputs2, inputs1, alpha]);
	});
	let stylized = features[3];
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
	inputs1.dispose();
	inputs2.dispose();
	VGG_MEAN.dispose();
}
