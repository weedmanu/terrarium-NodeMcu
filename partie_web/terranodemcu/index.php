<!DOCTYPE html> 
<head> 
		<meta charset="utf-8" /> 
        <title>Terrarium</title> <!-- titre -->
        <script type="text/javascript" src="lib/jquery.js"></script> <!-- appel de la librairie jquery --> 
        <script src="lib/RGraph.common.core.js"></script> <!-- appel de la librairie Rgraph --> 
        <script src="lib/RGraph.common.csv.js"></script> <!-- appel de la librairie Rgraph --> 
        <script src="lib/RGraph.gauge.js"></script> <!-- appel de la librairie Rgraph --> 
        <script src="lib/RGraph.common.dynamic"></script> <!-- appel de la librairie Rgraph --> 
        <script type="text/javascript" src="lib/dateheure.js"></script> <!-- appel de la fonction date et heure javascript --> 
        <script src="lib/highcharts.js"></script> <!-- appel de la librairie highcharts --> 
        <script src="lib/graphhisto.js"></script> <!-- appel du graphique highcharts -->
        <script src="lib/gray.js"></script> <!-- appel du thème highcharts --> 
        <link rel="stylesheet" href="index.css" /> <!-- appel du thème de la page --> 
        
   
<script type="text/javascript">  <!-- fonction qui rafraîchi les jauges toute les 30 secondes >    
var auto_refresh = setInterval(
  function ()
  {
    $('#gauge').load('jauge.php').fadeIn("slow");
  }, 30000); // rafraichis toutes les minutes ,30000 millisecondes
 </script> 
    
</head>

<body>

        <header>    <!-- entête -->

        <div id="conteneur">        
			<div class="element" id="date"><?php require'date.php';?></div> <!-- contiendra la date -->
    
			<div class="element" id="heure"><?php require'heure.php';?></div> <!-- contiendra l'heure -->  
        </div>      

        </header>
        
        
        
				<section>	<!-- corps de page -->	
				
				<div id="conteneur1">       
			
					<div class="element" id="gauge"><?php require'jauge.php';?></div> <!-- contiendra les jauges -->
				
				</div>      
				
			
				<div id="container"></div> <!-- contiendra le graphique -->
				
				</section>
        
        
        
        <footer>	<!-- pied de page -->
			
		 <div id="conteneur2">       
    
			<div class="element" id="bdd"><?php require'bdd.php';?></div> <!-- contiendra la gestion de la bdd -->
        
        </div>												

		</footer>
        
       
 

</body>
</html>


