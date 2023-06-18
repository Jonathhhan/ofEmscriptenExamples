setupArbitraryStyle__deps = ['$GL'];
loadStyleImage__deps = ['$GL'];

var canvas2 = document.createElement('canvas');    
var fb1;
var fb2;

function setupArbitraryStyle (textureID1, textureID2) {
	tf.loadGraphModel('data/arbitrayStyleModel/model.json').then( (model) => {
		arbitrayStyleModel = model;
	});

	var texture1 = GL.textures[textureID1];
	fb1 = GLctx.createFramebuffer();
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, fb1);
	GLctx.framebufferTexture2D(GLctx.FRAMEBUFFER, GLctx.COLOR_ATTACHMENT0, GLctx.TEXTURE_2D, texture1, 0);
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, null);
			
	var texture2 = GL.textures[textureID2];
	fb2 = GLctx.createFramebuffer();
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, fb2);
	GLctx.framebufferTexture2D(GLctx.FRAMEBUFFER, GLctx.COLOR_ATTACHMENT0, GLctx.TEXTURE_2D, texture2, 0);
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, null);
}
    
function runArbitraryStyle (textureID3, textureWidth2, textureHeight2) {
	var w = textureWidth2, h = textureHeight2;

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
		var alpha = tf.tensor2d([[0.9]]);
		return arbitrayStyleModel.predict([inputs2, inputs1, alpha]);
	});
	let stylized = features[3];
	stylized = stylized.clipByValue(0, 255);
	if (canvas2.width != w || canvas2.height != h) {
		canvas2.width = w;
		canvas2.height = h;
	}
	tf.browser.toPixels(stylized.toInt().squeeze(), canvas2).then(()=>{
		GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[textureID3]);
		GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, canvas2);
		GLctx.bindTexture(GLctx.TEXTURE_2D, null);
        });
	inputs1.dispose();
	inputs2.dispose();
	VGG_MEAN.dispose();
}
