#include "../SPPoint.h"
#include "unit_test_util.h"
#include <stdbool.h>

bool pointBasicCreateTest() {
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	int dim2;
	int i; //TODO noa.was inside the loop declaration
	SPPoint p = spPointCreate(data, dim, index);
	ASSERT_TRUE(p!=NULL);
	dim2 = p->dim;
	printf("%s %f %s %f %s", "first element is:", p->data[0], "the second is:", p->data[1], "\n");
	ASSERT_TRUE(dim2==dim);
	ASSERT_TRUE(p->index==1);
	for ( i = 0; i < spPointGetDimension(p); i++) {
		printf("%s %d %s %f","create p[",i,"]",spPointGetAxisCoor(p, i));  //TODO noa
		fflush(NULL);
		printf("\n");
		fflush(NULL);
		ASSERT_TRUE(spPointGetAxisCoor(p, i)==1.0);
	}
	spPointDestroy(p);
	return true;
}


//Checks if copy Works
bool pointBasicCopyTest() {
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	int i; //TODO noa.was inside the loop declaration
	SPPoint p = spPointCreate(data, dim, index);
	SPPoint q = spPointCopy(p);
	ASSERT_TRUE(spPointGetIndex(p) == spPointGetIndex(q));
	ASSERT_TRUE(spPointGetDimension(p) == spPointGetDimension(q));
	for ( i = 0; i < spPointGetDimension(p); i++) {
		printf("%s %d %s %f","p[",i,"]",spPointGetAxisCoor(p, i));  //TODO noa
		fflush(NULL);
		printf("\n");
		fflush(NULL);
		printf("%s %d %s %f","q[",i,"]",spPointGetAxisCoor(q, i));
		fflush(NULL);
		ASSERT_TRUE(spPointGetAxisCoor(p, i+1) == spPointGetAxisCoor(q, i+1));
	}
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}
bool pointBasicL2Distance() {
	double data1[2] = { 1.0, 1.0 };
	double data2[2] = { 1.0, 0.0 };
	int dim1 = 2;
	int dim2 = 2;
	int index1 = 1;
	int index2 = 1;
	SPPoint p = spPointCreate((double *)data1, dim1, index1);
	SPPoint q = spPointCreate((double *)data2, dim2, index2);
	ASSERT_TRUE(spPointL2SquaredDistance(p,p) == 0.0);
	ASSERT_TRUE(spPointL2SquaredDistance(q,q) == 0.0);
	ASSERT_FALSE(spPointL2SquaredDistance(p,q) == 0.0);
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}
//int main() {
//	RUN_TEST(pointBasicCreateTest);
//	RUN_TEST(pointBasicCopyTest);
//	RUN_TEST(pointBasicL2Distance);
//	printf("Done"); //noa TODO
//	fflush(NULL);
//	return 0;
//}
