// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
var new_text = "";
var content = {};
var clay = {};
var source = "main";
var first_run = true;
var userData = {};

// Assemble data object
var dict_out = {'201': 0};
// Send the object
Pebble.addEventListener('ready', function(e1){
  Pebble.addEventListener('showConfiguration', function(e2){
    console.log("Showing configuration");
    console.log('Sending message: ' + JSON.stringify(dict_out));
    Pebble.sendAppMessage(dict_out, function() {
      console.log('Message sent successfully: ' + JSON.stringify(dict_out));
    }, function(e) {
      console.log('Message failed: ' + JSON.stringify(e));
    });
    console.log('Source: ' + source);
    /*if(source == "main"){
      clay.destroy();
    }*/
  });
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (e && !e.response) {
    return;
  }
  console.log("Closing configuration");

  // Get the keys and values from each config item
  var Dict_out = clay.getSettings(e.response);
  localStorage.setItem("parse_cards", Dict_out["10008"]);
  delete Dict_out["10000"];
  console.log("Settings: " + JSON.stringify(Dict_out));
  /*var Dict_out_new = {'101':Dict_out["10001"], '102':Dict_out["10002"], '103':Dict_out["10003"]};
  //var Dict_out_new = {"101":Dict_out["10001"], "102":Dict_out["10002"], "103":Dict_out["10003"]};
  console.log(JSON.stringify(Dict_out_new));*/

  // Send settings values to watch side
  console.log('Sending message: ' + JSON.stringify(Dict_out));
  Pebble.sendAppMessage(Dict_out, function() {
    console.log('Message sent successfully: ' + JSON.stringify(Dict_out));
  }, function(e1) {
    console.log('Message failed: ' + JSON.stringify(e1));
  });
  /*for(var key in Dict_out_new){
    var dict_temp = {};
    dict_temp[key] = Dict_out_new[key];
    console.log(JSON.stringify(dict_temp));
    Pebble.sendAppMessage(dict_temp, function() {
      console.log('Message sent successfully: ' + JSON.stringify(dict_out));
    }, function(e1) {
      console.log('Message failed: ' + JSON.stringify(e1));
    });
  }*/
  console.log('Source: ' + source);
  source = "main";
  clay = {};
});


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
  var team_parse = ['A', 'B'];
  var subs_parse = ['-'];
  var cards_parse = ['R', 'Y', 'W'];
  //console.log('Found in local storage: ' + localStorage.getItem("parse_cards"));
  /*if(localStorage.getItem("parse_cards")){
    cards_parse = localStorage.getItem("parse_cards").split(',');
  }*/
  keys = Object.keys(data);
  for(var i = 0; i < keys.length; i++){
    Dict[keys[i]] = data[keys[i]];
  }
  keys.sort(function(a, b){return a - b;});
  //console.log("Keys: " + keys);
  for (i = 0; i < keys.length; i++){
    //console.log("Parsing " + i + " " + keys[i] + "" + Dict[keys[i]]);
    if(keys[i] == 65535){
      events.push(Dict[keys[i]]);
    } else {
      var secs_abs = keys[i] - i;
      var period = Math.floor(secs_abs / 5000);
      var secs_period = secs_abs - period * 5000;
      var mins = Math.floor(secs_period / 60);
      var secs = secs_period - 60 * mins;
      var event = "";
      if (Dict[keys[i]].substring(0, 1) == "#"){
        //console.log(i + " " + keys[i] + " " + Dict[keys[i]]);
        var i1 = Number(Dict[keys[i]].substring(1, 2));
        var i2 = Number(Dict[keys[i]].substring(2, 3));
        event = ["End Period ", "Try", "Conv", "Pen", "Drop", "Card", "Inj"][i1] + " " + ["", "Team A ", "Team B "][i2];
      }
      else {
        event = Dict[keys[i]];
      }
      //console.log("Parsed " + event);
      events.push((period + 1).toString() + " " + ("00" + mins).slice(-2) + ":" + ("00" + secs).slice(-2) + " " + event);
    }
  }
  for (i = 0; i < events.length; i++){
    report = report + events[i] + new_line;
    if(subs_parse.some(function(s, index, array){return ~events[i].indexOf(s);})){
    //if (~events[i].indexOf('-')){
      if (~events[i].indexOf(team_parse[0])){
        subs_a.push(events[i]);
      }
      if (~events[i].indexOf(team_parse[1])){
        subs_b.push(events[i]);
      }
    }
    /*function element_exists(s, index, array){
      return ~events[i].indexOf(s);
    }*/
    if(cards_parse.some(function(s, index, array){return ~events[i].indexOf(s);})){
    //if (~events[i].indexOf('R') | ~events[i].indexOf('Y') | ~events[i].indexOf('W')){
      cards.push(events[i]);
    }
  }
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

// Get AppMessage events
Pebble.addEventListener('appmessage', function(e) {
  // Get the dictionary from the message
  //console.log(new_text);
  var dict = e.payload;
  console.log("js: got message!" + JSON.stringify(dict));
  for (var k in dict){
    content[k] = dict[k];
  }
  console.log(JSON.stringify(content));
  if(65535 in content){
    //console.log("Parsing last batch");
    new_text = parse_events(content);
    clayConfig[6]["defaultValue"] = new_text;
    var args = {data_in: new_text, data_out: "No data"};
    console.log(args.data_in);
    source = "watch";
    userData = {userData: args};
    if(first_run){
      clay = new Clay(clayConfig, require('./custom-clay'), userData, {autoHandleEvents: false});
    }
    //clay = new Clay(clayConfig);
    clay.registerComponent(require('./clay-component-textarea'));
    //clay.build();
    console.log(args.data_out);
    console.log(new_text);
    /*console.log((new_text.replace(/ /g, "%20")).replace(/\\n/g, "%5Cn"));
    var new_url = clay.generateUrl().replace(/input%22%2C%0A%20%20%20%20defaultValue%3A%22.*%22%20%7D%2C%0A%20%20%7B%20/g, "input%22%2C%0A%20%20%20%20defaultValue%3A%22" + (new_text.replace(/ /g, "%20")).replace(/\\n/g, "%5Cn") + "%22%20%7D%2C%0A%20%20%7B%20");
    //var new_url = clay.generateUrl().replace(/input%22%2C%0A%20%20%20%20defaultValue%3A%22.*%22%20%7D%2C%0A%20%20%7B%20/g, "input%22%2C%0A%20%20%20%20defaultValue%3A%22" + (new_text.replace(/ /g, "%20")) + "%22%20%7D%2C%0A%20%20%7B%20");
    console.log(new_url);*/
    if(first_run){
      first_run = false;
    } else {
      clay.destroy();
      clay = new Clay(clayConfig, require('./custom-clay'), userData, {autoHandleEvents: false});
      //Pebble.openURL(clay.generateUrl());
    }
    content = {};
  }
  console.log("js: parsed message!");
});
