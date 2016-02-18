/* 
 * Author: Adam Benda<adam@adasek.cz>, 2016
 * 
 * @classdesc Represents a plot of one or more SensorDataSet
 * 
 * @class
 */


var Plot = function (node) {
    this.node = node;
    this.color = '#ff0000';
};

Plot.prototype.plot = function () {


    var margin = {top: 20, right: 20, bottom: 30, left: 100},
    width = 1020 - margin.left - margin.right,
            height = 600 - margin.top - margin.bottom;



// Set the ranges
    var x = d3.time.scale().range([0, width]);
    var y = d3.scale.linear().range([height, 0]);

// Define the axes
    var xAxis = d3.svg.axis().scale(x)
            .orient("bottom").ticks(5)
            .tickFormat(d3.time.format("%d.%m. %H:%M"));

    var yAxis = d3.svg.axis().scale(y)
            .orient("left").ticks(5);

// Define the line
    var valueline = d3.svg.line()
            .x(function (d) {
                return x(d.time);
            })
            .y(function (d) {
                return y(d.value);
            });


    /* data loaded */
    for (var datasetI = 0; datasetI < this.datasets.length; datasetI++) {
        var data = this.datasets[datasetI].getDataD3();

        data.forEach(function (d) {
            d.time = d.time * 1000;
            d.value = +d.value;
        });

        dataset = data;

        // Scale the range of the data      
        var x_ext = d3.extent(dataset, function (d) {
            return d.time;
        });
        
        x.domain(d3.extent(dataset, function (d) {
            return d.time;
        }));
        
        y.domain([
            d3.min(dataset, function (d) {
                return d.value;
            }),
            d3.max(dataset, function (d) {
                return d.value;
            })]);

        if (svg === null) {
            svg = d3.select(this.node)
                    .append("svg")
                    .attr("width", width + margin.left + margin.right)
                    .attr("height", height + margin.top + margin.bottom).append("g")
                    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");
        } else {
            line.remove();
            xaxis.remove();
            yaxis.remove();
        }


        line = svg.append("path")      // Add the valueline path.
                .attr("d", valueline(dataset))
                .attr("stroke", this.color)
                .attr("fill", "none");

        xaxis = svg.append("g")         // Add the X Axis
                .attr("class", "x axis")
                .attr("transform", "translate(0," + height + ")")
                .call(xAxis);

        yaxis = svg.append("g")         // Add the Y Axis
                .attr("class", "y axis")
                .call(yAxis);

    }

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

            this.plot();
        }
    }
});
