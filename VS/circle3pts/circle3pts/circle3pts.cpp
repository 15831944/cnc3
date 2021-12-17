#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <conio.h>
#include <stdio.h>

typedef struct { double x, y; } fpoint_t;

double sq(double x) { return x * x; }

double length_fpoint(const fpoint_t* const A, const fpoint_t* const B) {
	double dx = B->x - A->x;
	double dy = B->y - A->y;
	return sqrt(dx * dx + dy * dy);
}

/* Find circle center by 3 points
 * A, B, C - points
 * return: center and diameter (D)
 * D < 0 - points error
*/
fpoint_t circle_3pt_n(const fpoint_t* An, const fpoint_t* Bn, const fpoint_t* Cn, double* const Dn) {
	static fpoint_t res = { 0,0 };
	static fpoint_t AB = { 0,0 }, BC = { 0,0 }, CA = {0, 0};
	static double a1 = 0, b1 = 0, a2 = 0, b2 = 0;

	res.x = res.y = 0;

	AB.x = An->x - Bn->x;
	AB.y = An->y - Bn->y;

	BC.x = Bn->x - Cn->x;
	BC.y = Bn->y - Cn->y;

	CA.x = Cn->x - An->x;
	CA.y = Cn->y - An->y;

	if (BC.x > AB.x && CA.x > AB.x) { // AB is the most vertical line
		const fpoint_t* t = Cn;
		Cn = Bn;
		Bn = An;
		An = t;

		AB.x = An->x - Bn->x;
		AB.y = An->y - Bn->y;

		BC.x = Bn->x - Cn->x;
		BC.y = Bn->y - Cn->y;
	}
	else if (AB.x > BC.x && CA.x > BC.x) { // BC is the most vertial line
		const fpoint_t* t = An;
		An = Bn;
		Bn = Cn;
		Cn = t;

		AB.x = An->x - Bn->x;
		AB.y = An->y - Bn->y;

		BC.x = Bn->x - Cn->x;
		BC.y = Bn->y - Cn->y;
	}

	if (fabs(AB.x) >= 1 && fabs(BC.x) >= 1) { // There are not vertical lines
		a1 = (sq(An->x) - sq(Bn->x) + sq(An->y) - sq(Bn->y)) / (2.0 * AB.x);
		b1 = -AB.y / AB.x;

		a2 = (sq(Bn->x) - sq(Cn->x) + sq(Bn->y) - sq(Cn->y)) / (2.0 * BC.x);
		b2 = -BC.y / BC.x;

		if (fabs(b1 - b2) > 0.001) { // it's not one line
			res.y = (a2 - a1) / (b1 - b2);
			res.x = a1 + b1 * res.y;

			if (Dn) *Dn = (length_fpoint(&res, An) + length_fpoint(&res, Bn) + length_fpoint(&res, Cn)) * (2.0 / 3.0);
			return res;
		}
	}

	if (Dn) *Dn = -1;
	return res;
}

void circle3pts(double x0, double y0, double R, double a, double b, double g, fpoint_t& A, fpoint_t& B, fpoint_t& C) {
	a *= 2 * M_PI / 360;
	b *= 2 * M_PI / 360;
	g *= 2 * M_PI / 360;

	A.x = x0 + R * cos(a);
	A.y = y0 + R * sin(a);

	B.x = x0 + R * cos(b);
	B.y = y0 + R * sin(b);

	C.x = x0 + R * cos(g);
	C.y = y0 + R * sin(g);
}

int main() {
	fpoint_t A = {0, 0}, B = {0, 0}, C = {0, 0}, Center = {0, 0};
	double D = 0;

	circle3pts(3, 2, 10, 0, 135, 270, A, B, C);
	Center = circle_3pt_n(&A, &B, &C, &D);
	printf("Center = (%f, %f), D = %f\n", Center.x, Center.y, D);
	
	// Horizontal
	circle3pts(3, 2, 10, 10, 170, 280, A, B, C);
	Center = circle_3pt_n(&A, &B, &C, &D);
	printf("Center = (%f, %f), D = %f\n", Center.x, Center.y, D);

	// The first two points are vertical
	circle3pts(3, 2, 10, 100, 260, 0, A, B, C);
	Center = circle_3pt_n(&A, &B, &C, &D);
	printf("Center = (%f, %f), D = %f\n", Center.x, Center.y, D);

	std::cout << "Press any key\n";
	_kbhit();
}
