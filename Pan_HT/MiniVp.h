// include once only
#pragma once

// #include <Arduino.h> // Utile pour utiliser les fonctions Arduino
// #include les libs dont tu as besoin


class MiniVp
{
public:
	MiniVp(int a);
	~MiniVp();
	int coucouCestNous();
	void setB(int cestnous);
	int a;
	int b;
private:
	bool c;
};

