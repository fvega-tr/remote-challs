#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct	s_struct
{
				int column;
				int row;
				int len_row;
				int len_column;
				char **board;
}				t_struct;

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	char	*source;

	i = 0;
	j = 0;
	if (dst == NULL)
		return (dstsize);
	source = (char *)src;
	while (source[j] != '\0')
		j++;
	if (dstsize > 0)
	{
		while ((i < dstsize - 1) && (source[i] != '\0'))
		{
			dst[i] = source[i];
			i++;
		}
		dst[i] = ('\0');
	}
	return (j);
}

static int	ft_wordscounter(const char *s, char c, int *pointer)
{
	int i;
	int size;

	if (!s)
		return (-1);
	i = 0;
	size = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			size++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		else
			i++;
	}
	*pointer = i - 1;
	return (size);
}

static int	ft_malloc(int size, int i, int j, char **pointer)
{
	((pointer[size] = (char *)malloc(sizeof(char) * (j - i + 1))));
	if (pointer[size] == NULL)
	{
		while (pointer[size] != '\0')
			free(pointer[size++]);
		free(pointer);
		return (0);
	}
	return (1);
}

char		**ft_split(char const *s, char c)
{
	int		i;
	int		size;
	int		j;
	char	**pointer;

	i = 0;
	size = ft_wordscounter(s, c, &i);
	if (!(pointer = (char **)malloc(sizeof(char *) * (size + 1))) || size == -1)
		return (0);
	pointer[size] = NULL;
	while (i >= 0)
	{
		if (s[i] != c)
		{
			j = i;
			while (i >= 0 && s[i] != c)
				i--;
			size--;
			if (ft_malloc(size, i, j, pointer) == 0)
				return (0);
			ft_strlcpy(pointer[size], &((char *)s)[i + 1], j - i + 1);
		}
		i--;
	}
	return (pointer);
}

int	ft_check_invalid(char *str)
{
	int i;
	int len;
	int temp;
	int column;
	int row;
	int king;
	
	i = 0;
	temp = 0;
	column = 0;
	row = 0;
	king = 0;
	len = strlen(str);
	
	if (len < 1 || len > 89)
		return (1);
	
	while (str[i])
	{
		if (str[i] == '\n' && column == 0)
		{
			column = temp;
			row++;
			temp = -1;
		}
		else if (str[i] == '\n' && column != temp)
			return (1);
		else if (str[i] == '\n')
		{
			temp = -1;
			row++;
		}
		if (!(strchr(".PBRQK\n", str[i])))
			return (1);
		if (str[i] == 'K')
			king++;
		temp++;
		i++;		
	}
	row++;
	if (str[i] == '\0' && column != temp)
		return (1);
	return row  == column && king == 1? 0 : 1;
}

void	ft_check_king(t_struct *t)
{
	int i;
	int j;
	
	i = 0;
	j = 0;
	while (t->board[i])
	{
		j = 0;
		while (t->board[i][j])
		{
			if (t->board[i][j] == 'K')
			{
				t->row = i;
				t->column = j;
			}
			j++;	
		}
		i++;
	}
	t->len_row = i - 1;
	t->len_column = j - 1;
}

int	ft_check_line(t_struct *t)
{
	int i;
	
	i = 0;
	while (t->len_row >= i)
	{
		if (t->board[t->row][i] == 'Q' || t->board[t->row][i] == 'R')
			return (1);
		if (t->board[t->row][i] == 'P' || t->board[t->row][i] == 'B')
			break;
		i++;
	}
	i = 0;
	while (t->len_column >= i)
	{
		if (t->board[i][t->column] == 'Q' || t->board[i][t->column] == 'R')
			return (1);
		if (t->board[i][t->column] == 'P' || t->board[i][t->column] == 'B')
			break;
		i++;
	}
	return (0);
}

int	ft_check_diagonal(t_struct *t)
{
	int i;

	i = 0;
	while ((t->row + i) <= t->len_row && (t->column + i) <= t->len_column)
	{
		if (strchr("QB", t->board[t->row + i][t->column + i]))
			return (1);
		else if (strchr("PR", t->board[t->row + i][t->column + i]))
			break;
		i++;
	}
	i = 0;
	while ((t->row - i) >= 0 && (t->column - i) >= 0)
	{
		if (strchr("QB", t->board[t->row - i][t->column - i]))
			return (1);
		else if (strchr("PR", t->board[t->row - i][t->column - i]))
			break;
		i++;
	}
	i = 0;
	while ((t->row - i) >= 0 && (t->column + i) <= t->len_column)
	{
		if (strchr("QB", t->board[t->row - i][t->column + i]))
			return (1);
		else if (strchr("PR", t->board[t->row - i][t->column + i]))
			break;
		i++;
	}
	i = 0;
	while ((t->row + i) <= t->len_row && (t->column - i) >= 0)
	{
		if (strchr("QB", t->board[t->row + i][t->column - i]))
			return (1);
		else if (strchr("PR", t->board[t->row + i][t->column - i]))
			break;
		i++;
	}
	return (0);
}

int	ft_check_pawn(t_struct *t)
{
	if (t->row < t->len_row)
	{
		if (t->column < t->len_column)
			if (t->board[t->row + 1][t->column + 1] == 'P')
				return (1);
		if (t->column > 0)
			if (t->board[t->row + 1][t->column - 1] == 'P')
				return (1);
	}
	return (0);
}

int ft_check_mate(char *str)
{
	t_struct t;
	int i;
	int j;
	
	i = 0;	
	if (ft_check_invalid(str))
		return (1);			
	if (!((t.board = ft_split(str, '\n'))))
		return (1);
	ft_check_king(&t);
	if (ft_check_line(&t))
		return (0);
	if (ft_check_diagonal(&t))
		return (0);
	if (ft_check_pawn(&t))
		return (0);
	return (1);
}

