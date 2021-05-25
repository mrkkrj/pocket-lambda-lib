# pocket-lambda-lib
[comment]: # " ![lambda pic](haskell-logo.png) "
<img src="haskell-logo.png" alt="lambda pic" width="80"/><br/>Lambdas for C++98 - my very old code, for sentimental purposes only!

**Warning:** At the moment, the code isn't working with a modern compiler!!!

The idea was to be able to write code like:

    find_if(vec.begin(), vec.end(), lambda(_$1 <= 2))
    for_each(vec.begin(), vec.end(), lambda(cout << _$1 << "\n"));

    sort(vec_of_ptrs.begin(), vec_of_ptrs.end(), lambda(*_$1 <= *_$2));
    find_if(vec_of_ptrs.begin(), vec_of_ptrs.end(), lambda(_$1->getValue() != 7))
    for_each(vec_of_ptrs.begin(), vec_of_ptrs.end(), lambda(delete _$1));

What finally was implemented is:

    find_if(vec.begin(), vec.end(), _$1 <= 2);
    for_each(vec.begin(), vec.end(), cout << _$1 << delay("\n")); 

    sort(vec_of_ptrs.begin(), vec_of_ptrs.end(), *_$1 <= *_$2);
    find_if(vec_of_ptrs.begin(), vec_of_ptrs.end(), _$1->*(&TestObj::getVal) != 7)

and, additionally:

    // printing for members 
    for_each(vec_of_ptrs.begin(), vec_of_ptrs.end(), cout << delay(" value=") << (_$1->*(&TestObj::getVal)) << delay(","));

    // access to globals
    for_each(vecx.begin(), vecx.end(), globvar(global_counter) += 5);

    // currying
    for_each(vec.begin(), vec.end(), bind(print_sinus, _$1 * 2));

    // control structure(s)
    for_each(vec.begin(), vec.end(), if_then(_$1 == 1, globvar(global_counter)++));
    for_each(vec.begin(), vec.end(), if_then(_$1 == 7, _$1 = 9));

The *lambda()* expression wrapper wasn't done (no time) and I just forgot (!!) about *delete* support.

# Explantions

I wrote a couple of blogposts explaining ideas behind this implementation:

 - http://ib-krajewski.blogspot.com/2007/12/c-pocket-lambda-library.html
 - http://ib-krajewski.blogspot.com/2008/01/c-pocket-lambda-library-part-2.html
 - http://ib-krajewski.blogspot.com/2008/04/pocket-c-lambda-library-part-iia.html
 - http://ib-krajewski.blogspot.com/2008/06/c-pocket-lambda-library-part-iii.html
 - http://ib-krajewski.blogspot.com/2008/07/c-pocket-lambda-library-last.html

# TODOs

 - remove asserts in C++17 Debug build
 - remove crashes in C++17 Release build
 - add lambda() wrapper for expressions
 - impl. support for deleting over pointers!
 - add CMake support
 - add C++98 build
