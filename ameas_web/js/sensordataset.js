/* 
 * Author: Adam Benda<adam@adasek.cz>, 2016
 * 
 * @classdesc Represents subset of data. Contains actual measurement data.
 * 
 * @class
 */

/**
 * 
 * @param {SensorData} parent - SensorData
 * @param {Date} from - lowest value on X (time)
 * @param {Date} to - highest value on X (time)
 * @param {function} callback - Function to call after successfully initialized
 * @returns {SensorDataSet}
 */
var SensorDataSet = function (parent, from, to, callback) {
    if (Object.getPrototypeOf(from) !== Date.prototype || Object.getPrototypeOf(to) !== Date.prototype) {
        throw "SensorDataSet constructor from and to bad parameter type";
    }

    this.callback = callback;

    this.parent = parent;

    this.ready = false;
    var dataAddr = "http://adasek.cz/ameas/data.php?id=" + parent.id + "&pos=" + parent.pos + '&from=' + from.getTime() / 1000 + '&to=' + to.getTime() / 1000;

    jQuery.ajax(dataAddr)
            .done(function (data) {
                this.ready = true;
                this._data = data;
                this.callback();
            }.bind(this));
};

SensorDataSet.prototype.getDataD3 = function () {
    if (!this.ready || this._data === undefined) {
        throw "Data in SensorDataSet are not ready yet!";
    }
    
    return this._data;
};
