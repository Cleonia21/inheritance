#include <iostream>
#include <string.h>
#include <math.h>
#include <fstream>

using namespace std;

/*
	По заданию классы TAObject и TLA абстрактные. В плюсах, по определению, абстрактный класс
	это такой, который содержит хотя бы одну виртуальную функцию без определения.
	Это заставит наследников эту виртуальную функцию Обязательно переопределить и
	запретит создание экземпляров этого абстрактного класса. 

	Выглядит виртуальная функция без определения вот так:
	virtual void functionName( void ) = 0;

	Соответственно классы TAObject и TLA не абстрактные. Потому что какую функцию сделать
	виртуальной + без определения не указанно. Темболее ниже в одном из классов создается
	Массив экземпляров класса TLA, а сделать это будет не возвожно, если TLA будет абстрактным.

	Полупрограмистам, которые это задание составляли, нужно вспомнить терминологию ;))))
*/

class TAObject
{

public:
	double _x, _y;
	bool _f;

	// В задании нет этого конструктора, но он нужен что бы создавать массив экземпляров
	TAObject( void )
	{
		std::cout << "Default constructor from TAObject colled" << std::endl;
	}

	TAObject(double x, double y)
	{
		std::cout << "Param constructor from TAObject colled" << std::endl;
		_x = x;
		_y = y;
		_f = false;
	}

	~TAObject()
	{
		std::cout << "Destructor from TAObject colled" << std::endl;
	}

};

class TLA : public TAObject //Вот здесь указывается, что TLA наследуется от TAObject
{

//Это ключевое слово означает, что наследники получат доступ к коду ниже, но через экземпляр класса доступа нет
protected:
	double _xc, _yc, _R, _fi, _V;
//Это ключевое слово: доступно всем))
public:
	bool _landing;

	// В задании нет этого конструктора, но он нужен что бы создавать массив экземпляров
	TLA( void )
	{
		std::cout << "Default constructor from TLA colled" << std::endl;
	}

	//При наследовании из родителя в потомка передается все кроме конструкторов
	//Поэтому нужно заново в каждом наследнике писать конструктор и из него
	//вызывать родительский
	TLA(double x, double y, double V, double xc, double yc) //Сразу после двоеточия вызывается нужный нам конструктор родителя, 
		: TAObject(x, y), _xc(xc), _yc(yc), _V(V) 			//там проинициализируются переменные _x и _y, потом через запятую
	{														//инициализируются поля конкретно этого класса
		std::cout << "Param constructor from TLA colled" << std::endl;
		_landing = false; //Самолет не преземлился
		_f = true; 	// не совсем понимаю как эта переменная должна работать. В задании это:
					// "признак выполнения посадки", в каком месте этот признак должен меняться
					// true -> false/false -> true, я хз.
		_R = sqrt(pow(_x - _xc, 2) + pow(_y - _yc, 2)); //sqrt - корень, pow - степень

		double b = 0;
		if (_x < _xc && _y < _yc)
			b = -1;
		if (_x < _xc && _y > _yc)
			b = 1;
		_fi = b * M_PI + atan((_y - _yc) / (_x - _xc)); //atan - арктангенс
	}

	virtual void move(double t, int a) 	//ключевое слово virtual указывает, что хорошо бы наследникам этот метод переопределить,
	{									//но если это не сделать, то ничего не случится и через экземпляр наследника сработает эта реализация
		std::cout << "Move from TLA colled" << std::endl;
		if (a == 1)
			_f = true;
		if (a == 0)
			_f = false;
	}

	//Эта функция нужна что бы определить тип.
	//В вычислениях ниже нужно знать кто вертолет, а кто самолет.
	//Можно другими способами это определять, но это самый простой
	virtual string getType( void ) { return (""); }

};

/*
	Эта функция для того что бы вывести вразумительный вывод вот такой операцией:
		std::cout << *_PLA[i] << std::endl;	

	Использовал, когда тестил, нужно было видеть как меняются переменные после вызова разных методов
*/
std::ostream	&operator<<( std::ostream &ostr, TLA &instance )
{
	ostr	<< "landing = " << instance._landing << std::endl
			<< "f = " << instance._f << std::endl
			<< "x = " << instance._x << std::endl
			<< "y = " << instance._y;
	return (ostr);
}

class TAircraft : public TLA
{

public:

	TAircraft( void )
	{
		std::cout << "Default constructor from TAircraft colled" << std::endl;
	}

	TAircraft(double x, double y, double V, double xc, double yc)
		:TLA(x, y, V, xc, yc)
	{
		std::cout << "Param constructor from TAircraft colled" << std::endl;
	}

	string getType( void )
	{
		return ("aircraft");
	}

	virtual void move(double t, int a)
	{
		std::cout << "Move from TAircraft colled" << std::endl;
		double w = - (_V / _R);
		// В конце не '1', а 'dt'. Где взять эту 'dt' Я хуй его знает 
		this->_x = - (_xc + _R * cos(_fi + w * t)) * (a - 1) + a * (_x + _V * 1);
		this->_y = - (_yc + _R * sin(_fi + w * t)) * (a - 1) + a * _y;
	}

};

class THelicopter : public TLA
{

public:

	THelicopter( void )
	{
		std::cout << "Default constructor from THelicopter colled" << std::endl;
	}

	THelicopter(double x, double y, double V, double xc, double yc)
		:TLA(x, y, V, xc, yc)
	{
		std::cout << "Param constructor from THelicopter colled" << std::endl;
	}

	string getType( void )
	{
		return ("helicopter");
	}

	virtual void move(double t, int a)
	{
		std::cout << "Move from THelicopter colled" << std::endl;
		// Та же хуйня с dt
		_x = _x - a * _V * cos(_fi) * 1;
		_y = _y - a * _V * sin(_fi) * 1;
	}
};

class TAirport : public TAObject
{
	const int _dt;
	double _l;
	int _landedNum;//количество приземлившихся самолетов
	int _plaNum;   //количество элементов в массиве на данный момент
	TLA **_PLA;    //не до конца понял какой нужен массив - одномерный или двумерный

public:
	TAirport(double x, double y, double l)
		: TAObject(x, y), _l(l), _dt(1)
	{
		std::cout << "Default constructor from TAirport colled" << std::endl;
		_landedNum = 0;
		_plaNum = 0;
		_PLA = NULL;
	}

	void Do(double t0, double tk)
	{
		std::cout << "Do from TAirport colled" << std::endl;
		int a;

		std::ofstream	fout("1"); //для записи в файл
		if (!fout)
		{
			std::cout << "TAirport::Do - file open error";
			return ;
		}

		for(int t = t0; t < tk; t += _dt)
		{
			for (int i = 0; i < _plaNum; i++)
			{
				// std::cout << *_PLA[i] << std::endl;
				if (_PLA[i]->_landing)
				{
					std::cout << "PLA " << i << " already landed" << std::endl;
					continue ;
				}
				if (_PLA[i]->_f && !this->_f)
				{
					std::cout << "PLA " << i << " won`t move" << std::endl;
					double X = _x + 1.1 * _l;
					a = 0;
					if (_PLA[i]->getType() == "aircraft" && !(_f && !_PLA[i]->_f) 		// пункт 1.6
							&& (_PLA[i]->_x < X) && (abs(_PLA[i]->_y - _y) < _l / 50))
						a = 1;
					if (_PLA[i]->getType() == "helicopter" && !(_f) && !_PLA[i]->_f)	// пункт 1.7
						a = 1;
					_PLA[i]->move(t, a);
				}
				if (_PLA[i]->getType() == "aircraft"									// пункт 1.8
						&& (_PLA[i]->_x > (_x + _l) && _PLA[i]->_f) && !_f)
					_PLA[i]->_landing = true;
				if (_PLA[i]->getType() == "helicopter"									// пункт 1.9
						&& (pow(_PLA[i]->_x - _x, 2) + pow(_PLA[i]->_y - _y, 2)
						< pow(_l / 50, 2)) && _PLA[i]->_f)
					_PLA[i]->_landing = true;

				if (_PLA[i]->_landing) // внутри то что необходимо записать в файл в первом варианте
				{
					std::cout << "PLA " << i << " was landed" << std::endl;
					fout	<< "TLA " << i << " was landing and has serial number "
							<< _landedNum << std::endl;
					_landedNum++;
				}
				else
					std::cout << "PLA " << i << " don`t landed" << std::endl;
			}
		}
		fout.close();
	}

	//Вот здесь есть не большая бредовость задания. Нет объяснения как заполнять массив _PLA,
	//а не заполнив его нельзя нормально протестировать работу функции Do. Костыльная реализация,
	//но для теста пойдет.
	void addTLA(TLA *newTLA)
	{
		// Просто создаем новый массив экземпляров TLA, переписываем из старого все указатели в новый
		// и освобождаем старый. Плюс дописываем в конец новый экземпляр транспорта 
		_plaNum++;
		TLA **newPLA = new TLA* [_plaNum];
		for (int i = 0; i < _plaNum - 1; i++)
			newPLA[i] = _PLA[i];
		delete [] _PLA;
		newPLA[_plaNum - 1] = newTLA;
		_PLA = newPLA;
	}

	// Освобождаем в деструкторе память которую выделяли...
	~TAirport()
	{
		std::cout << "Destructor from TAirport colled" << std::endl;
		delete [] _PLA;
	}
};

int main( void )
{
	THelicopter helicopter(1., 1., 90., 5., 7.);
	TAircraft aircraft(2., 2., 900., 42., 54.);
	TAirport airport(5., 5., 300.);

	airport.addTLA(&helicopter);
	airport.addTLA(&aircraft);

	std::cout << "||||||||||" << std::endl;
	airport.Do(0., 1.);
	airport.Do(0., 1.);
	std::cout << "||||||||||" << std::endl;

	return (0);
}
