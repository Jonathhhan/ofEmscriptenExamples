var LibraryHTML5Audio = {
    $AUDIO: {
        contexts: [],
        players: [],
        ffts: [],
        lastContextID: 0,

        soundBuffers: [],
        soundSources: [],
        soundStartTimes: [],
        soundGains: [],
        lastSoundID: 0,

        streams: [],
        mediaElements: [],
        lastStreamID: 0,

        soundPosition: function (sound_id) {
            var source = AUDIO.players[sound_id];
            if (source != undefined) {
                var context = source.context;
                var playTime = context.currentTime - source.startTime;
                var duration = AUDIO.players[sound_id].duration / AUDIO.players[sound_id].playbackRate.value;
                return Math.min(duration, playTime);
            } else {
                return 0;
            }
        }
    },

    html5audio_context_create: function () {
        try {
            // Fix up for prefixing
            window.AudioContext = window.AudioContext || window.webkitAudioContext;
            var context = new AudioContext({ sampleRate: 44100 });

            // Fix issue with chrome autoplay policy
            document.addEventListener('mousedown', function cb(event) {
                context.resume();
                event.currentTarget.removeEventListener(event.type, cb);
            });

            var id = AUDIO.lastContextID++;
            AUDIO.contexts[id] = context;
            
            var fft = context.createAnalyser();
            fft.smoothingTimeConstant = 0;
            fft.connect(AUDIO.contexts[id].destination);
            fft.maxDecibels = 0;
            fft.minDecibels = -100;
            AUDIO.ffts[id] = fft;
            return id;
        } catch (e) {
            console.log('Web Audio API is not supported in this browser', e);
            return -1;
        }
    },

    html5audio_context_start: function (context_id) {
        AUDIO.contexts[context_id].resume();
    },

    html5audio_context_stop: function (context_id) {
        AUDIO.contexts[context_id].suspend();
    },

    html5audio_context_spectrum: function (context_id, bands, spectrum) {
        AUDIO.ffts[context_id].fftSize = bands * 2;
        var spectrumArray = Module.HEAPF32.subarray(spectrum >> 2, (spectrum >> 2) + bands);
        AUDIO.ffts[context_id].getFloatFrequencyData(spectrumArray);
    },

    html5audio_context_samplerate: function (context_id) {
        return AUDIO.contexts[context_id].sampleRate.value;
    },

    html5audio_sound_load: function (context_id, url) {
	var audio = document.createElement('audio');
	var id = AUDIO.lastSoundID++;
	AUDIO.players[id] = audio;
	AUDIO.players[id].src = UTF8ToString(url);
	AUDIO.soundSources[id] = AUDIO.contexts[context_id].createMediaElementSource(AUDIO.players[id]);
	AUDIO.soundSources[id].connect(audioWorklet);
	return id;
    },

    html5audio_sound_play: function (context_id, sound_id, offset) {
        AUDIO.players[sound_id].play(offset);
    },

    html5audio_sound_stop: function (sound_id) {
        AUDIO.players[sound_id].currentTime = 0;
        AUDIO.players[sound_id].pause();
    },

    html5audio_sound_pause: function (sound_id) {
        AUDIO.players[sound_id].pause();
    },

    html5audio_sound_rate: function (sound_id) {
	return AUDIO.players[sound_id].playbackRate;
    },

    html5audio_sound_set_rate: function (sound_id, rate) {
	AUDIO.players[sound_id].playbackRate = rate;
    },

    html5audio_sound_done: function (sound_id) {
	return AUDIO.players[sound_id].done;
    },

    html5audio_sound_duration: function (sound_id) {
	return AUDIO.players[sound_id].duration;
    },

    html5audio_sound_position: function (sound_id) {
        return AUDIO.players[sound_id].currentTime;
    },

    html5audio_sound_set_loop: function (sound_id, loop) {
        AUDIO.players[sound_id].loop = true;
    },

    html5audio_sound_set_gain: function (sound_id, gain) {
        var gain = 1;
        AUDIO.players[sound_id].volume = gain;
    },

    html5audio_sound_gain: function (sound_id) {
        return AUDIO.players[sound_id].volume;
    },

    html5audio_sound_free: function (sound_id) {
	if(AUDIO.players[sound_id] != undefined){
	AUDIO.players[sound_id].pause();
	URL.revokeObjectURL(AUDIO.players[sound_id].src);
	}
    },

    html5audio_stream_create: function (context_id, bufferSize, inputChannels, outputChannels, inbuffer, outbuffer, callback, userData, pthreadPtr) {
        audioWorklet;
        var id = AUDIO.lastStreamID++;
        out("Buffer size: " + bufferSize);
        PThread.initAudioWorkletPThread(AUDIO.contexts[context_id], pthreadPtr).then(function () {
            out("Audio worklet PThread context initialized!")
        }, function (err) {
            out("Audio worklet PThread context initialization failed: " + [err, err.stack])
        });
        PThread.createAudioWorkletNode(AUDIO.contexts[context_id], "native-passthrough-processor", {
            numberOfInputs: 1,
            numberOfOutputs: 1,
            inputChannelCount: [2],
            outputChannelCount: [2],
            processorOptions: {
                inputChannels: inputChannels,
                outputChannels: outputChannels,
                inbuffer: inbuffer,
                outbuffer: outbuffer,
                bufferSize: bufferSize,
                callback: callback,
                userData: userData
            }
        }).then(function (workletNode) {
            audioWorklet = workletNode;
            if (typeof video != "undefined") {
                var source = AUDIO.contexts[context_id].createMediaElementSource(video);
                source.connect(audioWorklet);
            }
            out("Audio worklet node created! Tap/click on the window if you don't hear audio!");
            workletNode.connect(AUDIO.ffts[context_id]);
            if (inputChannels > 0) {
                navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia;
                if (navigator.getUserMedia) {
                    navigator.getUserMedia(
                        { audio: true },
                        function (audioIn) {
                            var mediaElement = AUDIO.contexts[context_id].createMediaStreamSource(audioIn);
                            mediaElement.connect(audioWorklet);
                            AUDIO.mediaElements[id] = mediaElement;
                        },
                        function (error) {
                            console.log("error creating audio in", error);
                        }
                    );
                }
            }
        }, function (err) {
            out("Audio worklet node creation failed: " + [err, err.stack])
        })
        return id;
    },

    html5audio_stream_free: function (stream_id) {
        return AUDIO.streams[stream_id] = null;
        return AUDIO.mediaElements[stream_id] = null;
    },

    html5audio_sound_is_loaded: function (sound) {
        if (audio.src != undefined) {
            return true;
        }
        return false;
    }
}


autoAddDeps(LibraryHTML5Audio, '$AUDIO');
mergeInto(LibraryManager.library, LibraryHTML5Audio);
