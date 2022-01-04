# Bilan :

## 2.1 :
On constate que l'affichage prend très peu de temps (0.0085s environ) comparé au calcul (0.034s environ).
Au total, on passe en moyenne 0.053s par pas de calcul.

## 2.2 :
Au total, ici, on passe plutôt 0.034s par étape de calcul. On a un speedup de 1.55, ce qui s'explique par le fait que l'on puisse gérer en parallèle calcul et affichage.

## 2.3 :
Les résultats sont assez étonnants puisqu'à première vue, il n'y a aucun gain de vitesse.
Cela va à l'encontre de ce que j'imaginais, mais en réalité, comparé au temps pris par la simulation ou l'affichage, le bloquage dû à l'attente doit être négligeable:
En effet, vu le temps passé dans l'affichage (ordre de grandeur 10 fois plus faible que la simulation), si une simulation se termine, peu d'attente est à espérer.

## 2.4 :
La parallélisation des boucles contenant la fonction "estContaminé" n'est pas spécialement plus difficile que les autres, mais pour obtenir le même résultat qu'avant, il faut faire attention au fait que "estContaminé" fait appelle à un générateur de nombres pseudo-aléatoires dont l'ordre de traitement des individus fera changer la valeur.

En ce qui concerne le nombre d'individus:
- constant, on mesure un temps global moyen de 0.032s environ, ce qui constitue une légère accélération supplémentaire comparé aux versions précédentes, et une accélération absolue de 1.65.
- constant par thread, on obtient une complexité (quasiment) linéaire par rapport au nombre de threads. Ce qui signifie que l'optimisation est presque optimale : En effet, à 1 thread et une population de 100.000, le temps total par étape était de 0.032s. à 4 threads et une population de 400.000 gentilhommes, le temps total mesuré en moyenne est de l'ordre de 0.1312. Or 0.032\*100.000 / (0.1312\* 400.000) =~ 1.

## 2.5 :
Ayant manqué de temps, je ne propose pour cette question que mon brouillon. L'idée est de s'occuper de seulement une partie de la grille (Scatter, pour la séparer parmi les processus du sous-communicateur), que l'on regroupe avec Gather pour l'envoyer et l'actualiser, étant donné que l'envoi direct d'objets de la classe individu est difficilement faisable avec MPI.

## 2.5.1 :
à partir des résultats précédents, on peut combiner OpenMP pour la parallélisation des boucles "for", tandis que MPI aura divisé le problème. Une machine peut par exemple s'occuper de l'affichage tandis que d'autres machines, multicoeurs, traitent une seule partie de la simulation à chaque instant.

## 2.5.2 : Bilan :
J'ai trouvé, d'une manière générale, les résultats assez peu parlants car l'ordre de grandeur était relativement faible, comparé aux TPs en classe où tout pouvait se compter en secondes, parfois.

Le projet a été globalement difficile étant donné le moment auquel il est arrivé, s'ajoutant à une forte charge de travail externe, ce qui malheureusement m'a empêché de vraiment approfondir ce que je souhaitais.

J'ai trouvé la communication MPI au travers des classes C++ difficile, et très primitive. Je suppose (et j'espère) qu'il existe des bibliothèques s'en chargeant; surtout étant donné la possibilité d'utiliser MPI en C++, alors que les classes sont la base de la POO.