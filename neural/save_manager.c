#include <stdlib.h>
#include <stdio.h>

// sauvegarde les poids dans le fichier weights_cache

// weights_cache contient un flottant sur chaque ligne (au format 2.5f)
// lors du chargement il faut specifier le nombre de neuronnes a charger

// a l'exterieur du fichier cache les poids sont dans un tableau de flottants


void load(float weights[], size_t nb_neurons)
{
  FILE *save = NULL;
  save = fopen("weights_cache", "r");
  if (save != NULL)
  {
    size_t i = 0;
    char line[nb_neurons];
    while(i < nb_neurons)
    {
      fgets(line, 7, save);
      weights[i] = strtof(line, NULL);
    }
    fclose(save);
  }
  // ici le fichier n a pas pu etre charge
}


void save(float weights[], size_t nb_neurons)
{
  FILE *save = NULL;
  save = fopen("weights_cache", "w");
  for(size_t i = 0; i < nb_neurons; ++i)
    fprintf(save, "%2.5f\n", weights[i]);
  fclose(save);
}
