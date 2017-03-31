#include <pushswap.h>
#include <libft.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <limits.h>

void	rotcor(t_out *ret, t_stack **sa, t_stack **sb, int descending)
{
	int tmp;

	tmp = 0;
	if (descending)
	{
		tmp = getlargest(*sb);
		if (wheredestj(*sb, tmp) >= 0)
			while ((*sb)->v != tmp)
				updateretstack(ret, RB, sa, sb);
		else
			while ((*sb)->v != tmp)
				updateretstack(ret, RRB, sa, sb);
	}
	else
	{
		tmp = getsmallest(*sa);
		if (wheredestj(*sa, tmp) >= 0)
			while ((*sa)->v != tmp)
				updateretstack(ret, RA, sa, sb);
		else
			while ((*sa)->v != tmp)
				updateretstack(ret, RRA, sa, sb);
	}
}

void	astructmoves(t_stack *sa)
{
	int i;
	t_stack *tmpa2;

	i = 0;
	tmpa2 = sa;
	while (sa)
	{
		sa->maf = i;
		sa = sa->nx;
		i++;
	}
	tmpa2->mar = 0;
	tmpa2 = tmpa2->nx;
	i--;
	while (tmpa2)
	{
		tmpa2->mar = i;
		tmpa2 = tmpa2->nx;
		i--;
	}
}

static void	movedirhelp(t_stack *sa, int *ar)
{

	if (ar[0] <= ar[1] && ar[0] <= ar[2] && ar[0] <= ar[3])
	{
		sa->moves = ar[0];
		sa->dir = 1;
	}
	else if (ar[1] <= ar[0] && ar[1] <= ar[2] && ar[1] <= ar[3])
	{
		sa->moves = ar[1];
		sa->dir = 2;
	}
	else if (ar[2] <= ar[0] && ar[2] <= ar[1] && ar[2] <= ar[3])
	{
		sa->moves = ar[2];
		sa->dir = 3;
	}
	else if (ar[3] <= ar[1] && ar[3] <= ar[0] && ar[3] <= ar[2])
	{
		sa->moves = ar[3];
		sa->dir = 4;
	}
}

void	movedir(t_stack *sa)
{
	int *ar;

	ar = (int *)malloc(sizeof(int) * 4);
	while (sa)
	{
		ar[0] = ((sa->maf > sa->mbf) ? sa->maf : sa->mbf);
		ar[1] = ((sa->mar > sa->mbr) ? sa->mar : sa->mbr);
		ar[2] = sa->maf + sa->mbr;
		ar[3] = sa->mar + sa->mbf;
		movedirhelp(sa, ar);
		sa = sa->nx;
	}
	free(ar);
}

void	prepoprot(t_stack *target, t_stack **sa, t_stack **sb, t_out *ret)
{
	int moves;
	int a;
	int b;

	moves = target->moves;
	while (moves)
	{
		if (target->dir == 1)
		{
			a = target->maf;
			b = target->mbf;
			if (b > 0 & a > 0)
				updateretstack(ret, RR, sa, sb);
			else if (b > 0)
				updateretstack(ret, RB, sa, sb);
			else if (a > 0)
				updateretstack(ret, RA, sa, sb);
			target->maf -= 1;
			target->mbf -= 1;
		}
		else if (target->dir == 2)
		{
			a = target->mar;
			b = target->mbr;
			if (b > 0 & a > 0)
				updateretstack(ret, RRR, sa, sb);
			else if (b > 0)
				updateretstack(ret, RRB, sa, sb);
			else if (a > 0)
				updateretstack(ret, RRA, sa, sb);
			target->mar -= 1;
			target->mbr -= 1;
		}
		else if (target->dir == 3)
		{
			a = target->maf;
			b = target->mbr;
			if (b > 0 & a > 0)
			{
				updateretstack(ret, RA, sa, sb);
				updateretstack(ret, RRB, sa, sb);
			}
			else if (b > 0)
				updateretstack(ret, RRB, sa, sb);
			else if (a > 0)
				updateretstack(ret, RA, sa, sb);
			target->maf -= 1;
			target->mbr -= 1;
		}
		else if (target->dir == 4)
		{
			a = target->mar;
			b = target->mbf;
			if (b > 0 & a > 0)
			{
				updateretstack(ret, RRA, sa, sb);
				updateretstack(ret, RB, sa, sb);
			}
			else if (b > 0)
				updateretstack(ret, RB, sa, sb);
			else if (a > 0)
				updateretstack(ret, RRA, sa, sb);
			target->mar -= 1;
			target->mbf -= 1;
		}
		if (ordered(*sa))
			return ;
		moves--;
	}
}

void	insertbest(t_stack **sa, t_stack **sb, t_out *ret)
{
	t_stack *tmp;
	t_stack *target;
	int moves;

	tmp = *sa;
	target = tmp;
	moves = tmp->moves;
	while (tmp)
	{
		if (tmp->moves < moves)
		{
			moves = tmp->moves;
			target = tmp;
		}
		tmp = tmp->nx;
	}
	prepoprot(target, sa, sb, ret);
	updateretstack(ret, PB, sa, sb);
}

void	mergetwo(t_stack **sa, t_stack **sb, t_out *ret)
{
	while (*sb)
	{
		if ((*sb)->v > getlargest(*sa))
		{
			rotcor(ret, sa, sb, 0);
			updateretstack(ret, PA, sa, sb);
		}
		else if ((*sb)->v < getsmallest(*sa))
		{
			rotcor(ret, sa, sb, 0);
			updateretstack(ret, PA, sa, sb);
		}
		else if ((*sb)->v > (*sa)->v )
			updateretstack(ret, RA, sa, sb);
		else if ((*sb)->v < (*sa)->v && (*sb)->v > (getend(sa))->v)
			updateretstack(ret, PA, sa, sb);
		else
			updateretstack(ret, RRA, sa, sb);
	}
}

int	threesort(t_stack **sa, t_stack **sb, t_out *ret)
{
	if (stacklen(*sa) != 3)
		return (0);
	if ((*sa)->v == getlargest(*sa) && ((*sa)->nx)->v < (getend(sa))->v)
		return (1);
	else if ((*sa)->v > ((*sa)->nx)->v)
		updateretstack(ret, SA, sa, sb);
	else if ((*sa)->v == getsmallest(*sa) && ((*sa)->nx)->v == getlargest(*sa))
		updateretstack(ret, SA, sa, sb);
	return (1);
}

int numsort(t_stack **sa, t_stack **sb, t_out *ret, int size)
{
	int i;
	int num;

	i = 0;
	num = size;
	if (stacklen(*sa) != num)
		return (0);
	while (i < (num - 3))
	{
		rotcor(ret, sa, sb, 0);
		updateretstack(ret, PB, sa, sb);
		i++;
	}
	threesort(sa, sb, ret);
	i = 0;
	while (i < (num - 3))
	{
		rotcor(ret, sa, sb, 0);
		updateretstack(ret, PA, sa, sb);
		i++;
	}
	return (1);
}

void	fortysort(t_stack **sa, t_stack **sb, t_out *ret)
{
	int i;
	int len;

	if (aassigngroups(sa) == 1)
		return ;
	i = 0;
	len = 3;
	if (stacklen(*sa) > 101)
		len = 7;
	while (*sa)
	{
		if (almostordered(*sa) || numsort(sa, sb, ret, len))
			{
				rotcor(ret, sa, sb, 0);
				mergetwo(sa, sb, ret);
				rotcor(ret, sa, sb, 0);
				break;
			}
		astructmoves(*sa);
		bstructmoves(*sa, *sb);
		movedir(*sa);
		insertbest(sa, sb, ret);
	}
}
