svg = null;
line = null;
xaxis = null;
yaxis = null;

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
    console.log(valueline);




    //Get the data
    d3.csv("data.csv.php?id=" + sensorID + "&pos=" + sensorPos + '&from=' + from + '&to=' + to, function (data) {


        data.forEach(function (d) {
            d.time = d.time * 1000;
            d.value = +d.value;
        });

        dataset = data;


        // Scale the range of the data      
        var x_ext = d3.extent(dataset, function (d) {
            return d.time;
        });
        console.log(x_ext);
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
            svg = d3.select("body")
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
                .attr("stroke", color)
                .attr("fill", "none");



        xaxis = svg.append("g")         // Add the X Axis
                .attr("class", "x axis")
                .attr("transform", "translate(0," + height + ")")
                .call(xAxis);

        yaxis = svg.append("g")         // Add the Y Axis
                .attr("class", "y axis")
                .call(yAxis);



    });

}


/**
 * Plot again based on form options.
 * Called onSubmit of replot options form
 * 
 * @returns {false} we don't want to send the form
 */
function replot(sensorID, sensorPos, color) {

    var newFromTime = timeH2U(document.getElementById('replotForm_fromTime').value);
    var newToTime = timeH2U(document.getElementById('replotForm_toTime').value);
    if (newFromTime === false || newToTime === false) {
        throw "Bad datetime";
    }


    showPlot(sensorID, sensorPos, color, newFromTime, newToTime);

    return false;
}

/* Convert DATETIME in format YYYY-MM-DD HH:II to timestamp*/
function timeH2U(dtime) {
    //todo: check dtime against regexp,return false if not successful.

    var myDate = dtime.split(" ");
    var date = myDate[0].split("-");
    var time = myDate[1].split(":");

    return new Date(date[0], date[1] - 1, date[2], time[0], time[1]).getTime() / 1000;
}