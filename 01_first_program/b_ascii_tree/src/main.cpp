#include <iostream>
#include <string>
#include <fstream>


/**
 * print_tree(int levels)
 *
 * Description:
 *   Affiche sur la sortie standard un sapin ASCII centré composé
 *   d'un feuillage en étoiles et d'un tronc en barres verticales.
 *
 * Paramètres:
 *   - `levels` : nombre de lignes de feuillage (doit être un entier positif).
 *
 * Comportement attendu:
 *   - Produit `levels` lignes de feuillage ; la i-ème ligne (0-based)
 *     contient `2*i + 1` étoiles ('*'), centrées sur la largeur maximale
 *     `2*levels - 1`.
 *   - Après le feuillage, affiche une ligne de tronc composée de trois
 *     barres verticales ("|||") centrée sur la même largeur. Si la largeur
 *     maximale est inférieure à 3, le tronc utilise la largeur disponible.
 *   - Chaque ligne est terminée par un saut de ligne.
 *
 * Contraintes de mise en œuvre:
 *   - Utiliser des boucles (`for`/`while`) pour construire chaque ligne.
 *   - Calculer l'indentation pour centrer chaque ligne en se basant sur la
 *     largeur maximale.
 *
 * Exemple pour `levels = 4` ,   `levels = 5` :
 * 
 *    *                                *
 *   ***                              ***
 *  *****                            *****
 * *******                          *******
 *   |||                           *********
 *                                    |||
 *
 */
void print_tree(int levels) {
    // TO DO: implement the function to print the tree
    // ...
}
int main() {
    std::cout << "Hello, World!" << std::endl;
    print_tree(4);
return 0;
}