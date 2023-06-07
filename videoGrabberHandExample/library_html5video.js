var LibraryHTML5Video = {
    $VIDEO: {
        players: [],
        playersCounter: 0,

        getNewPlayerId: function() {
          var ret = VIDEO.playersCounter++;
          return ret;
        },

        grabbers: [],
        grabbersContexts: [],
        grabbersCounter: 0,

        getNewGrabberId: function() {
          var ret = VIDEO.grabbersCounter++;
          return ret;
        },

        getUserMedia: function(){
        	return navigator.mediaDevices.getUserMedia ||
        	    navigator.mediaDevices.webkitGetUserMedia ||
        	    navigator.mediaDevices.mozGetUserMedia ||
        	    navigator.mediaDevices.msGetUserMedia;
        },

        update: function(updatePixels, video, context, dstPixels){
        	if((updatePixels || video.pixelFormat!="RGBA") && video.width!=0 && video.height!=0 && dstPixels!=0){
        		try {
	            	context.drawImage( video, 0, 0, video.width, video.height );
	            	if (video.pixelFormat=="RGBA"){
		            	array.set(imageData.data);
	            	} else if (video.pixelFormat=="RGB" && typeof detector != 'undefined') {
	            		var ANCHOR_POINTS = [[0, 0, 0], [0, 0.1, 0], [-0.1, 0, 0], [-0.1, -0.1, 0]];
				var fingerLookupIndices = {
				thumb: [0, 1, 2, 3, 4],
				indexFinger: [0, 5, 6, 7, 8],
				middleFinger: [0, 9, 10, 11, 12],
				ringFinger: [0, 13, 14, 15, 16],
				pinky: [0, 17, 18, 19, 20],
				}; // for rendering each finger as a polyline
				var connections = [
				[0, 1], [1, 2], [2, 3], [3,4],
				[0, 5], [5, 6], [6, 7], [7, 8],
				[0, 9], [9, 10], [10, 11], [11, 12],
				[0, 13], [13,14], [14, 15], [15, 16],
				[0, 17], [17, 18],[18, 19], [19,20]
				];
				detector.estimateHands(video).then(hands => {
					for (let i = 0; i < hands.length; ++i) {
						drawResult(hands[i]);
					}
					function drawResult(hand) {
						if (hand.keypoints != null) {
							drawKeypoints(hand.keypoints, hand.handedness);
						}
					}
					function drawKeypoints(keypoints, handedness) {
						var keypointsArray = keypoints;
						context.fillStyle = handedness === 'Left' ? 'Red' : 'Blue';
						context.strokeStyle = 'White';
						context.lineWidth = 2;
						for (let i = 0; i < keypointsArray.length; i++) {
							var y = keypointsArray[i].x;
							var x = keypointsArray[i].y;
							drawPoint(x - 2, y - 2, 6);
						}
						var fingers = Object.keys(fingerLookupIndices);
						for (let i = 0; i < fingers.length; i++) {
							var finger = fingers[i];
							var points = fingerLookupIndices[finger].map(idx => keypoints[idx]);
      							drawPath(points, false);
						}
					}
					function drawPath(points, closePath) {
						var region = new Path2D();
						region.moveTo(points[0].x, points[0].y);
						for (let i = 1; i < points.length; i++) {
							var point = points[i];
							region.lineTo(point.x, point.y);
						}
						if (closePath) {
							region.closePath();
						}
						context.stroke(region);
					}
					function drawPoint(y, x, r) {
						context.beginPath();
						context.arc(x, y, r, 0, 2 * Math.PI);
						context.fill();
					}
					array = Module.HEAPU8.subarray(dstPixels, dstPixels+(video.width*video.height*3));
  					var imageData2 = context.getImageData(0,0,video.width,video.height);	
		            		for(var i=0, j=0; i<array.length; ){
		            			array[i++] = imageData2.data[j++];
		            			array[i++] = imageData2.data[j++];
		            			array[i++] = imageData2.data[j++];
		            			++j;
		            		}	
					GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[video.textureId]);
                			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGB, video.width, video.height, 0, GLctx.RGB, GLctx.UNSIGNED_BYTE, imageData2);
                			GLctx.bindTexture(GLctx.TEXTURE_2D, null);
				});
	            	} else if (video.pixelFormat=="GRAY") {
		            	array = Module.HEAPU8.subarray(dstPixels, dstPixels+(video.width*video.height));
		            	for(var i=0, j=0; i<array.length; ){
		            		array[i++] = (((srcPixels[j++]|0) << 1) + ((srcPixels[j]|0) << 2) + (srcPixels[j++]|0) + (srcPixels[j++]|0)) >> 3;
		            		++j;
		            	}

		                GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[video.textureId]);
		                GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.LUMINANCE, video.width, video.height, 0, GLctx.LUMINANCE, GLctx.UNSIGNED_BYTE, array);
		                GLctx.bindTexture(GLctx.TEXTURE_2D, null);
	            	}
        		}catch(e){console.log(e);}
        	}
        	if (video.pixelFormat=="RGBA"){
                GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[video.textureId]);
                GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, video);
                GLctx.bindTexture(GLctx.TEXTURE_2D, null);
        	}
        }
    },
    html5video_list_devices: function(){
        if (!navigator.mediaDevices.enumerateDevices) {
            console.log("enumerateDevices() not supported.");
        } else {
            // List cameras and microphones.
            navigator.mediaDevices.enumerateDevices()
            .then((devices) => {
                devices.forEach((device) => {
                    if(device.kind == "videoinput"){
                        console.log(`${device.kind}: ${device.label} id = ${device.deviceId}`);
                    }
                });
            })
            .catch((err) => {
                console.error(`${err.name}: ${err.message}`);
            });
        }
    },

   html5video_player_create: function(){	
        var video = document.createElement('video');
        video.loop = true;
        video.pixelFormat = "RGBA";
	var player_id = VIDEO.getNewPlayerId();
	VIDEO.players[player_id] = video;
	var source = AUDIO.context.createMediaElementSource(VIDEO.players[player_id]); 
	VIDEO.players[player_id].soundPan = AUDIO.context.createStereoPanner();
	source.connect(VIDEO.players[player_id].soundPan).connect(AUDIO.fft);
	video.onloadedmetadata = function (e){
        	VIDEO.players[player_id].width = this.videoWidth;
        	VIDEO.players[player_id].height = this.videoHeight;
		var videoImage = document.createElement( 'canvas' );
		videoImage.width = this.videoWidth;
		videoImage.height = this.videoHeight;
		var videoImageContext = videoImage.getContext( '2d' );
		// background color if no video present
		videoImageContext.fillStyle = '#000000';
		videoImageContext.fillRect( 0, 0, videoImage.width, videoImage.height );
		VIDEO.players[player_id].playersContext = videoImageContext;
        };
        return player_id;
    },

    html5video_player_delete: function(id){
    	VIDEO.players[id] = null;
    },
    
    html5video_player_load__deps: ['$GL'],
    html5video_player_load: function(id,src){
        VIDEO.players[id].src = UTF8ToString(src);
        var texId = GL.getNewId(GL.textures);
        var texture = GLctx.createTexture();
        texture.name = texId;
        GL.textures[texId] = texture;
        GLctx.bindTexture(GLctx.TEXTURE_2D, texture);
        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_MAG_FILTER, GLctx.LINEAR);
        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_MIN_FILTER, GLctx.LINEAR);
        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_WRAP_S, GLctx.CLAMP_TO_EDGE);
        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_WRAP_T, GLctx.CLAMP_TO_EDGE);
        VIDEO.players[id].textureId = texId;
    },

    html5video_player_pixel_format: function(id){
        var string = VIDEO.players[id].pixelFormat;
        var size = lengthBytesUTF8(string) + 1;
        var stringPointer = stackAlloc(size);
        stringToUTF8Array(string, HEAP8, stringPointer, size);
        return stringPointer;
    },

    html5video_player_set_pixel_format: function(id, format){
        VIDEO.players[id].pixelFormat = UTF8ToString(format);
    },

    html5video_player_update__deps: ['$GL'],
    html5video_player_update: function(id,update_pixels,pixels){
        var player = VIDEO.players[id];
        var array;
        var imageData;
        var data;
        if ( player.readyState === player.HAVE_ENOUGH_DATA ) {
        	VIDEO.update(update_pixels, player, VIDEO.players[id].playersContext, pixels);
            return true;
        }else{
        	return false;
        }
    },

    html5video_player_texture_id: function(id){
        return VIDEO.players[id].textureId;
    },

    html5video_player_width: function(id){
        return VIDEO.players[id].width;
    },

    html5video_player_height: function(id){
        return VIDEO.players[id].height;
    },

    html5video_player_play: function(id){
        VIDEO.players[id].play();
    },

    html5video_player_pause: function(id){
        VIDEO.players[id].pause();
    },

    html5video_player_stop: function(id){
        VIDEO.players[id].pause();
    },

    html5video_player_is_paused: function(id){
        return VIDEO.players[id].paused;
    },

    html5video_player_ready_state: function(id){
        return VIDEO.players[id].readyState;
    },

    html5video_player_duration: function(id){
    if (VIDEO.players[id].duration >= 0){
        return VIDEO.players[id].duration;
        } else {
        return 0;
        }
    },

    html5video_player_current_time: function(id){
        return VIDEO.players[id].currentTime;
    },

    html5video_player_set_current_time: function(id, time) {
	VIDEO.players[id].currentTime = time;
    },

    html5video_player_ended: function(id){
        return VIDEO.players[id].ended;
    },

    html5video_player_playback_rate: function(id){
        return VIDEO.players[id].playbackRate;
    },

    html5video_player_set_playback_rate: function(id,rate){
        VIDEO.players[id].playbackRate = rate;
    },

    html5video_player_volume: function(id){
        return VIDEO.players[id].volume;
    },

    html5video_player_set_volume: function(id,volume){
        VIDEO.players[id].volume = volume;
    },

    html5video_player_set_loop: function(id,loop){
        VIDEO.players[id].loop = loop;
    },

    html5video_player_loop: function(id){
        return VIDEO.players[id].loop;
    },
    
    html5video_player_set_pan: function (id, pan) {
    	VIDEO.players[id].soundPan.pan.value = pan;
    },

    html5video_player_pan: function (id) {
        return VIDEO.players[id].soundPan.pan.value;
    },
    
    html5video_grabber_create: function(){
	var model = handPoseDetection.SupportedModels.MediaPipeHands;
	var detectorConfig = {
		runtime: 'tfjs',
		modelType: 'lite'
	}
	handPoseDetection.createDetector(model, detectorConfig).then(dect => {
		detector = dect;
	});
	
        var video = document.createElement('video');
        video.autoplay=true;
        video.pixelFormat = "RGB";

        var grabber_id = VIDEO.getNewGrabberId();
        VIDEO.grabbers[grabber_id] = video;
	var texId = GL.getNewId(GL.textures);
	var texture = GLctx.createTexture();
	texture.name = texId;
	GL.textures[texId] = texture;
	GLctx.bindTexture(GLctx.TEXTURE_2D, texture);
	GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_MAG_FILTER, GLctx.LINEAR);
	GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_MIN_FILTER, GLctx.LINEAR);
	GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_WRAP_S, GLctx.CLAMP_TO_EDGE);
	GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_WRAP_T, GLctx.CLAMP_TO_EDGE);
	VIDEO.grabbers[grabber_id].textureId = texId;
	return grabber_id;
    },

    html5video_grabber_init__deps: ['$GL'],
    html5video_grabber_init: function(id, w, h, framerate){
    	if(id!=-1){
        	VIDEO.grabbers[id].width = w;
        	VIDEO.grabbers[id].height = h;

    	    var videoImage = document.createElement( 'canvas' );
    	    videoImage.width = w;
    	    videoImage.height = h;

    	    var videoImageContext = videoImage.getContext( '2d' );
    	    // background color if no video present
    	    videoImageContext.fillStyle = '#000000';
    	    videoImageContext.fillRect( 0, 0, w, h );
    	    videoImageContext.translate(w, 0);
    	    videoImageContext.scale(-1, 1);

    	    VIDEO.grabbersContexts[id] = videoImageContext;

    		var errorCallback = function(e) {
    			console.log('Couldn\'t init grabber!', e);
    		};

    		if(framerate==-1){
    			var constraints = {
	    			video: {
		    		    mandatory: {
		    		        maxWidth: w,
		    		        maxHeight: h,
		    		    }
	    		    }
    			};
    		}else{
    			var constraints = {
	    			video: {
		    		    mandatory: {
		    		        maxWidth: w,
		    		        maxHeight: h,
		    		    },
    					optional: [
    					    { minFrameRate: framerate }
		    		    ]
	    		    }
    			};
    		}

        navigator.mediaDevices.getUserMedia(constraints)
        .then(function(stream) {
          window.stream = stream;
          VIDEO.grabbers[id].srcObject = stream;
          VIDEO.grabbers[id].onloadedmetadata = function (e){
            VIDEO.grabbers[id].play();
          }
        })
        .catch(function(err) {
          console.log(e);
        });

    	}
    },

    html5video_grabber_pixel_format: function(id){
        var string = VIDEO.grabbers[id].pixelFormat;
        var size = lengthBytesUTF8(string) + 1;
        var stringPointer = stackAlloc(size);
        stringToUTF8Array(string, HEAP8, stringPointer, size);
        return stringPointer;
    },

    html5video_grabber_set_pixel_format: function(id, format){
        VIDEO.grabbers[id].pixelFormat = UTF8ToString(format);
    },

    html5video_grabber_update__deps: ['$GL'],
    html5video_grabber_update: function(id,update_pixels,pixels){
        var grabber = VIDEO.grabbers[id];
        if ( grabber.readyState >= grabber.HAVE_METADATA ) {
        	VIDEO.update(update_pixels, grabber, VIDEO.grabbersContexts[id], pixels);
            return true;
        }else{
        	return false;
        }
    },

    html5video_grabber_texture_id: function(id){
        return VIDEO.grabbers[id].textureId;
    },

    html5video_grabber_width: function(id){
        return VIDEO.grabbers[id].width;
    },

    html5video_grabber_height: function(id){
        return VIDEO.grabbers[id].height;
    },

    html5video_grabber_ready_state: function(id){
        return VIDEO.grabbers[id].readyState;
    },

}

autoAddDeps(LibraryHTML5Video, '$VIDEO');
mergeInto(LibraryManager.library, LibraryHTML5Video);
