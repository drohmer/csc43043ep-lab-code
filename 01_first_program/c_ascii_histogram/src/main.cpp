#include <iostream>
#include <fstream>
#include <string>
#include <map>

// ============================================================================
// Exercice : Histogramme ASCII de mots
// ============================================================================



/**
 * Construit un histogramme de mots a partir d'un fichier texte.
 *
 * Chaque mot est compte dans une std::map.
 * Separateurs : espaces (lecture via >>).
 *
 * Etapes a suivre :
 *   1. Creer une variable std::map<std::string, int> pour stocker l'histogramme.
 *   2. Ouvrir le fichier avec std::ifstream.
 *   3. Verifier que le fichier est bien ouvert (if (!file) { ... }).
 *      En cas d'erreur, afficher un message sur std::cerr et retourner la map vide.
 *   4. Lire les mots un par un avec une boucle : while (file >> word) { ... }
 *      - L'operateur >> decoupe automatiquement par espaces/retours a la ligne.
 *   5. Pour chaque mot lu, incrementer son compteur dans la map :
 *      histogram[word]++
 *      - Si le mot n'existe pas encore, il est cree automatiquement avec la valeur 0
 *        puis incremente a 1.
 *   6. Retourner la map.
 */
std::map<std::string, int> build_word_histogram(const std::string& filename) {
    // TO DO
}

/**
 * Affiche un histogramme ASCII a partir d'une std::map.
 *
 * Format attendu pour chaque mot :
 *   mot | *****
 * ou le nombre de '*' correspond au nombre d'occurrences.
 *
 * Etapes a suivre :
 *   1. Parcourir la map avec une boucle range-based for :
 *      for (const auto& it : hist) { ... }
 *      - it.first  : la cle   (le mot,   type std::string)
 *      - it.second : la valeur (le compte, type int)
 *     Alternative: for(const auto& [word, count] : hist) { ... } 
 *   2. Pour chaque entree, afficher le mot suivi de " | ".
 *   3. Afficher autant de caracteres '*' que la valeur du compteur
 *      (boucle for classique).
 *   4. Terminer la ligne avec '\n'.
 *
 * Remarque : std::map trie automatiquement les cles par ordre alphabetique.
 */
void print_histogram(const std::map<std::string, int>& hist) {
    // TO DO
}


int main() {
    std::string filename = "src/input.txt";

    auto hist = build_word_histogram(filename);

    std::cout << "Histogramme:\n";
    print_histogram(hist);

    return 0;
}
