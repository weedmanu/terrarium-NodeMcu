
$(function () {
	$.getJSON('data.php', function (data) {  // on récupere les data du j		
	
		$('#container').highcharts({
			chart: {
				zoomType: 'x',
				spacingBottom: 15,
				spacingTop: 50,
				spacingLeft: 10,
				spacingRight: 10,
			},
			title: {
				text: 'Historique terrarium'
			},
			subtitle: {
				text: 'click sur la légende pour afficher ou pas la courbe <br/> click et selectionne une zone du graphe pour zoomer'
			},
			xAxis: [{
				categories: data[0].data
				
			}],
			yAxis: [{ // Primary yAxis
				labels: {
					format: '{value}°C',
				},
				title: {
					text: 'Temperature',
				}				

			}, { // 2EME yAxis
				gridLineWidth: 0,
				title: {
					text: 'humiditée',
				},
				labels: {
					format: '{value} %',
				},
				opposite: true
			}],
			tooltip: {
				shared: true
			},
			legend: {                            // la légende 
                    layout: 'vertical',
                    align: 'left',
                    verticalAlign: 'top',
                    x: 60,
                    y: 0,
                    floating: true,
                    borderWidth: 0
                },
				
			series: [{
				name: 'Temperature pointchaud',
				type: 'spline',
				yAxis: 0,
				marker: {
					enabled: false,
					states: {
						hover: {
							enabled: true
						}
					}
            },
				data: data[1].data,
				tooltip: {
					valueSuffix: ' °C'
				}
			} , {
				name: 'Temperature pointfroid',
				type: 'spline',
				yAxis: 0,
				marker: {
					enabled: false,
					states: {
						hover: {
							enabled: true
						}
					}
            },
				data: data[2].data,
				tooltip: {
					valueSuffix: ' °C'
				}
			},  {
				name: 'humiditée pointfroid',
				type: 'spline',
				yAxis: 1,
				marker: {
					enabled: false,
					states: {
						hover: {
							enabled: true
						}
					}
            },
				data: data[4].data,				
				tooltip: {
					valueSuffix: ' %'
				}

			}, {
				name: 'humiditée pointchaud',
				type: 'spline',
				yAxis: 1,
				marker: {
					enabled: false,
					states: {
						hover: {
							enabled: true
						}
					}
            },
				data: data[3].data,				
				tooltip: {
					valueSuffix: ' %'
				}

			}]
		});
	});
});

