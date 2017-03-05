#include <pushswap.h>
#include <libft.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <limits.h>


void	fpb(t_stack **stacka, t_stack **stackb)
{

	if (*stacka)
	{
		if (!(*stackb))
		{
			*stackb = *stacka;
			*stacka = (*stacka)->nx;
			(*stackb)->nx = NULL;
		}
		else
		{
			(*stackb)->pv = *stacka;
			*stacka = (*stacka)->nx;
			((*stackb)->pv)->nx = *stackb;
			*stackb = (*stackb)->pv;
		}
	}
}

void	fpa(t_stack **stacka, t_stack **stackb)
{

	if (*stacka)
	{
		if (!(*stackb))
		{
			*stackb = *stacka;
			*stacka = (*stacka)->nx;
			(*stackb)->nx = NULL;
		}
		else
		{
			(*stackb)->pv = *stacka;
			((*stackb)->pv)->nx = *stackb;
			*stackb = (*stackb)->pv;
		}
		*stacka = (*stacka)->nx;
	}
}