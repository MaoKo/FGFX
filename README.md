# **FGFX - Générateur de Lexer et Parser Automatisé**

---

## **Introduction**
**FGFX** est un outil minimaliste et performant qui automatise la génération d'analyseurs lexicaux (**lexers**) et syntaxiques (**parsers**) à partir d'expressions régulières et de grammaires définies.  

Conçu pour les développeurs travaillant sur des compilateurs, interpréteurs ou autres outils basés sur l'analyse syntaxique, **FGFX** simplifie considérablement le processus de création et d'intégration d'analyseurs personnalisés.

---

## **Fonctionnalités**
- 📜 **Génération de lexers :**  
  - À partir d'expressions régulières pour analyser et catégoriser les tokens d'un langage.
- 🔧 **Génération de parsers :**  
  - Basé sur des grammaires formelles pour construire des arbres syntaxiques.  
  - Supporte les grammaires sans ambiguïtés.
- ⚡ **Rapide et modulaire :**  
  - Implémenté en **C**, garantissant des performances élevées.  
  - Interface simple et flexible, adaptée aux projets existants.

---

## **Structure du Projet**
- `lexer/` : Contient les outils et fonctions pour la génération d'analyseurs lexicaux.  
- `parser/` : Fournit les structures et algorithmes pour créer et manipuler des analyseurs syntaxiques.  
- `examples/` : Exemples illustrant l'utilisation de FGFX pour générer des lexers et parsers.  
- `docs/` : Documentation détaillée sur les fonctionnalités et l'architecture.

---

## **Prérequis**
- Un compilateur **C** (GCC ou Clang recommandé).  
- **Make** pour automatiser la compilation.  

---

## *TODO* ##
-   More accurate handle of error in FGFX
-   Optimizing FOLLOW set
-   Handle if file already exist
-   Improve the algorithm for constructing the
-  LALR parsing table
-  DFA transition table
-   Add error recovery mechanism
-   Add group element ( ... | ... ) in FGFP
-   Remove unreachable state
