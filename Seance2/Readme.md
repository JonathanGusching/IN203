##Séance 2
#Interblocage:
Dans la situation où 1 envoie à 2 en premier, il y a interblocage entre 0 et 2 qui s'envoient mutuellement des données de manière synchrone.
Si 0 envoie son message en premier, il n'y a en revanche aucun problème.
Il y a donc 50% de risque d'un interblocage.

#Question 2:
La proportion de séquentiel est 52% du temps de calcul total.
La loi d'Amdahl nous donne une accélération environ égale à 2.08 pour n grand.
Par croissance, c'est le maximum.
Pour 8 noeuds, ce qui est assez standard, on est déjà à 1.83 d'accélération.
Pour 16 noeuds, on obtient 1.95. 8 ou 16 noeuds semblent suffisants comparés au maximum que l'on peut obtenir. Cela coûterait trop cher pour peu de bénéfices de dépasser 16 noeuds.

On trouve, on résolvant le système, que l'accélération maximale est 7.


#Mandelbrot:
Le temps d'exécution en parallélisant équitablement est environ divisé par 5 (en revanche, deux taches prennent 4s alors que deux autres prennent 6s à cause des calculs de convergence qui sont différents selon les endroits de l'image, contre 20s sans parallélisme)

en maître-esclave, on est un peu plus lent (7s) mais c'est uniformément réparti sur tous les processus, donc mieux optimisé.

