module.exports = function(minified) {
  var clayConfig = this;
  var _ = minified._;
  var $ = minified.$;
  var HTML = minified.HTML;
  var text;

  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
    text = clayConfig.meta.userData.data_in;
    clayConfig.getItemByMessageKey('report').set(text);
  });
};