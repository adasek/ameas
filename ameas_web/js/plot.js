/* 
 * Author: Adam Benda<bendaad1@fit.cvut.cz>
 * 
 * @classdesc Represents a plot of one or more SensorDataSet
 * 
 * @class
 */


var Plot = function (node) {

};



Object.defineProperties(Plot.prototype, {
    /**
     * Plot datasets
     *
     * @member {SensorDataSet[]}
     * @memberof Plot#
     */
    datasets: {
        get: function ()
        {
            return this._datasets;
        },
        set: function (value)
        {
            if (!Array.isArray(value)) {
                throw "Bad datasets given to plot";
            }
            this._datasets = value;

            this.replot();
        }
    }
});
