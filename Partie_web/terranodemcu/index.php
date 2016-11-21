<!DOCTYPE html> 
<head> 
		<meta charset="utf-8" /> 
        <title>Terrarium</title> <!-- titre -->
        <link rel="icon" type="image/png" href="img/serpent.png" />
        <script type="text/javascript" src="lib/jquery.js"></script> <!-- appel de la librairie jquery --> 
        <script src="lib/RGraph.common.core.js"></script> <!-- appel de la librairie Rgraph --> 
        <script src="lib/RGraph.common.csv.js"></script> <!-- appel de la librairie Rgraph --> 
        <script src="lib/RGraph.gauge.js"></script> <!-- appel de la librairie Rgraph --> 
        <script src="lib/RGraph.common.dynamic"></script> <!-- appel de la librairie Rgraph --> 
        <script type="text/javascript" src="lib/dateheure.js"></script> <!-- appel de la fonction date et heure javascript --> 
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

        <div class="conteneur">        
			<div class="element" id="date"><?php require'date.php';?></div> <!-- contiendra la date -->
    
			<div class="element" id="heure"><?php require'heure.php';?></div> <!-- contiendra l'heure -->  
        </div>      

        </header>
        
        
        
				<section>	<!-- corps de page -->	
				
				<div class="conteneur">       
			
					<div class="element" id="gauge"><?php require'jauge.php';?></div> <!-- contiendra les jauges -->
				
				</div>      
				
				</section>
        
        
        
         <footer>	<!-- pied de page -->
			
		 <div class="conteneur">       
			 
			<div class="element" id="serpent">
                <a href="graphemulti.php" style="text-decoration:none"><span id="histo">Historique</span></a> <!--lien vers la page historique-->
            </div> 
    
			
			<div class="element" id="bdd"><?php require'bdd.php';?></div> <!-- contiendra la gestion de la bdd -->		
			

			<div class="element" id="pc">
                <a href="admin/index.php" style="text-decoration:none"><span id="admin">Admin</span></a><!--lien vers la page admin-->
            </div>            
            			
        </div>												

		</footer>
       
 

</body>
</html>


