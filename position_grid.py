import numpy as np

#Generates position 
x=np.arange(-20,20,1)
y=np.arange(-20,20,1)

fic = open("position.txt", "w")


k=2.0
units="cm"
for i in range(len(x)): # on crée une boucle pour assigner chacune des valeurs de x,y et t à une autre variable qui permettra d'afficher dans le format texte les variables nécessaires.
   for j in range(len(y)):
     # on assigne donc à une variable h la valeur de x[i], c'est-à-dire la valeur de x à la position 1, 2,3,4 jusqu'à ce que toute la longueur de x soit faites.  
    a=x[i]
    b=y[j]
    #fic.write("%i %5.2f\n" % (a[i], b[i]))
    fic.write ("/gun/position  ")
    fic.write('{:.2f} '.format(a))
    fic.write('{:.2f} '.format(b))
    fic.write('{:.2f} '.format(k))
    fic.write("cm"+'  \n') # ici on écrit notre nombre avec deux décimales (2f) en utilisant la fonction format et en faisant le champ de remplacement avec {} pour y insérer la variable b qui contient toutes les valeurs de t.
    fic.write("/run/beamOn 100"+'  \n')
    #fic.write('     {:.4f}'.format(h))# même chose ici sauf que l'on veut 4 décimales (4f)
    #fic.write( '      {:.4f}'.format(k)+'  \n')#Après avoir écrit nos 3 valeurs il faut faire un saut de ligne, c'est pourquoi il faut ajouter '\n'
  
fic.close() # pour s'assurer que notre programme fonctionne toujours corretement, il faut refermer le fichier créer à chaque fois que nous exécutons ce code (fonction fic.close).
