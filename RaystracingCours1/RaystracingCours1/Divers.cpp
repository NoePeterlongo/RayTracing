#include "pch.h"
#include "Divers.h"

void CopierValeurs(Intersection iIn, Intersection iOut)
{
	*iOut.point = *iIn.point;
	*iOut.normale = *iIn.normale;
	*iOut.materiau = *iIn.materiau;
	*iOut.t = *iIn.t;
}