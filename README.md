# pocket-lambda-lib

Lambdas for C++98 - my very old code, for sentimental purposes only!

**Warning:** At the moment, the code isn't working with a modern compiler!!!

The idea was to be able to write code like:

    find_if(vec.begin(), vec.end(), lambda(_$1 <= 2))
    for_each(vec.begin(), vec.end(), lambda(cout << _$1 << "\n"));

    sort(vec_of_ptrs.begin(), vec_of_ptrs.end(), lambda(*_$1 <= *_$2));
    find_if(vec_of_ptrs.begin(), vec_of_ptrs.end(), lambda(_$1->getValue() != 7))
    for_each(vec_of_ptrs.begin(), vec_of_ptrs.end(), lambda(delete _$1));

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
 - add CMake support
 - add C++98 build
