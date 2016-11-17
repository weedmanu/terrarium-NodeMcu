<?php
// On prolonge la session
session_start();
// On teste si la variable de session existe et contient une valeur
if(empty($_SESSION['login'])) 
{
  // Si inexistante ou nulle, on redirige vers le formulaire de login
  header('Location: auth.php');
  exit();
}
?>

<!DOCTYPE html> 
<head> 
		<meta charset="utf-8" /> 
        <title>Terrarium</title> <!-- titre -->
        <script type="text/javascript" src="lib/jquery.js"></script> <!-- appel de la librairie jquery --> 
        <script type="text/javascript" src="lib/dateheure.js"></script> <!-- appel de la fonction date et heure javascript --> 
        <link rel="stylesheet" href="index.css" /> <!-- appel du thème de la page --> 
            
</head>

<body>

        <header>    <!-- entête -->

        <div class="conteneur">        
			<div class="element" id="date"><?php require'date.php';?></div> <!-- contiendra la date -->
    
			<div class="element" id="heure"><?php require'heure.php';?></div> <!-- contiendra l'heure -->  
        </div>      

        </header>
         
        
        
				<section>	<!-- corps de page -->	
				
				<div class="conteneur2">  
					
					<div class="element3"><iframe id="update" src="http://192.168.0.30/update" ></iframe></div>					
					
				</div>      				
				
				</section>
        
        
        
        <footer>	<!-- pied de page -->
			
		 <div class="conteneur">       
			 
			<div class="element" id="serpent">
                <a href="histo.php" style="text-decoration:none"><span id="histo">Historique</span></a> <!--lien vers la page historique-->
            </div> 
            
			<div class="element" id="serpent">
                <a href="index.php" style="text-decoration:none"><span id="accueil">Accueil</span></a><!--lien vers la page admin-->
            </div>     
            
		</div> 
		
		<div class="conteneur">  
		
			<div class="element" id="bdd"><?php require'bdd.php';?></div> <!-- contiendra la gestion de la bdd -->		
		
		</div>			
		
		<div class="conteneur">  
    
            <div class="element" id="pc">
                <a href="logout.php" style="text-decoration:none"><span id="logout">Déconnexion</span></a><!--lien vers la page admin-->
            </div>         
            			
        </div>												

		</footer>
        
       
 

</body>
</html>
