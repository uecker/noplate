
noplate library
===============

*experimental* generic type-safe container data structures for C


Generic types provided are: vector, list, product, sum, and
maybe. The generic types are based on structures defined in
macros. They take typedef names as arguments and define a
generic container with this element type:

vec(int)* vi; // pointer to a vector of ints


There is also support for arrays and strings. Arrays are just
regular C arrays but we provide functions such as array_slice
that perform bounds checking. A string is a vector of char
with null-termination.


All container types preserve information about the type of
the elements and also have bounds checking, i.e. out-of-bounds
accesses trap at run-time (with UBSan).


Interoperability works by providing access to the underlying
elements. For example, vec_access provides access to an
lvalue for the underlying element of a vector and vec_array
provides access to an array representing the complete vector.
This array is a VLA, so accesses are also checked when using
UBSan. A string type is defined as vector of char with
null termination. The underlying array can be passed as
an argument to C functions, because the array decays to
a pointer.



(the need to declare the types upfront will go away with C23)


Example 1 (vector of integers)
------------------------------

https://godbolt.org/z/7xPY6Wx1T

	vec_decl(int);
	vec(int)* v = vec_alloc(int);

	vec_push(int, &v, 1);
	vec_push(int, &v, 3);

	vec_access(int, v, 1)++;

	vec_access(int, v, 10) = 1;	// run-time error!

	free(v);


Example 2 (view)
----------------

https://godbolt.org/z/Woje9s6PY

    void sum(view(int) v)
    {
        int sum = 0;
        for (int i  = 0; i < vec_length(&v); i++)
            sum += vec_array(int, &v)[i];
        return sum;
    }

	vec(int)* v = vec_alloc(int);

	int s = sum(vec_view(int, v));


Example 3 (strings)
-------------------

https://godbolt.org/z/KE4o17fnW

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

https://godbolt.org/z/zc3bd1va5

    maybe(int) divide(int a, int b)
    {
        return (b != 0 && b != -1) ? maybe_ok(int, a / b) : maybe_fail(int);
    }


Example 6 (product and sum)
------------------------------

https://godbolt.org/z/zqGPvGb1a

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

https://godbolt.org/z/Pbfb81v7z

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

https://godbolt.org/z/noKGz6nWj

    char str[] = "Hallo";
	auto slice = &array_slice(str, 1, 1 + 3);
	(*slice)[0] = 'A';
	(*slice)[1] = 'L';
	(*slice)[2] = 'L';



Example 9 (byte-level loads and stores)
---------------------------------------

https://godbolt.org/z/xeYfn17zG

	char buf[sizeof(int)];
	poke(int, &buf, 33);
    int i = peek(int, &buf);




