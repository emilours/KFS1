// itoa, atoi
#include "../../includes/kfs1.h"

int	ft_atoi(const char *str)
{
	char	*s;
	int		i;
	int		signe;
	int		resultat;

	s = (char *)str;
	i = 0;
	signe = 1;
	resultat = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == 32)
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			signe = signe * (-1);
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		resultat = resultat * 10 + s[i] - '0';
		i++;
	}
	return (resultat * signe);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*cpy_dest;
	const unsigned char	*cpy_src;

	cpy_dest = (unsigned char *)dest;
	cpy_src = (unsigned char *)src;
	while (n > 0)
	{
		*cpy_dest = *cpy_src;
		cpy_src++;
		cpy_dest++;
		n--;
	}
	return (dest);
}

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*str;

	str = s;
	while (n > 0)
	{
		*str = (unsigned char)c;
		str++;
		n--;
	}
	return (s);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
		i++;
	return (i);
}

int ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}