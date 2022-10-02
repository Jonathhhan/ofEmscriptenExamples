function midiMessageReceived( ev ) {
  var channel = ev.data[0];
  var pitch = ev.data[1];
  var velocity = ev.data[2];
  console.log( "MIDI in: " + ev.data[0] + " " + ev.data[1] + " " + ev.data[2])
  Module.midiIn(ev.data[0] , ev.data[1] , ev.data[2]);
  }
  
  Object.defineProperty(window, "setMidiOut", { 
  set: function(setMidiOut) {
    sendMIDI(setMidiOut); 
    }
  }); 
  
function sendMIDI(ev) { 
  const device = midiOut; 
  console.log("MIDI out: " + ev[0] + " " + ev[1] + " " + ev[2]) 
  device.send(ev);  
  }

var inSelectMIDI = null;
var outSelectMIDI = null
var midiAccess = null;
var midiIn = null;
var midiOut = null;

function selectMIDIIn( ev ) {
  if (midiIn)
    midiIn.onmidimessage = null;
    var id = ev.target[ev.target.selectedIndex].value;
    if ((typeof(midiAccess.inputs) == "function"))   //Old Skool MIDI inputs() code
      midiIn = midiAccess.inputs()[ev.target.selectedIndex];
    else
      midiIn = midiAccess.inputs.get(id);
      if (midiIn)
        midiIn.onmidimessage = midiMessageReceived;
  }

function populateMIDIInSelect() {
  // clear the MIDI input select
  inSelectMIDI.options.length = 0;
  if (midiIn && midiIn.state=="disconnected")
    midiIn=null;
    var firstInput = null;
    var inputs=midiAccess.inputs.values();
    for ( var input = inputs.next(); input && !input.done; input = inputs.next()){
      input = input.value;
      if (!firstInput)
        firstInput=input;
        var str=input.name.toString();
        var preferred = !midiIn;	
    // if we're rebuilding the list, but we already had this port open, reselect it.
      if (midiIn && midiIn==input)
        preferred = true;
        inSelectMIDI.appendChild(new Option(input.name,input.id,preferred,preferred));
      if (preferred) {
        midiIn = input;
        midiIn.onmidimessage = midiMessageReceived;
    }
  }
  if (!midiIn) {
    midiIn = firstInput;
    if (midiIn)
      midiIn.onmidimessage = midiMessageReceived;
  }
}

function midiConnectionStateChangeIn( e ) {
  console.log("connection in: " + e.port.name + " " + e.port.connection + " " + e.port.state );
  populateMIDIInSelect();
}

function onMIDIStartedIn( midi ) {
  var preferredIndex = 0;
  midiAccess = midi;
  document.getElementById("midiIn");
  inSelectMIDI=document.getElementById("midiIn");
  midi.onstatechange = midiConnectionStateChangeIn;
  populateMIDIInSelect();
  inSelectMIDI.onchange = selectMIDIIn; 
}

function selectMIDIOut( ev ) {
  if (midiOut)
    midiOut.onmidimessage = null;
    var id = ev.target[ev.target.selectedIndex].value;
  if ((typeof(midiAccess.outputs) == "function"))   //Old Skool MIDI inputs() code
    midiOut = midiAccess.outputs()[ev.target.selectedIndex];
  else
    midiOut = midiAccess.outputs.get(id);
  if (midiOut)
    midiOut.onmidimessage = midiMessageReceived;
  }

function populateMIDIOutSelect() {
  // clear the MIDI input select
  outSelectMIDI.options.length = 0;
  if (midiOut && midiOut.state=="disconnected")
    midiOut=null;
    var firstOutput = null;
    var outputs=midiAccess.outputs.values();
  for ( var output = outputs.next(); output && !output.done; output = outputs.next()){
    output = output.value;
    if (!firstOutput)
      firstOutput=output;
      var str=output.name.toString();
      var preferred = !midiOut;

    // if we're rebuilding the list, but we already had this port open, reselect it.
    if (midiOut && midiOut==output)
      preferred = true;
      outSelectMIDI.appendChild(new Option(output.name,output.id,preferred,preferred));
    if (preferred) {
      midiOut = output;
      midiOut.onmidimessage = midiMessageReceived;
      }
  }
  if (!midiOut) {
    midiOut = firstOutput;
  if (midiOut)
    midiOut.onmidimessage = midiMessageReceived;
  }
}

function midiConnectionStateChangeOut( e ) {
  console.log("connection out: " + e.port.name + " " + e.port.connection + " " + e.port.state );
  populateMIDIOutSelect();
}

function onMIDIStartedOut( midi ) {
  var preferredIndex = 0;
  midiAccess = midi;
  document.getElementById("midiOut");
  outSelectMIDI=document.getElementById("midiOut");
  midi.onstatechange = midiConnectionStateChangeOut;
  populateMIDIOutSelect();
  outSelectMIDI.onchange = selectMIDIOut;
}

function onMIDISystemError( err ) {
  console.log( "MIDI not initialized - error encountered:" + err.code );
}

//init: start up MIDI
window.addEventListener('load', function() {   
  if (navigator.requestMIDIAccess)
    navigator.requestMIDIAccess().then( onMIDIStartedIn, onMIDISystemError );
    navigator.requestMIDIAccess().then( onMIDIStartedOut, onMIDISystemError );
  });
