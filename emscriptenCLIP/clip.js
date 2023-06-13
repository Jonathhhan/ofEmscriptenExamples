var model;
var tokenizer;

function loadCLIP() {
        var modelPath = 'data/clip-text-vit-32-tfjs/model.json';
        tf.loadGraphModel(modelPath).then(mod => {
        	tokenizer = new tok();
        	model = mod;
		console.log("Model loaded.");
	});	
}

function interference(prompt) {
	var textTokens = tokenizer.encodeForCLIP(prompt);
	textTokens = Int32Array.from(textTokens);
	var input = {'input': tf.tensor(textTokens, [1, 77], "int32")};
	model.executeAsync(input).then(results => {   
        	console.log(results.dataSync());
        	tf.dispose(results);
	});
}
