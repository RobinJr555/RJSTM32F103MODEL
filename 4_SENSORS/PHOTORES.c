#include "PHOTORES.h"

u16   Get_Brightness_Strength(float Vtemp)
{
	u16 temp;
	temp = (3.20 - Vtemp)/0.032;
	return temp;

}

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
