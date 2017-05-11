// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
var userData = {data_in: "No data from pebble"};
var clay = new Clay(clayConfig, null, {autoHandleEvents: false});
clay.registerComponent(require('./clay-component-textarea'));
// Initiate variables
var message_received = false;
var dict_out = {'201': 0};
var content = {};
var last_report = encodeURIComponent(userData.data_in);
var old_url = "";
old_url = clay.generateUrl();
var new_text_uri = "";

var team_parse = Array('A', 'B');
var subs_parse = Array('-');
var cards_parse = Array('R', 'Y');

function find_char(s, c){
  var result = false;
  for(var i = 0; i < c.length; i++){
    result = (s.indexOf(c[i]) >= 0) || result;
  }
  return result;
}

// Parse incoming data from pebble
function parse_events(data){
  var events = new Array();
  var subs_a = new Array();
  var subs_b = new Array();
  var cards = new Array();
  var keys = new Array();
  var new_line = '\n';
  var Dict = {};
  var report = "";
  // Create working dictionary
  keys = Object.keys(data);
  for(var i = 0; i < keys.length; i++){
    Dict[keys[i]] = data[keys[i]];
  }
  // sort keys and loop
  keys.sort(function(a, b){return a - b;});
  for (i = 0; i < keys.length; i++){
    // last key
    if(keys[i] == 65535){
      events.push(Dict[keys[i]]);
    } else { 
      // other keys: extract period and time
      var secs_abs = keys[i] - i;
      var period = Math.floor(secs_abs / 5000);
      var secs_period = secs_abs - period * 5000;
      var mins = Math.floor(secs_period / 60);
      var secs = secs_period - 60 * mins;
      var event = "";
      // parse event type if coded, pass it otherwise
      if (Dict[keys[i]].substring(0, 1) == "#"){
        var i1 = Number(Dict[keys[i]].substring(1, 2));
        var i2 = Number(Dict[keys[i]].substring(2, 3));
        event = ["End Period ", "Try", "Conv", "Pen", "Drop", "Card", "Inj"][i1] + " " + ["", "Team " + team_parse[0] + " ", "Team " + team_parse[1] + " "][i2];
      } else {
        event = Dict[keys[i]];
      }
      events.push((period + 1).toString() + " " + ("00" + mins).slice(-2) + ":" + ("00" + secs).slice(-2) + " " + event);
    }
  }
  // build final string, parse events for subs and cards
  for (i = 0; i < events.length; i++){
    report = report + events[i] + new_line;
    if(events[i].indexOf(subs_parse) >= 0){
      if (events[i].indexOf(team_parse[0]) >= 0){
        subs_a.push(events[i]);
      }
      if (events[i].indexOf(team_parse[1]) >= 0){
        subs_b.push(events[i]);
      }
    }
    if(find_char(events[i], cards_parse)){
      cards.push(events[i]);
    }
  }
  // append cards and subs to final string
  report = report + "=====" + new_line + "SUBS " + team_parse[0] + new_line;
  for (i = 0; i < subs_a.length; i++){
    report = report + subs_a[i] + new_line;
  }
  report = report + "=====" + new_line + "SUBS " + team_parse[1] + new_line;
  for (i = 0; i < subs_b.length; i++){
    report = report + subs_b[i] + new_line;
  }
  report = report + "=====" + new_line + "CARDS" + new_line;
  for (i = 0; i < cards.length; i++){
    report = report + cards[i] + new_line;
  }
  return report;
}

// Show the config anyway even if pebble doesn't respond
function show_config(){
  if(!(message_received)){
    Pebble.openURL(clay.generateUrl());
  }
}

// System ready
Pebble.addEventListener('ready', function(e1){
  userData.data_in = "No data from pebble";

  // App message received from pebble
  Pebble.addEventListener('appmessage', function(e) {
    var dict = e.payload;
    
    // Store message dictionary content...
    console.log("js: got message!" + JSON.stringify(dict));
    for (var k in dict){
      content[k] = dict[k];
    }
    //... until end is detected
    if(65535 in content){
      // Block config show timer
      message_received = true;
      // Create and update config url
      var new_text = parse_events(content);
      old_url = clay.generateUrl();
      console.log("Old URL: " + old_url);
      new_text_uri = encodeURIComponent(new_text).replace(/%0A/g, "%5Cn");
      // var new_url = (((old_url.replace(encodeURIComponent(userData.data_in), new_text_uri)).replace(encodeURIComponent(userData.data_in), new_text_uri)).replace(last_report, new_text_uri)).replace(last_report, new_text_uri);
      // var new_url = (old_url.replace(encodeURIComponent(userData.data_in), new_text_uri)).replace(last_report, new_text_uri);
      console.log("Replacing " + last_report + " with " + new_text_uri);
      console.log("Replacing " + encodeURIComponent(userData.data_in) + " with " + new_text_uri);
      var new_url = (old_url.replace(last_report, new_text_uri)).replace(encodeURIComponent(userData.data_in), new_text_uri);
      console.log("New URL: " + new_url);
      Pebble.openURL(new_url);
      // Reset variables for next call
      content = {};
    }
    console.log("js: parsed message!");
  });
    
  Pebble.addEventListener('showConfiguration', function(e2){
    // Initiate timer for showing config
    message_received = false;
    setTimeout(show_config, 10 * 1000);
    console.log("Showing configuration");
    // Request data from pebble
    console.log('Sending message: ' + JSON.stringify(dict_out));
    Pebble.sendAppMessage(dict_out, function() {
      console.log('Message sent successfully: ' + JSON.stringify(dict_out));
    }, function(e) {
      console.log('Message failed: ' + JSON.stringify(e));
    });
  });

  Pebble.addEventListener('webviewclosed', function(e) {
    last_report = new_text_uri;
    if (e && !e.response) {
      return;
    }
    console.log("Closing configuration");
      
    // Get the keys and values from each config item
    var Dict_out = clay.getSettings(e.response);
    // Update parsing values
    team_parse[0] = Dict_out[10009].split(',')[0];
    team_parse[1] = Dict_out[10009].split(',')[1];
    subs_parse = [Dict_out[10010]];
    cards_parse[0] = Dict_out[10007].split(',')[0];
    cards_parse[1] = Dict_out[10007].split(',')[1];
    // Send new configuration to pebble
    var dict_to_send = {
      10001:Dict_out[10000], 
      10002:Dict_out[10001], 
      10003:Dict_out[10002], 
      10004:Dict_out[10003], 
      10005:Dict_out[10004], 
      10006:Dict_out[10005], 
      10007:Dict_out[10006], 
      10009:Dict_out[10010], 
      10010:Dict_out[10009]
    };
    console.log('Sending message: ' + JSON.stringify(dict_to_send));
    Pebble.sendAppMessage(dict_to_send, function() {
      console.log('Message sent successfully: ' + JSON.stringify(dict_to_send));
    }, function(e2) {
      console.log('Message failed: ' + JSON.stringify(e2));
    });
  });
  
});
