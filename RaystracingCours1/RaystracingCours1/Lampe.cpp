#include "pch.h"
#include "Lampe.h"


Lampe::Lampe() : position(), intensite()
{
}



Lampe::Lampe(Vector3 _position, Vector3 _intensite) :
	position(_position), intensite(_intensite)
{

}


Lampe::~Lampe()
{
}
