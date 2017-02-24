#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <math.h>
#define pi  3.14159265359
#define V 72
using namespace std;

//FONCTION DISTANCE
////////////////////////////////////////////////////////////////////////////////////////////
//Conversion en Rad
double toRad(double degree)
{
	return degree / 180 * pi;
}
//Calcule de la distance
double Calcul_Distance(double lat1, double long1, double lat2, double long2)
{
	double dist;
	dist = sin(toRad(lat1)) * sin(toRad(lat2)) + cos(toRad(lat1)) * cos(toRad(lat2)) * cos(toRad(long1 - long2));
	dist = acos(dist);
	dist = 6371 * dist;
	return dist;
}
////////////////////////////////////////////////////////////////////////////////////////////

//Creation d'une matrice diagonale symetrique contenant les distances entre chaque ville
void Matrice_Dist(double matrice_distance[72][72], double tab_coord[V+1][2])
{
	int i, j;

	for (i = 1; i < 72; i++)
	{
		for (j = 1; j < 72; j++)
		{
			{
				matrice_distance[i][j] = Calcul_Distance(tab_coord[i][0], tab_coord[i][1], tab_coord[j][0], tab_coord[j][1]);
				matrice_distance[j][i] = Calcul_Distance(tab_coord[i][0], tab_coord[i][1], tab_coord[j][0], tab_coord[j][1]);
				if (matrice_distance[i][j] < 100)
				{
					matrice_distance[i][j] = 0;
				}
				if (matrice_distance[j][i] < 100)
				{
					matrice_distance[j][i] = 0;
				}
			}
		}
	}
}

//Algorythme de Dijkstra afin de trouver le plus grand chemin en parcourant chaque ville une seulle fois
void Algo_Dijkstra(double matrice[V][V], int indice_ville_depart, string Tab_ville[V+1][1])
{
	int Tableau_Passage[72];

	for (int k = 1; k < 72; k++)
	{
		Tableau_Passage[k] = 0;
	}

	int j;
	int ville_depart = indice_ville_depart;
	int ville_courante = indice_ville_depart;
	int new_indice;
	double dist_total = 0;

	int somme_bool = 0;

	
	Tableau_Passage[indice_ville_depart]= 1;
	cout << Tab_ville[ville_depart][0] << " -> ";

	while (somme_bool < 70)
	{
		double dist_max = 0;
		
		for (j = 1; j < 72; j++)
		{
			if (Tableau_Passage[j] == 0)
			{
				if (dist_max <= matrice[ville_courante][j])
				{
					dist_max = matrice[ville_courante][j];
					new_indice = j;
				}
			}
		}

		Tableau_Passage[new_indice] += 1;
		ville_courante = new_indice;

		cout << Tab_ville[new_indice][0] << " -> ";
		
		dist_total += dist_max;
		somme_bool += 1;
	}

	dist_total += matrice[ville_courante][ville_depart];
	cout << Tab_ville[ville_depart][0] << endl;
	cout << endl;
	cout << "La distance totale parcouru pour se chemin est de  " << dist_total << " km"<< endl;
	cout << endl;
}

//Creation fichier.txt avec les differentes villes et leur distance entre elles
void Fichier_distance_ville(double matrice_dist[72][72], int tab_ville_id[V + 1][1])
{
	ofstream fichier("test.txt", ios::app);  // ouverture en écriture avec effacement du fichier ouvert

	if (fichier)
	{
		for (int i = 1; i < V; i++)
		{
			for (int j = 1; j < V; j++)
			{
				if (matrice_dist[i][j] != 0)
				{
					fichier << tab_ville_id[i][0] << " - " << tab_ville_id[j][0] << "  " << matrice_dist[i][j] << endl;
				}
			}
		}

	}
	else
	{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
}

int main()
{
	const char*filename = "D:\\GoogleDRIVE\\ESILV\\A4\\S7\\Advanced Data Structures & Algorithms\\TDs\\Cites.txt";
	ifstream fichier(filename);  // on ouvre le fichier en lecture
	string line;

	int linecount = 0;

	if (!fichier)
	{
		cout << "Failed to open ! \n";
		return 0;
	}

	//Création d'un tableau où seront stockés les valeurs
	int Tableau_villes_numero[V+1][1];
	string Tableau_villes[V+1][1];
	double Tableau_coord[V+1][2];

	//Lecture des données du fichier
	while (getline(fichier, line))
	{
		stringstream iss(line);
		string ville, latitude, longitude;

		getline(iss, ville, ',');
		getline(iss, latitude, ',');
		getline(iss, longitude, ',');

		//Converstion des données longitude et latitude de string -> double
		double d_long, d_lat;

		stringstream ilongitude(longitude);
		stringstream ilatitude(latitude);
		ilatitude >> d_lat;
		ilongitude >> d_long;

		//Affectation des valeurs dans 2 tableaux	
		if (linecount < V)
		{
			Tableau_villes_numero[linecount][0] = linecount;
			Tableau_villes[linecount][0] = ville;
			Tableau_coord[linecount][0] = d_lat;
			Tableau_coord[linecount][1] = d_long;
		}
		if (linecount == 0)
		{
			cout << ville << "	" << latitude << "  " << longitude << endl;
		}
		else
		{
			cout << Tableau_villes_numero[linecount][0] << " " << Tableau_villes[linecount][0] << "	" << Tableau_coord[linecount][0] << "  " << Tableau_coord[linecount][1] << endl;
		}
		linecount++;
	}


	//Creation d'une matrice diagonale symetrique contenant les distances entre chaque ville
  
	double Matrice_distance[72][72];
	Matrice_Dist(Matrice_distance, Tableau_coord);

	cout << endl; cout << endl;

	cout << "La ville de depart est PARIS " << endl;
	cout << "Taper 48 pour commencer de PARIS "<< endl;
	int id;
	cin >> id;

	cout << endl;

	Fichier_distance_ville(Matrice_distance, Tableau_villes_numero);

	Algo_Dijkstra(Matrice_distance, id, Tableau_villes);	cout << endl;


		for (int k = 0; k < 100; k++)
		{
			cout << k + 1 << "  ";
		}

	return 0;
}
