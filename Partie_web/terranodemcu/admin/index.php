<?php
// On prolonge la session
session_start();

  // Réinitialisation du tableau de session
  // Destruction du tableau de session
  unset($_SESSION['form']['limit']);
  unset($_SESSION['form']['graphe']);  

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
        <link rel="icon" type="image/png" href="img/serpent.png" />
        <script type="text/javascript" src="../lib/jquery.js"></script> <!-- appel de la librairie jquery --> 
        <script type="text/javascript" src="../lib/dateheure.js"></script> <!-- appel de la fonction date et heure javascript --> 
        <link rel="stylesheet" href="index.css" /> <!-- appel du thème de la page --> 
            
</head>

<body>	

        <header>    <!-- entête -->

        <div class="conteneur1">       
	 
			<div class="element" id="date"> <!-- contiendra la date -->
			
				<script type="text/javascript">window.onload = date('date');</script>
			
			</div>
						

			<div class="element" id="heure"> <!-- contiendra l'heure --> 
			
				<script type="text/javascript">window.onload = heure('heure');</script>	
			
			</div>  
			
		</div>     

        </header>
         
        
        
				<section>	<!-- corps de page -->	
				
				<div class="conteneur">                                                    <!-- on replace ici l'ip de votre nodemcu -->
					
					<div class="element"><h1>Flash du NodeMCU</h1><iframe id="update" src="http://192.168.XXX.XXX/firmware" ></iframe></div>	
					
					<div id="cf" class="element">
					 				  
					</div>
					
					<div class="element"><h1>Sauvegarde de la base de donnée</h1><div id="bddadmin"> <?php require "bddadmin.php" ?></div></div>						
					
				</div>      				
				
				</section>
        
        
        
        <footer>	<!-- pied de page -->
			
		 <div class="conteneur3">       
			 
			<div class="element" id="serpent">
                <a href="../histo.php" style="text-decoration:none"><span id="histo">Historique</span></a> <!--lien vers la page historique-->
            </div> 
            
            <div class="element" id="logout">            
                <a href="logout.php" style="text-decoration:none"><img src="../img/serpent.gif" alt="serpent.gif">Déconnexion</a><!--lien vers la page logout-->
            </div>     
            
            
			<div class="element" id="serpent">
                <a href="../index.php" style="text-decoration:none"><span id="accueil">Accueil</span></a><!--lien vers la page admin-->
            </div>     
            
		</div> 

											

		</footer>
        
       
 

</body>
</html>
