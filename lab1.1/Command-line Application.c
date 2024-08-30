#include <utility.h>
#include <ansi_c.h>

void CalculateRoots(float a, float b, float c, float* pD, float* pValue1, float* pValue2)
{
	*pD = b*b - 4*a*c;
	if(*pD > 0)
	{
		*pValue1 = (- sqrt(*pD) - b)/(2*a);
		*pValue2 = (sqrt(*pD) - b)/(2*a);
	}
	else
	{
		*pValue1 = - b/(2*a);
		*pValue2 = sqrt(- (*pD))/(2*a);
	}
}

void CalculateRoot(float a, float b, float* pValue)
{
	*pValue = - (b / a);
}

								
int main (int argc, char *argv[])
{
	float a;
	float b;
	float c;
	float pD;
	float pValue1;
	float pValue2;
	
	while(1)
	{
		printf("Input the coefficients of quadric equation: \n");
		scanf("%f%f%f", &a, &b, &c);
		
	
		if(a == 0 && b == 0 && c != 0)
		{
			printf("There is no solution. Press 'enter' to continue and press 'q' to exit.");
		}
		else if(a == 0 && b == 0 && c == 0)
		{
			printf("There are infinitely many solutions. Press 'enter' to continue and press 'q' to exit.");
		}
	    else if(a == 0 && b != 0)
		{
			CalculateRoot(b, c, &pValue1);
			printf("It's a liniar equation. The root is %f", pValue1);
		}
		else
		{
			CalculateRoots(a, b, c, &pD, &pValue1, &pValue2);
			if(pD > 0)
			{
				printf("The roots is %f, %f. Press 'enter' to continue and press 'q' to exit.", pValue1, pValue2);
			}
			else
			{
				printf("The real part of roots is %f, the imaginary part of roots is %f. Press 'enter' to continue and press 'q' to exit.", pValue1, pValue2);
			}
		
		}
		
		int key = GetKey();
		if ((key == 0x0051) || (key == 0x0071))
		{
			return (0);
		}
		
		printf("\n");
	}
	
}

