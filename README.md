<p align="center">
    <img alt="MDEA logo" src="https://gitlab.com/Ynigvi/mdea/raw/master/doc/logo.png" />
</p>

# Génération de modèles diversifiés à l'aide d'algorithme génétiques

Ce projet est un projet de recherche réalisé au Laboratoire d'Informatique, de Robotique et Microélectronique de Montpellier (UMR CNRS et Université de Montpellier). Un résumé est donné ci-dessous.

# Résumé/Abstract

## Français
La génération de modèles à partir de méta-modèles est une solution apportée par l'ingénierie des modèles pour tester les transformations de modèles ou pour la validation de méta-modèles. Il existe dans la littérature un certain nombre de techniques combinatoires pour l'instanciation de méta-modèle, mais peu se sont intéressées au problème de la diversité. L'utilisation des algorithmes génétiques est une des plus prometteuses en terme de diversité. Durant nos travaux, nous avons expérimentés deux approches, basées sur l'algorithme évolutionnaire NSGA-II, pour la génération de nouveaux modèles, plus diversifiés. Les résultats ainsi obtenus mettre en évidence une réelle augmentation de la distance entre modèles, permettant de fournir des bancs d'essais plus réalistes et plus complets que ceux initiaux.

## English

Generation of models that conform to a meta-model is one of the solutions from model driven engineering for models transformations testing or meta-models validation. Some combinatorial techniques have been explored in previous works, but few have focused on problem of diversity. Amongst previous works, genetic algorithms usage seems to be one of the most promising for diversity generation. In this work, we experiment two approach, based on the evolutionnary algorithm NSGA-II, to generate more diverse models. These approaches provide results with an increased distance, enabling creation of more realistic and complete benchmark.

# Spécification

L'objectif de l'application MDEA (Model-Driven Engineering using Evolutionary Algorithms) est de prendre en entrée un certain nombre de modèles préexistants et d'en générer des nouveaux, plus diversifiés.

Syntaxe :
```
mdea -in <models directory>
     [-cx <inter|intra>]
     [-dist <cosine|hamming|centrality>]
     [-fitness <min|avg|minavg|minavgs|dist>]
     [-g <number of generations>]
     [-m <percentage of mutation chance>]
     [-nb <number of model in a single chromosom>]
     [-out <output directory>]
```

## Format des fichiers

Les fichiers de définition de modèles doivent être définis en suivant la syntaxe suivante :
```
Vecteur (entiers séparés par des espaces)
Emplacement du fichier XCSP
Emplacement du fichier .grimm
Emplacement du méta-modèle
Racine du méta-modèle
```

Exemple :
```
1 2 2 2 3 3 3 1 1 1 1 1 1 1 1 1 1 2 34 34 34 34 1 2 34 34 34 34 1 2 34 34 34 34 1 2 34 34 34 1 2 34 34 34 1 1 11 3 1 1 11 3 1 1 11 3 1 1 11 3 1 1 12 3 1 1 11 3 1 1 11 3 1 1 11 3 1 1 12 3 1 1 12 3 1 21 31 1 22 32 1 23 33 1 24 34 1 25 34 1 11 1 12 1 13 
javasmall/c0.xml
ProjectSmall.grimm
MyJava.ecore
Project
```

