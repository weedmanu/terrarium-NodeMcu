<?php 
session_start(); // pour pouvoir utiliser les sessions

  if(empty($_POST))
		{
		  $limit = 1440;
		  $graphe = 'datamoyh.php';
		  $titre = "<strong>Historique du terrarium  des dernières 24 heures </strong><em>(Données lissées)</em>";			  
		}
	
  // Si le tableau $_POST existe alors le formulaire a été envoyé
  if(!empty($_POST))
  {
		$_SESSION['form']['graphe'] = $_POST['graphe']; 
		$_SESSION['form']['limit'] = $_POST['limit']; 
		
	    $graphe = $_SESSION['form']['graphe']; 
	    $limit = $_SESSION['form']['limit'];  
	    
	    
		if ($graphe == 'data.php') {	
			switch ($limit) {
			case "1440":
				$titre = "<strong>Historique du terrarium des dernières 24 heures </strong><em>(Données bruts)</em>";
				break;
			case "10080":
				$titre = "<strong>Historique du terrarium des 7 derniers jours </strong><em>(Données bruts)</em>";
				break;
			case "43200":
				$titre = "<strong>Historique du terrarium des 30 derniers jours </strong><em>(Données bruts)</em>";
				break;
			case "1000000":
				$titre = "<strong>Historique du terrarium </strong><em>(Données bruts)</em>";
				break;	   }		
			
			} else  { 
				switch ($limit) {
			case "1440":
				$titre = "<strong>Historique du terrarium  des dernières 24 heures </strong><em>(Données lissées)</em>";
				break;
			case "10080":
				$titre = "<strong>Historique du terrarium des 7 derniers jours </strong><em>(Données lissées)</em>";
				break;
			case "43200":
				$titre = "<strong>Historique du terrarium des 30 derniers jours </strong><em>(Données lissées)</em>";
				break;
			case "1000000":
				$titre = "<strong>Historique du terrarium </strong><em>(Données lissées)</em>";
				break;	   		}
					
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
				text: 'clique sur la légende pour afficher ou non la courbe, clique et selectionne une zone du graphe pour zoomer'
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

<header>    <!-- entête -->

<div class="conteneur">       
	 
	<div class="element" id="date"> <!-- contiendra la date -->
	
		<script type="text/javascript">window.onload = date('date');</script>
	
	</div>

	<div class="element" id="heure"> <!-- contiendra l'heure --> 
	
		<script type="text/javascript">window.onload = heure('heure');</script>	
	
	</div>  
	
</div>      

</header>
        
        
        
<section>	<!-- corps de page -->	
	
	<h1><?php echo "$titre"; ?></h1>
						
	<div id="container"> <!-- contiendra le graphique -->
		
		<div class="loader">Loading...</div>
	
	</div> 
	
		
	
	<form id="formu" method="post" action="histo.php"> <!-- le formulaire -->
    <p>
	   <select id="graphe" name="graphe">

		   <option value="data.php" <?php if ($graphe == "data.php") { echo 'selected="selected"'; }?> >Données bruts</option>

		   <option value="datamoyh.php" <?php if ($graphe == "datamoyh.php") { echo 'selected="selected"'; }?> >Données lissées</option>							   

	   </select>
	   <select id="limit" name="limit" >

		   <option value="1440" <?php if ($limit == 1440) { echo 'selected="selected"'; }?> >Les 24 deniéres heures</option>

		   <option value="10080" <?php if ($limit == 10080) { echo 'selected="selected"'; }?> >Les 7 derniers jours</option>
		   
		   <option value="43200" <?php if ($limit == 43200) { echo 'selected="selected"'; }?> >Les 30 derniers jours</option>	
		   
		   <option value="1000000" <?php if ($limit == "1000000") { echo 'selected="selected"'; }?> >Tout</option>
		   
								   

	   </select>
	   
	   <input type="submit" id="submit" name="submit" value="Valider" />
    </p>
	</form>

</section>
        
        
        
<footer>	<!-- pied de page -->
	
	<div class="conteneur">       

		<div class="element" id="serpent">
			<a href="index.php" style="text-decoration:none"><span id="accueil">Accueil</span></a> <!--lien vers la page d'accueil-->
		</div> 


		<div class="element" id="bdd"><?php require'bdd.php';?></div> <!-- la bdd -->		


		<div class="element" id="pc">
			<a href="admin/index.php" style="text-decoration:none"><span id="admin">Admin</span></a><!--lien vers la page admin-->
		</div>            
			
	</div>									

</footer>
             
 

</body>
</html>


