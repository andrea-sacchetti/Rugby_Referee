module.exports = [
  {
    "type": "heading",
    "defaultValue": "Time Settings"
  },
  {
    "type": "slider",
    "messageKey": "half",
    "defaultValue": 40,
    "label": "Half Duration",
    "description": "Duration of one half in minutes",
    "min": 5,
    "max": 60,
    "step": 1
  },
  {
    "type": "slider",
    "messageKey": "card",
    "defaultValue": 10,
    "label": "Card Duration",
    "description": "Duration of a sent-off in minutes",
    "min": 2,
    "max": 20,
    "step": 1
  },
  {
    "type": "slider",
    "messageKey": "injury",
    "defaultValue": 15,
    "label": "Injury Duration",
    "description": "Duration of a blood injury in minutes",
    "min": 2,
    "max": 20,
    "step": 1
  },
  {
    "type": "submit",
    "defaultValue": "OK"
  },
  {
    "type": "heading",
    "defaultValue": "Match Report"
  },
  {
    "type": "input_mod",
    "messageKey": "report",
    "id": "report",
    "defaultValue": "No data from pebble",
  },
  {
    "type": "heading",
    "defaultValue": "Points Settings"
  },
  {
    "type": "slider",
    "messageKey": "try",
    "defaultValue": 5,
    "label": "Try points",
    "description": "Number of points for a try",
    "min": 1,
    "max": 10,
    "step": 1
  },
  {
    "type": "slider",
    "messageKey": "conv",
    "defaultValue": 2,
    "label": "Conversion points",
    "description": "Number of points for a conversion",
    "min": 1,
    "max": 10,
    "step": 1
  },
  {
    "type": "slider",
    "messageKey": "pen",
    "defaultValue": 3,
    "label": "Penalty points",
    "description": "Number of points for a penalty goal",
    "min": 1,
    "max": 10,
    "step": 1
  },
  {
    "type": "slider",
    "messageKey": "drop",
    "defaultValue": 3,
    "label": "Drop points",
    "description": "Number of points for a drop goal",
    "min": 1,
    "max": 10,
    "step": 1
  },
  {
    "type": "heading",
    "defaultValue": "Parsing Settings"
  },
  {
  "type": "input",
  "messageKey": "cards",
  "defaultValue": "Y,R",
  "label": "Cards parsing indicators (coma-sep.)",
  "attributes": {
    "placeholder": "Y,R",
    "limit": 8,
    }
  },
  {
  "type": "input",
  "messageKey": "teams",
  "defaultValue": "A,B",
  "label": "Teams parsing indicators (coma-sep)",
  "attributes": {
    "placeholder": "A,B",
    "limit": 3,
    }
  },
  {
  "type": "input",
  "messageKey": "subs",
  "defaultValue": "-",
  "label": "Subs parsing indicator",
  "attributes": {
    "placeholder": "-",
    "limit": 1,
    }
  },
  {
    "type": "submit",
    "defaultValue": "OK"
  }
];