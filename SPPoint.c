/*
 * SPPoint.c
 *
 *  Created on: 17 ���� 2016
 *      Author: Noa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPPoint.h"
//
//struct sp_point_t{
//	double* data;
//	int index;
//	int dim;
//};
SPPoint spPointCreate(double* data, int dim, int index){
	int i;
	if(data == NULL || dim <=0 || index <0){
		return NULL;
	}
	//SPPoint newPoint=(SPPoint)malloc(2*sizeof(int)+sizeof(double*)); //malloc(sizeof(sp_point_st))
	SPPoint newPoint=(SPPoint)malloc(sizeof(struct sp_point_t)); //sizeof(newPoint)
	if(newPoint == NULL)    //memory allocation failed
		{
			return NULL;
		}
	double* newData=(double*)malloc(dim*sizeof(double));
	if(newData == NULL)    //memory allocation failed
			{
				free(newPoint);
				return NULL;
			}
	//printf("%s %f", "this is cell a in data:", data[0]);
	memcpy(newData,data,dim*sizeof(double)); //memcpy. try to do sizeof(double)*dim (total size) TODO
//	for (i=0 ; i<dim ; i++){
//		newData[i]=data[i];
//	}
	newPoint->data=newData;
	//printf("%s %f", "this is cell a:", newPoint->data[0]);
	for (i=0 ; i<dim;i++){  //TODO testing
		printf("%s %d %s %f", "this is cell a[",i,"]", newPoint->data[0]);
	}
	newPoint->index=index;
	newPoint->dim=dim; // = (*(newPoint)).dim
	return newPoint;
}

SPPoint spPointCopy(SPPoint source){ /* use Creat function TODO */
	int i;
	//SPPoint pointCopy=(SPPoint)malloc(2*sizeof(int)+sizeof(double*));
	SPPoint pointCopy=(SPPoint)malloc(sizeof(SPPoint));
		if(pointCopy == NULL)    //memory allocation failed
			{
				free(pointCopy);
				return NULL;
			}
	//pointCopy->data=(double*)malloc(source->dim*sizeof(double));
	double* newData=(double*)malloc(source->dim*sizeof(double));
	if(newData == NULL)    //memory allocation failed
			{
				free(pointCopy);
				return NULL;
			}
	//memcpy(pointCopy->data,source->data,sizeof(source->data));
	for (i=0 ; i<source->dim ; i++){
		newData[i]=source->data[i];
		}
	pointCopy->data=newData;
	pointCopy->index=source->index;
	pointCopy->dim=source->dim;
	return pointCopy;
}

void spPointDestroy(SPPoint point){
	free(point->data);
	free(point);

}

int spPointGetDimension(SPPoint point){
	int res=point->dim;
	return res;
}

int spPointGetIndex(SPPoint point){
	return point->index;
}

double spPointGetAxisCoor(SPPoint point, int axis){
	double res=(double)(point->data[axis]);
	return res;
}

double spPointL2SquaredDistance(SPPoint p, SPPoint q){
	double res=0;
	int i;
	for (i=0 ; i<p->dim ; i++){
		double pI=p->data[i];
		double qI=q->data[i];
		res+=(pI-qI)*(pI-qI);
	}
	return res;
}
