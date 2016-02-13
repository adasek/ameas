/* 
 * Author: Adam Benda<bendaad1@fit.cvut.cz>
 * 
 * @classdesc Represents whole data of one sensor position. Contains only metadata, not the actual data.
 * 
 * @class
 */

/**
 * 
 * @param {int} id - Sensor ID
 * @param {int} pos - Position of this value on sensor
 * @param {function} callback - Function to call after successfully initialized
 * @returns {SensorData}
 */
var SensorData = function (id, pos, callback) {
    if (isNaN(id) || isNaN(pos)) {
        throw "Invalid parameters to SensorData constructor";
    }
    this.id = id;
    this.pos = pos;


    //download metadata from web
    this.ready = false;
    this.callback = callback;
    jQuery.ajax("metadata.csv.php?id=" + id + "&pos=" + pos)
            .done(function (data) {
                $(this).ready = true;
                $(this)._data = data;
                $(this).callback();
            });
};


SensorData.prototype.getData = function (fromStr, toStr) {
    return new SensorDataSet(this.id, this.pos, this.timestr2date(fromStr), this.timestr2date(toStr));
};


/* Convert DATETIME in format YYYY-MM-DD HH:II to Date */
/**
 * 
 * @param {String} timeStr - datetime in format yyyy-mm-hh HH:MM
 * @returns {Date} - converted date
 */
SensorData.prototype.timestr2date = function (timeStr) {
    //todo: check dtime against regexp,return false if not successful.

    var myDate = timeStr.split(" ");
    var date = myDate[0].split("-");
    var time = myDate[1].split(":");

    return new Date(date[0], date[1] - 1, date[2], time[0], time[1]);
};