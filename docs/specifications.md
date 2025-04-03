# Cahier des charges

## Présentation du problème

Le personnage du jeu peut se déplacer dans différentes salles d'un château qui comporte plusieurs étages.
On peut passer d'une salle à l'autre par des portes ou des escaliers.
Le personnage peut ramasser des objets, par exemples des armes, des clefs, qu'il peut mettre dans son sac.
Il peut rencontrer des personnages plus ou moins hostiles.
Le but est de découvrir un trésor.

## Technologies utilisées

- **Langage principal** : C++
- **Moteur de jeu** : Godot Engine (utilisation de `GDExtension` pour le code en C++)
- **Système de gestion des sauvegardes** : Base de données SQLite (éventuellement) ou fichier JSON (**optionnel**)
- **Système de gestion des niveaux** : Fichier JSON (**optionnel**)
- **Gestion des entrées** : Clavier et souris (ou manette)

## Architecture du jeu

### Modèle objet

- `GameManager` : Gère l'état global du jeu

- `Entity` : Représente une entité (inventaire, actions)
  - `NPC` : Représente un P.N.J.
    - `Passive` : Représente un P.N.J. passif

    - `Hostile` : Représente un P.N.J. hostile
      - différents types d'ennemis

  - `Player` : Représente le joueur

- `Room` : Contient les informations d'une salle (grille de tuiles)

- `Item` : Représente un objet pouvant être ramassé ou utilisé
  - `Weapon` : Représente une arme
  - `Consumable` : Représentable un consommable
  - `KeyItem` : un _item_ essentiel à la progression du jeu

- `Floor` : Représente un étage, soit un ensemble de salles
- `Castle` : Représente un ensemble d'étages
- `Tile` : une tuile (un décor et un _item_ sur la tuile)
- `Save` : une sauvegarde (l'état des entités et le château associé)

## Fonctionnalités principales

### Déplacement et interactions

- Déplacement en 2D (top-down) avec détection de collision

- Interaction avec les objets
  - Utiliser l'objet
  - Le récupérer si applicable

- Interaction avec les P.N.J.
  - Attaquer
  - Discuter

- Gestion des transitions entre les salles via des portes et escaliers

### Gestion des objets et de l'inventaire

- Système d'inventaire avec un nombre limité de _slots_
- Possibilité d'utiliser ou de jeter des objets
- Objets interactifs : clefs, potions
- Objets équipables : armes (un _slot_ particulier qui correspond à la main)

### Système de combat

- Attaques de base (corps-à-corps) avec prise en compte de l'arme équipée
- Système de points de vie et dégâts (possibilité d'utiliser des potions pour regénérer sa vie)
- IA ennemie avec détection, poursuite et attaque dès que possible

## Système de rendu et UI

- Affichage en 2D avec sprites, tuiles et annimations
- Interface minimaliste : inventaire, barre de vie

## Sauvegarde et chargement

- Sauvegarde de la progression dans un fichier
- Chargement depuis un fichier de sauvegarde

## Contraintes de développement

- **Modularité** : Séparer logique métier (C++) et interface (Godot)
- **Performance** : Optimisation du rendu et des collisions
- **Extensibilité** : Faciliter l'ajout de nouvelles salles, objets et ennemis
