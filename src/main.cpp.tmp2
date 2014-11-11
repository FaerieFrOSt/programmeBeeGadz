#include <iostream>
#include "mysql_driver.h"
#include <ctime>
#include <string>
#include <exception>
#include "print_driver.h"
#include <vector>

using namespace std;

int	main()
{
	int nbmystere;
	srand(time(nullptr));
	cout << "Bienvenue." << endl << "1 - Jouer" << endl << "2 - Voir les scores" << endl;
	cin >> nbmystere;
	Mysql *m;
	try
	{
		m = new Mysql("127.0.0.1", "scores", "root", "vive-moi");
	}
	catch (exception &e)
	{
		cout << "mysql connection wasn't possible, exciting" << endl;
		return 1;
	}
	if (nbmystere == 1)
	{
		nbmystere = rand() % 100 + 1;
		int i = 1;
			int nb;
		do
		{
			cout << "nb ?" << endl;
			cin >> nb;
			if (nb > nbmystere)
				cout << "c'est moins." << endl;
			else if (nb < nbmystere)
				cout << "c'est plus." << endl;
			else
				cout << "bravo vous avez trouvé le nbmystere." << endl;
			++i;
		} while (nb != nbmystere);
		cout << "Vous avez fait un score de " << i << "." << endl;
		cout << "entrez votre pseudo : ";
		string pseudo;
		cin >> pseudo;
		if (!m->sendRequest(string("insert into scores values('', '") + pseudo + string("', '") + Printer::valueToString<int>(i) + string("');")))
			cout << "une erreur s'est produite lors de l'envoi de la requete" << endl;
	}else
	{
		cout << "liste des scores : " << endl;
		m->sendRequestData(string("select * from scores order by score"));
		vector<string>	tab;
		tab = m->getNextData();
		while (tab.size())
		{
			for (auto i : tab)
				cout << i << " ";
			cout << endl;
			tab = m->getNextData();
		}
	}
	return 0;
}
