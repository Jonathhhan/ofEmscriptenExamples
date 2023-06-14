interferenceText__deps = ['$GL'];

var textModel;
var imageModel;
var tokenizer;
var textTensor;
var imageTensor = [];
var dict = {};
const promises = [];

function loadTextCLIP() {
        var modelPath = 'data/clip-text-vit-f16-tfjs/model.json';
        tf.loadGraphModel(modelPath).then(mod => {
        	tokenizer = new tok();
        	textModel = mod;
		console.log("Text Model loaded.");
		// loadImageCLIP();
		fetch('data/imageTensors.json').then((response) => response.json()).then((json) => imageTensor = json);
	});	
}

function loadImageCLIP() {
        var modelPath = 'data/clip-image-vit-uint8-tfjs/model.json';
        tf.loadGraphModel(modelPath).then(mod => {
        	imageModel = mod;
		console.log("Image Model loaded.");
		interferenceImage();
	});	
}

function interferenceImage() {
	imageTensor = [];
	var content = FS.readdir("/data/images");
	const filesNames = content.filter(Dirent => Dirent.includes("JPEG") || Dirent.includes("jpg")).map(Dirent => Dirent);
	console.log(filesNames);
	let canvas2 = new OffscreenCanvas(224, 224);
	let ctx = canvas2.getContext("2d");
	for (let i = 0; i < filesNames.length; i++) {
		promises.push(new Promise((resolve, reject) => {
			const file = FS.readFile("/data/images/" + filesNames[i]);
			var blob = new Blob([file], {type: "text/plain;charset=utf-8"});
        		createImageBitmap(blob).then(img => {   
        			ctx.drawImage(img, 0, 0, canvas2.width, canvas2.height);
				let imageData = ctx.getImageData(0, 0, canvas2.width, canvas2.height);
				let rgbData = [[], [], []]; // [r, g, b]
				// remove alpha and put into correct shape:
				let d = imageData.data;
				for(let i = 0; i < d.length; i += 4) { 
					let x = (i/4) % canvas2.width;
					let y = Math.floor((i/4) / canvas2.width)
					if(!rgbData[0][y]) rgbData[0][y] = [];
					if(!rgbData[1][y]) rgbData[1][y] = [];
					if(!rgbData[2][y]) rgbData[2][y] = [];
					rgbData[0][y][x] = d[i+0]/255;
					rgbData[1][y][x] = d[i+1]/255;
					rgbData[2][y][x] = d[i+2]/255;
					// From CLIP repo: Normalize(mean=(0.48145466, 0.4578275, 0.40821073), std=(0.26862954, 0.26130258, 0.27577711))
					rgbData[0][y][x] = (rgbData[0][y][x] - 0.48145466) / 0.26862954;
					rgbData[1][y][x] = (rgbData[1][y][x] - 0.4578275) / 0.26130258;
					rgbData[2][y][x] = (rgbData[2][y][x] - 0.40821073) / 0.27577711;
				}
				rgbData = Float32Array.from(rgbData.flat().flat());
				var input = {'input': tf.tensor(rgbData, [1, 3, 224, 224], 'float32')};
				imageModel.executeAsync(input).then(results => {
					imageTensor.push(results.dataSync(), "/data/images/" + filesNames[i]);
					// tf.dispose(results);
        				resolve();
				});
			});
		}));
	}
	Promise.all(promises).then(() => {
		console.log("Image Tensors ready.");
		var dataStr = "data:text/json;charset=utf-8," + encodeURIComponent(JSON.stringify(imageTensor));
		var a = document.createElement('a');
		a.setAttribute("href", dataStr);
		a.setAttribute("download", "imageTensors.json");
		a.click();
	});
}

function interferenceText(prompt, textureId1, textureId2, textureId3, textureId4) {
	dict = {};
	var textTokens = tokenizer.encodeForCLIP(prompt);
	textTokens = Int32Array.from(textTokens);
	var input = {'input': tf.tensor(textTokens, [1, 77], "int32")};
	textModel.executeAsync(input).then(results => {
		textTensor = results;
        	// tf.dispose(results);
        	for (let i = 0; i < imageTensor.length / 2; i++) {
        		var array  = [];
			for (key in imageTensor[i * 2]){
				array.push(imageTensor[i * 2][key]);
			}
			cosineSimilarity(array, imageTensor[i * 2 + 1]);
		}
		var entries = Object.entries(dict);
		var sorted = entries.sort((a, b) => b[1] - a[1]);
		var file = FS.readFile(sorted[0][0]);
		var blob = new Blob([file], {type: "text/plain;charset=utf-8"});
		createImageBitmap(blob).then(img => {
			GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[textureId1]);
			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, img);
			GLctx.bindTexture(GLctx.TEXTURE_2D, null);
		});
		file = FS.readFile(sorted[2][0]);
		blob = new Blob([file], {type: "text/plain;charset=utf-8"});
		createImageBitmap(blob).then(img => {
	        	GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[textureId2]);
                	GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, img);
                	GLctx.bindTexture(GLctx.TEXTURE_2D, null);
		});
		file = FS.readFile(sorted[4][0]);
		blob = new Blob([file], {type: "text/plain;charset=utf-8"});
		createImageBitmap(blob).then(img => {
	        	GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[textureId3]);
                	GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, img);
                	GLctx.bindTexture(GLctx.TEXTURE_2D, null);
		});
		file = FS.readFile(sorted[6][0]);
		blob = new Blob([file], {type: "text/plain;charset=utf-8"});
        	createImageBitmap(blob).then(img => {
	        	GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[textureId4]);
                	GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, img);
                	GLctx.bindTexture(GLctx.TEXTURE_2D, null);
		});
	});
}

function cosineSimilarity(imageTensor, imageName) {
	var A = textTensor.dataSync();
	var B = imageTensor;
        if(A.length !== B.length) throw new Error("A.length !== B.length");
	var dotProduct = 0, mA = 0, mB = 0;
	for(let i = 0; i < A.length; i++){
		dotProduct += A[i] * B[i];
		mA += A[i] * A[i];
		mB += B[i] * B[i];
	}
        mA = Math.sqrt(mA);
        mB = Math.sqrt(mB);
        let similarity = dotProduct / (mA * mB);
	dict[imageName] = similarity;
}
