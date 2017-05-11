module.exports = {
  name: 'input_mod',
  template: '<div class="component component-textarea">' +
              //'<label class="tap-highlight">' +
                //'<span class="label">{{{label}}}</span>' +
                '<span class="text">' +
                  '<textarea data-manipulator-target></textarea>' +
                '</span>' +
              //'</label>' +
              /*'{{if description}}' +
                '<div class="description">{{{description}}}</div>' +
              '{{/if}}' +*/
            '</div>',
  style: '.component-textarea textarea {height: 38em; width: 25em}',
  manipulator: 'html',
  defaults: {
    label: '',
    description: ''
  }
};