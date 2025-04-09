# Sujet de TP : jeu d’aventures dans un château

## De quoi parle-t-on ?

Ce TP est à réaliser par équipes de trois ou quatre étudiants.

Le but du TP est de réaliser un jeu d'aventures.
Le personnage du jeu peut se déplacer dans différentes salles d'un château qui comporte plusieurs étages.
On peut passer d'une salle à l'autre par des portes ou des escaliers.
Le personnage peut ramasser des objets, par exemples des armes, des clefs, qu’il peut mettre dans son sac.
Il peut rencontrer des personnages plus ou moins hostiles.
Le but est de découvrir un trésor.

Vous avez la possibilité d’utiliser une base de données, mais cela n’est pas obligatoire.
Vous pouvez, si vous le souhaitez, développer une interface graphique, mais vous pouvez vous contenter d’une interface textuelle.
Dans ce cas, l’architecture doit être prévue pour pouvoir ajouter facilement une interface graphique pour le jeu.

L’énoncé est volontairement peu détaillé, chaque équipe doit imaginer un cahier des charges plus complet.

Un incrément significatif du système devra être implémenté en Java ou C++.
Le but n’est pas d’obtenir un logiciel complet, mais d’en développer une partie qui pourrait correspondre à une première version à livrer au client.

## Construction

### Diagrammes UML

#### Dépendances

- [PlantUML](https://plantuml.com/fr/) dont le paquet Ubuntu est `plantuml`

#### Construction

Un _Makefile_ a été rédigé pour faciliter l'opération.
Il vous suffit donc d'appeler à la racine du projet

```console
make -C docs/uml
```

Les fichiers générés sont `docs/uml/*/*.png`.
