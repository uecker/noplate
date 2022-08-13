/* Copyright 2022. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include "mdvla.h"


int main()
{
	md_vla(3, int) x = { (int[24]){ 0 }, { 2, 3, 4, } };

	md_vla_array(x)[0][1][2] = 3;

	int (*p)[2][3][4] = &md_vla_array(x);
}





