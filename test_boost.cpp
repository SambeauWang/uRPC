#include <vector>
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/assign.hpp>
#include <boost/timer.hpp>

using namespace std;
using namespace boost;

int main()
{
	timer  t;
	vector<int> v = (assign::list_of(1), 2, 3, 4, 5);
	//(assign::list_of(1)( 2)(3)( 4) (5));
	BOOST_FOREACH(int x, v)
	{
		cout << x << ",";
	}
	cout << endl;

	cout << t.elapsed() << "s" << endl;
	cout << "Hello world!" << endl;
	system("pause");
	return 0;
}