/****************************************************************************/
/**
* Copyright (C) ib-krajewski.de 2007. All Rights Reserved. Confidential.
*
* Distribution only to people who need to know this information in
* order to do their job.(Need-to-know principle).
* Distribution to persons outside the company, only if these persons
* signed a non-disclosure agreement.
* Electronic transmission, e.g. via electronic mail, must be made in
* encrypted form.
*
* @file /.../lambda-test.cpp
* @version /main/....
* @author Marek Krajewski
* @date ....
*
* This file contains tests of a simple lambda expr. library for use with
* the STL library functions.
*
****************************************************************************/

#include "lambda.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>

using namespace kmx;
using namespace std;


// test helpers

struct XXX 
{ 
   int value;  
   XXX(int v) : value(v) {};
   bool isSeven() { return value == 7; }; 
   int getVal() { return value; };
};

double sinus(int val)
{
   return sin(val);
}

void pr_sinus(int val)
{
   cout << "sin(" << val << ")=" << sin(val) << "\n";
}

int g_counter = 0;

void countVec(char label, double target, int val)
{
   cout << "countVec: " << label << " - " << val << "\n";
   if (val == target)
   {
      g_counter++;
   }
}

void countVec1(int val, double target, char label)
{
   cout << "countVec1: " << label << " - " << val << "\n";
   if (val == target)
   {
      g_counter++;
   }
}

void countVec_short(char label, int val)
{
   countVec(label, 1, val);
}

void countVec_Xshort(int val)
{
   countVec('Z', 3, val);
}


// tests

int main()
{
   // 1. currying: bind funct args, _$1 is the unbound one!
   // ---
   vector<int> vec;
   vector<int>::iterator iter;

   vec.push_back(1);
   vec.push_back(3);

   for_each(vec.begin(), vec.end(), bind(countVec, 'X', 1.222, _$1));
   for_each(vec.begin(), vec.end(), bind(countVec1, _$1, 1.222, 'Y'));
   for_each(vec.begin(), vec.end(), bind(countVec_short, 'X', _$1));
   for_each(vec.begin(), vec.end(), bind(countVec_Xshort, _$1));
   for_each(vec.begin(), vec.end(), bind(sinus, _$1));
   for_each(vec.begin(), vec.end(), bind(pr_sinus, _$1 * 2));
   
   const float pi = 3.14;
   for_each(vec.begin(), vec.end(), /* - */ bind(sinus, _$1 * (pi / 180.0)));   
   // TODO --- negate (operator-())!!

   cout << " counter --> " << g_counter << endl;

   // 2. vector of integers
   // ---
   iter = find_if(vec.begin(), vec.end(), _$1 <= 2);
   cout << " find <=2 ---> " << *iter << endl;
   
   iter = find_if(vec.begin(), vec.end(), _$1 == 1);
   cout << " find == 1 ---> " << *iter << endl;
   
   vec.push_back(13);
   vec.push_back(12);
   vec.push_back(11);
   
   sort(vec.begin(), vec.end(), _$1 <= _$2);   
   for_each(vec.begin(), vec.end(), cout << delay(" -- ") << _$1);
   cout << endl;
   for_each(vec.begin(), vec.end(), _$1++);
   for_each(vec.begin(), vec.end(), cout << " -- " << _$1 * 2 << delay("."));
   cout << endl;
   
   sort(vec.begin(), vec.end(), _$1 >= _$2);
   for_each(vec.begin(), vec.end(), cout << delay(" -- ") << _$1);
   cout << endl;

   // -- init hack:
   vector<int> v10(10);
   for_each(v10.begin(), v10.end(), _$1 = 7);
   cout << "v10:" << endl;
   for_each(v10.begin(), v10.end(),
      cout << delay("--- ") << _$1 << delay("\n")); //<< endl);

   // OPEN TODO ---
   // for_each(v10.begin(), v10.end(), _$1 = bind(&mul2, _$1));

   // 3. vector of pointers to int
   // ---
   vector<int*> vp(10);
   transform(v10.begin(), v10.end(), vp.begin(), &_$1); // store ptrs to v10!!

   vector<int*>::iterator iterp;
   vp.push_back(new int(1));
   
   iterp = find_if(vp.begin(), vp.end(), *_$1 == 1);
   cout << " find *== 1 ---> " << *(*iterp) << endl;
   
   sort(vp.begin(), vp.end(), *_$1 <= *_$2);
   for_each(vp.begin(), vp.end(), cout << " vp --> " << *_$1 << delay(", "));
   cout << endl;

   // 4. vector of pointers to obj.
   // ---
   vector<XXX*> vecx;
   vector<XXX*>::iterator iterx;
   vecx.push_back(new XXX(1));
   vecx.push_back(new XXX(2));

   // access values: fields directly OR by getters
   for_each(vecx.begin(), vecx.end(), cout << delay(" .. ") << _$1->*(&XXX::value));
   cout << " || ";
   
   for_each(vecx.begin(), vecx.end(), cout << delay(" .. ") << (_$1->*(&XXX::getVal))); //*5);
   cout << endl;
   
   // CANNOT DO: find_if(vecx.begin(), vecx.end(), _$1->value == 1);
   // BUT:
   iterx = find_if(vecx.begin(), vecx.end(), _$1->*(&XXX::value) == 2);
   cout << " find_if _$1->*(XXX:value) == 2 ---> " << (*iterx)->value << endl;
   iterx = find_if(vecx.begin(), vecx.end(), _$1->*(&XXX::value) == 3);
   cout << " find_if _$1->*(XXX:value) == 3 ---> " << (*iterx)->value << endl;
   iterx = find_if(vecx.begin(), vecx.end(), _$1->*(&XXX::getVal) == 1);
   cout << " find_if _$1->*(XXX:getVal) == 1 ---> " << (*iterx)->value << endl;
   
   // shouldn't clash with - lambda: *_$1 <= *$2 !!!
   iterx = find_if(vecx.begin(), vecx.end(), _$1->*(&XXX::getVal) <= 2);
   cout << " find_if _$1->*(XXX:getVal) <= 2 ---> " << (*iterx)->value << endl;
   
   // read field values from vecx
   vector<int> v10_1(10);
   transform(vecx.begin(), vecx.end(), v10_1.begin(), _$1->*(&XXX::getVal));
   
   // --> cout...
   // use globvar for counting...
   int xxxx = 0;
   for_each(vecx.begin(), vecx.end(), globvar(xxxx) += 5);
   cout << "globvar(xxx) += 5: " << xxxx << endl;
   
   xxxx = 0;
   for_each(vecx.begin(), vecx.end(), globvar(xxxx)++);
   cout << "globvar(xxx)++: " << xxxx << endl;
   
   int aaa = 0;
   for_each(vecx.begin(), vecx.end(), globvar(aaa) = 1);
   cout << " globvar(aaa) = 1: " << aaa << endl;
   
   // TODO --> assign form lambda_expr... SEE comment in AssgnTo
   //for_each(vecx.begin(), vecx.end(), globvar(aaa) = _$1->*(&XXX::value));

   int yyy_ctr = 0;
   vec.push_back(1);

   for_each(vec.begin(), vec.end(), if_then(_$1 == 1, globvar(yyy_ctr)++));
   cout << " if_then:_$1==1 globvar(yyy_ctr)++: " << yyy_ctr << endl;   
   for_each(vec.begin(), vec.end(), if_then(_$1 == 1, _$1 = 9));

   // check:
   iter = find_if(vec.begin(), vec.end(), _$1 == 9);
   cout << " find_if _$1 == 9 ---> " << (*iter) << endl;
   
   // set all 7 in v10 to 9
   for_each(v10.begin(), v10.end(), if_then(_$1 == 7, _$1 = 9));
   yyy_ctr = 0;
   for_each(v10.begin(), v10.end(), if_then(_$1 == 9, globvar(yyy_ctr)++));
   cout << " if_then:_$1==9 globvar(yyy_ctr)++: " << yyy_ctr << " (10?)" << endl;

   // --- OPEN, TODO: Errors in Shift ????
   //for_each(vec.begin(), vec.end(), cout << _$1 << delay(", ") );
   //for_each(vec.begin(), vec.end(), cout << " -- " << _$1 << delay("."));
   // --> but THIS is OK: !!!!
   // for_each(vec.begin(), vec.end(), cout << delay(", ") << _$1);

   // --------> TODOs
   // Advanced.... ???
   //transform(x.begin(), x.end(), y.begin(), back_inserter(y), construct<pair<int, int> >(_$1, _$2));
   //for_each(vec.begin(), vec.end(), delete_ptr(_$1));
   //for_each(vec.begin(), vec.end(), _$1 = new_ptr<int>(0));

   // OPEN, TODO : bind to member functions...
   //for_each(vecx.begin(), vecx.end(), cout << bind(&XXX::getVal, _$1));
   //for_each(vecx.begin(), vecx.end(), bind(&XXX::getVal, _$1));
   //for_each(vecx.begin(), vecx.end(), bind(&countVec_short, 'X', _$1->getVal()));
   // OPEN TODO --> bind only works with void funcs!!!
   //transform(vecx.begin(), vecx.end(), v10_1.begin(), bind(&XXX::getVal, _$1));
   // TODO --> assign form lambda_expr... SEE comment in AssgnTo
   //for_each(vecx.begin(), vecx.end(), globvar(aaa) = _$1->*(&XXX::value));

   // OPEN TODO ---
   // for_each(v10.begin(), v10.end(), _$1 = bind(&mul2, _$1));


   // -- better syntax ???
   // sort(vecx.begin(), vecx.end(), lambda(*_$1 <= *_$2));
   // for_each(vecx.begin(), vecx.end(), lambda(delete _$1));
   // for_each(vecx.begin(), vecx.end(), lambda { delete _$1 });
   // find_if(vec.begin(), vec.end(), lambda(_$1 <= 2));


   cout << "\n ---- END of TESTS ---\n" << endl;


#if 0 // ==> OPEN TODO: Probably cannot be made work...
   // 4. object value list
   // ---
   vector<XXX> vecobj;
   vector<XXX>::iterator iterobj;
   vecobj.push_back(XXX(11));
   vecobj.push_back(XXX(12));

   // access values: fields directly OR by getters
   for_each(vecobj.begin(), vecobj.end(), cout << delay(" .. ") << _$1->*(&XXX::value));
   cout << " || ";
   
   for_each(vecobj.begin(), vecobj.end(), cout << delay(" .. ") << (_$1->*(&XXX::getVal))); //*5);
   cout << endl;
   
   iterobj = find_if(vecobj.begin(), vecobj.end(), _$1->*(&XXX::value) == 3);
   cout << " find_if _$1->*(XXX:value) == 3 ---> " << (*iterobj).value << endl;
   
   iterobj = find_if(vecobj.begin(), vecobj.end(), _$1->*(&XXX::getVal) == 1);
   cout << " find_if _$1->*(XXX:getVal) == 1 ---> " << (*iterobj).value << endl;
#endif
}
