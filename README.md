
noplate library
===============

*experimental* generic type-safe container data structures for C


Generic types provided are: vec, span, list, product, sum, and
maybe.  There is also a string type build on top of it, facilities
for safe slicing of arrays, and for storing the byte representation
of types in a buffer. The generic types are based on structures defined
in macros. They take typedef names as arguments and define a generic
container with this element type. For example:

vec(int)* vi; // pointer to a vector of ints


Note that the type argument must be a single identifier, i.e. to
use it with more complicated types, a typedef name must be defined
first.

There is also support for arrays and strings. Arrays are just
regular C arrays but we provide functions such as array_slice
that perform bounds checking. A string is a vector of char
which also has null-termination.


All container types preserve information about the type of
the elements and also have bounds checking, i.e. out-of-bounds
accesses trap at run-time (with UBSan).  (NOTE: EXPERIMENTAL,
this is certainly not safe at this point!)


Interoperability works by providing access to the underlying
elements. For example, vec_access provides access to an
lvalue for the underlying element of a vector and vec2array
provides access to an array representing the complete vector.
See example 9 how where a span is created from a vector and
a pointer to the underlying array is passed to a C function.

Array views constructed by vec2array or span2array are VLAs,
so accesses are bounds-checked when using UBSan. A string
type is defined as vector of char with null termination. The
underlying array can be passed as an argument to C functions,
because the array decays to a pointer.

Before C23, one has to add forward declarations of the types,
e.g. vec_decl(int).


Example 1 (vector of integers)
------------------------------

https://godbolt.org/z/oK5P31bzG

	vec_decl(int);
	vec(int)* v = vec_alloc(int);

	vec_push(int, &v, 1);
	vec_push(int, &v, 3);

	vec_access(int, v, 1)++;

	vec_access(int, v, 10) = 1;	// run-time error!

	free(v);


Example 2 (span)
----------------

https://godbolt.org/z/r3WbhdYfc

    void sum(span(int) v)
    {
        int sum = 0;
        for (int i  = 0; i < span_length(&v); i++)
            sum += span_access(int, &v, i);
        return sum;
    }

	vec(int)* v = vec_alloc(int);

	int s = sum(vec2span(int, v));


Example 3 (strings)
-------------------

https://godbolt.org/z/MMKEdG3or

    string *a = string_init("abc");
    string *b = string_printf("%d", 3);
    string *c = string_concat(a, b);

    printf("%s\n", string_cstr(c));

    assert(string_length(c) + 1 == sizeof(string_cstr(c)));


Example 4 (strview)
-------------------

    string s = ...
    strview v = string_view(s);

	printf("%s\n", string_cstr(&v));


Example 5 (maybe)
-----------------

https://godbolt.org/z/bocqjKb73

    maybe(int) divide(int a, int b)
    {
        return (b != 0) ? maybe_ok(int, a / b) : maybe_fail(int);
    }


Example 6 (product and sum)
------------------------------

https://godbolt.org/z/jMe8dsxcM

    typedef product(int, float) product_name(int, float);
    typedef product(int, int) product_name(int, int);

	auto ifl = product_init(int, float, (3, 0.1));
	auto si = sum_init(product_name(int, float), string_ptr, ifl);

	NESTED(int, add1, (product(int, float) ifl)) { return product_car(ifl) + 1; };
	NESTED(int, slen, (string_ptr x)) { return (int)string_length(x); };

	int si2 = sum_choice(si, add1, slen);

	assert(4 == si2);



Example 7 (vector of strings)
-----------------------------

https://godbolt.org/z/hhT98vM7Y

	typedef string* strptr;
	vec(strptr)* s = vec_alloc(strptr);

	vec_push(strptr, &s, string_init(" Du!"));
	vec_push(strptr, &s, string_init("Hallo"));

	int cmp2(const strptr* a, const strptr* b)
	{
		return strcmp(string_cstr(*a), string_cstr(*b));
	}

	vec_sort(strptr, s, cmp2);

	while (0 < vec_length(s))
		free(vec_pop(strptr, &s));

	free(s);



Example 8 (array slice)
-----------------------

https://godbolt.org/z/YqnKTT6ds

    char str[] = "Hallo";
	auto slice = &array_slice(str, 1, 1 + 3);
	(*slice)[0] = 'A';
	(*slice)[1] = 'L';
	(*slice)[2] = 'L';


Example 9 (span + slice)
------------------------

https://godbolt.org/z/fb61Gr9sf


Example 10 (byte-level loads and stores)
---------------------------------------

https://godbolt.org/z/xeYfn17zG

	char buf[sizeof(int)];
	poke(int, &buf, 33);
    int i = peek(int, &buf);


Example 11 (preorder tree traversal)
------------------------------------

https://godbolt.org/z/rrdE4ce1T

