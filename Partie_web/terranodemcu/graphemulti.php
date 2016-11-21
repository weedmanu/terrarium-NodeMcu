<?php 

  if(empty($_POST['graphe']))
		{
		  $graphe = 'datamoyh.php';
		  $titre = "Historique du terrarium <br/> <em>(Moyennes par heure)</em>";		  
		}

  // Si le tableau $_POST existe alors le formulaire a été envoyé
  if(!empty($_POST))
  {
	    $graphe = htmlspecialchars($_POST['graphe']); 
		if ($graphe == 'data.php') {
			$titre = "Historique du terrarium <br/> <em>(Données bruts)</em>";
			} else  { 
				$titre = "Historique du terrarium  <br/> <em>(Données lissées, moyennes par heure)</em>";	
				}
  }
?>


<!DOCTYPE html> 
<head> 
		<meta charset="utf-8" /> 
        <title>Terrarium</title> <!-- titre -->
        <link rel="icon" type="image/png" href="img/serpent.png" />
        <script type="text/javascript" src="lib/jquery.js"></script> <!-- appel de la librairie jquery --> 
        <script type="text/javascript" src="lib/dateheure.js"></script> <!-- appel de la fonction date et heure javascript --> 
        <script src="lib/highcharts.js"></script> <!-- appel de la librairie highcharts --> 
        
        <script src="lib/gray.js"></script> <!-- appel du thème highcharts --> 
        <link rel="stylesheet" href="histo.css" /> <!-- appel du thème de la page --> 
</head>
<script type="text/javascript">
$(function () {
	$.getJSON('<?php echo $graphe; ?>', function (data) {  // on récupere les data du j		
	
		$('#container').highcharts({
			chart: {
				zoomType: 'x',
				spacingBottom: 15,
				spacingTop: 50,
				spacingLeft: 10,
				spacingRight: 10,
			},
			title: {
				text: '<?php echo $titre; ?>'
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
					text: 'Température',
				}				

			}, { // 2EME yAxis
				gridLineWidth: 0,
				title: {
					text: 'humidité',
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
				name: 'Température pointchaud',
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
				name: 'Température pointfroid',
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
				name: 'humidité pointfroid',
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
				name: 'humidité pointchaud',
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
</script>
<body>
	<div class="form">
					<form method="post" action="graphemulti.php">
					   <p>
						   <label for="graphe">Quel graphe voulez-vous ? </label>

						   <select name="graphe" id="graphe">

							   <option value="data.php" checked="checked">Donnée brut</option>

							   <option value="datamoyh.php">Moyenne par heure</option>							   

						   </select>
						   
						   <input type="submit" name="submit" value="Valider" />
					   </p>
					</form>
					</div>

        <header>    <!-- entête -->

        <div class="conteneur">        
			<div class="element" id="date"><?php require'date.php';?></div> <!-- contiendra la date -->
    
			<div class="element" id="heure"><?php require'heure.php';?></div> <!-- contiendra l'heure -->  
        </div>      

        </header>
        
        
        
				<section>	<!-- corps de page -->	
									
					<div id="container"></div> <!-- contiendra le graphique -->
				
				</section>
        
        
        
                <footer>	<!-- pied de page -->
			        
         <div class="conteneur">       
			 
			<div class="element" id="serpent">
                <a href="index.php" style="text-decoration:none"><span id="accueil">Accueil</span></a> <!--lien vers la page historique-->
            </div> 
    
			
			<div class="element" id="bdd"><?php require'bdd.php';?></div> <!-- contiendra la gestion de la bdd -->		
			

			<div class="element" id="pc">
                <a href="admin/index.php" style="text-decoration:none"><span id="admin">Admin</span></a><!--lien vers la page admin-->
            </div>            
            			
        </div>									

		</footer>
        
        
       
 

</body>
</html>


