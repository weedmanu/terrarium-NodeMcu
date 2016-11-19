<!DOCTYPE html> 
<head> 
		<meta charset="utf-8" /> 
        <title>Terrarium</title> <!-- titre -->
        <script type="text/javascript" src="lib/jquery.js"></script> <!-- appel de la librairie jquery --> 
        <script type="text/javascript" src="lib/dateheure.js"></script> <!-- appel de la fonction date et heure javascript --> 
        <script src="lib/highcharts.js"></script> <!-- appel de la librairie highcharts --> 
        <script src="lib/graphhisto.js"></script> <!-- appel du graphique highcharts -->
        <script src="lib/gray.js"></script> <!-- appel du thème highcharts --> 
        <link rel="stylesheet" href="histo.css" /> <!-- appel du thème de la page --> 
</head>

<body>

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


