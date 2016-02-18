/*
 * Interfacing D3 library and generating plot
 *
 * Author: Adam Benda <adam@adasek.cz>, 2016
*/

svg = null;
line = null;
xaxis = null;
yaxis = null;
plot = null;
sensor = null;

function showPlot(sensorID, sensorPos, color, from, to) {
    console.log("showPlot(" + sensorID + "," + sensorPos + "," + color + "," + from + "," + to + ");");

    if (color === null || color === undefined || color === '') {
        color = '#ff0000';
    }

    if (to === undefined) {
        to = Math.floor(Date.now() / 1000);
    } else {
        to = parseInt(to);
    }
    if (from === undefined) {
        from = to - (3600 * 12); //default interval: 12h
        from = parseInt(from);
    }

    var fromDate = new Date(from * 1000);
    var toDate = new Date(to * 1000);




    plot = new Plot("body");



    sensor = new SensorData(sensorID, sensorPos, function () {});

    var showData = new SensorDataSet(sensor, fromDate, toDate, function () {

        //Add dataset to plot
        plot.datasets = [this];
        plot.plot();

    });

}


/**
 * Plot again based on form options.
 * Called onSubmit of replot options form
 * 
 * @returns {false} we don't want to send the form
 */
function replot(sensorID, sensorPos, color) {

    var newFromDate = sensor.timestr2date(document.getElementById('replotForm_fromTime').value);
    var newToDate = sensor.timestr2date(document.getElementById('replotForm_toTime').value);
    if (newFromDate === false || newToDate === false) {
        throw "Bad datetime";
    }

    
    showData = new SensorDataSet(sensor, newFromDate, newToDate, function () {
        //Add dataset to plot
        plot.datasets = [this];
        plot.plot();
    });


    return false;
}
