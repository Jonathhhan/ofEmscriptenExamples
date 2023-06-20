plotSpectra__deps = ['$GL'];

var ganSynth;
var destinationCanvas = document.createElement('canvas');

function startGanSynth() {
	const checkpointURL = "https://storage.googleapis.com/magentadata/js/checkpoints/gansynth/acoustic_only";
	// const checkpointURL = "https://storage.googleapis.com/magentadata/js/checkpoints/gansynth/all_instruments";
	const ganSynthModel = new gansynth.GANSynth(checkpointURL);
	ganSynthModel.initialize().then(() => {
		ganSynth = ganSynthModel;
		Module.ganSynthModelLoaded();
	});
}

function sampleGanNote(destinationTextureID1, destinationTextureID2) {
	const lengthInSeconds = 4.0;
	const sampleRate = AUDIO.context.sampleRate;
	const length = lengthInSeconds * sampleRate;
	const specgrams = ganSynth.randomSample(48);
	ganSynth.specgramsToAudio(specgrams).then((audio) => {
		const audioBuffer = AUDIO.context.createBuffer(1, length, sampleRate);
		audioBuffer.copyToChannel(audio, 0, 0);
		const source = AUDIO.context.createBufferSource();
		source.buffer = audioBuffer;
		source.connect(AUDIO.fft);
		source.start();
	});
	plotSpectra(specgrams, destinationTextureID1, destinationTextureID2);
}

function plotSpectra(spectra, destinationTextureID1, destinationTextureID2) {
	const spectraPlot1 = tf.tidy(() => {
		let spectraPlot = tf.slice(spectra, [0, 0, 0, 0], [1, -1, -1, 1]).reshape([128, 1024]);
		spectraPlot = tf.sub(spectraPlot, tf.min(spectraPlot));
		spectraPlot = tf.div(spectraPlot, tf.max(spectraPlot));
		return spectraPlot;
	});
	const spectraPlot2 = tf.tidy(() => {
		let spectraPlot = tf.slice(spectra, [0, 0, 0, 1], [1, -1, -1, 1]).reshape([128, 1024]);
		spectraPlot = tf.sub(spectraPlot, tf.min(spectraPlot));
		spectraPlot = tf.div(spectraPlot, tf.max(spectraPlot));
		return spectraPlot;
	});
	tf.browser.toPixels(spectraPlot1, destinationCanvas).then(()=>{
		GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[destinationTextureID1]);
		GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, destinationCanvas);
		GLctx.bindTexture(GLctx.TEXTURE_2D, null);
		spectraPlot1.dispose();
	});
	tf.browser.toPixels(spectraPlot2, destinationCanvas).then(()=>{
		GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[destinationTextureID2]);
		GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, destinationCanvas);
		GLctx.bindTexture(GLctx.TEXTURE_2D, null);
		spectraPlot2.dispose();
	});
}
