#include <stdlib.h>

int ft_strlen(const char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_isdigit(int i)
{
	if (i >= '0' && i <= '9')
		return (1);
	return (0);
}

int	ft_isalnum(int i)
{
	if ((i >= 'a' && i <= 'z') || \
	(i >= 'A' && i <= 'Z') || \
	(i >= '0' && i <= '9'))
		return (1);
	return (0);
}

int	ft_dollar_envname_len(char	*to_find)
{
	int	len;
	len = 0;

	if (ft_isdigit(to_find[len]) == 1 || to_find[len] == '?' || to_find[len] == '$')
		return (1);
	while (to_find[len])
	{
		if (ft_isalnum(to_find[len]) == 0 && to_find[len] != '_')
			break ;
		len++;
	}
	return (len);
}


char	*ft_strndup(const char *src, int n)
{
	char	*dest;
	int		srclen;
	int		i;

	srclen = ft_strlen(src);
	dest = (char *)malloc((srclen + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}



char	*get_dollar_envame(char	*after_dollar)
{
	int	len;
	char	*env_name;

	len = ft_dollar_envname_len(after_dollar);
	env_name = ft_strndup(after_dollar, len);
	return (env_name);
}

#include <stdio.h>
int main(int ac, char *av[])
{
	printf("%i\n", ft_dollar_envname_len(av[1]));
	printf("%s\n", get_dollar_envame(av[1]));
}