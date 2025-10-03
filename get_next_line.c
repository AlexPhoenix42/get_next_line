/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 20:54:37 by alephoen          #+#    #+#             */
/*   Updated: 2025/10/03 20:05:55 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 32
#endif

char    *ft_strchr(char *str, char c);
char	*ft_strjoin(char **ss1, char *s2, size_t len1, size_t len2);
size_t	ft_strlen2(char *str);
size_t	ft_strlen(char *str);
char	*ft_shift_buf(char *buf, char *ptr);
char	*ft_strndup(char *src, size_t n);
void	ft_bzero(char *buf, size_t len);

char    *get_next_line(int fd)
{
    static char     buf[BUFFER_SIZE+1] = {0};
	char			*line = NULL;
    ssize_t  		byt_r = 0;

    if (fd < 0 || BUFFER_SIZE <= 0 
        || read(fd, buf, 0) == -1)
        return (NULL);
    char *tmp  = ft_strchr(buf, '\n');
	if (tmp)
	{
		line = ft_strjoin(&line, buf, ft_strlen(line), ft_strlen2(buf));
		ft_shift_buf(buf, tmp);
		return (line);
	}
	if (buf[0])
		line = ft_strjoin(&line, buf, ft_strlen(line), ft_strlen(buf));
    while (!tmp)
    {
    	byt_r = read(fd, buf, BUFFER_SIZE);
		if (byt_r == 0)
		{
			free(tmp);
			tmp = NULL;
			free(line);
			return (NULL);
		}
		line = ft_strjoin(&line, buf, ft_strlen(line), ft_strlen2(buf));
    	tmp  = ft_strchr(buf, '\n');
    }
	if (tmp)
		ft_shift_buf(buf, tmp);
	return (line);
}

void	ft_bzero(char *buf, size_t len)
{
	size_t	i;

	i = -1;
	while (++i < len)
		buf[i] = '\0';
}

char	*ft_shift_buf(char *buf, char *ptr)
{
	char	*tmp = ft_strndup(ptr + 1, &(buf[BUFFER_SIZE]) - ptr);
	size_t	i;

	ft_bzero(buf, BUFFER_SIZE);
	i = -1;
	while (tmp[++i])
		buf[i] = tmp[i];
	buf[i] = '\0';
	free(tmp);
	return (buf);
}

char	*ft_strndup(char *src, size_t n)
{
	char	*str;
	size_t	i;

	if (!src)
		return (NULL);
	str = (char *) malloc(sizeof(char) * (n + 1));
	i = -1;
	while (++i < n && src[i])	
		str[i] = src[i];
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin(char **ss1, char *s2, size_t len1, size_t len2)
{
	char	*str;
	char	*s1;
	size_t	i;
	size_t	j;

	if (!ss1 && !s2)
		return (NULL);
	s1 = *ss1;
	str = (char *) malloc(sizeof(char) * (len1 + len2 + 1));
	i = -1;
	if (s1)
		while (++i < len1)	
			str[i] = s1[i];
	i = len1 - 1;
	j = -1;
	if (s2)	
		while (++j < len2)
			str[++i] = s2[j];
	str[len1 + len2] = '\0';
	free(*ss1);
	*ss1 = str;
	return (str);	
}

size_t	ft_strlen(char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = -1;
	while (str[++len])
		;
	return (len);
}

size_t	ft_strlen2(char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = -1;
	while (str[++len] && str[len] != '\n')
		;
	return (len + 1);
}

char    *ft_strchr(char *str, char c)
{
    size_t  i;

    if (!str)
        return (NULL);
    i = -1;
    while (str[++i])
        if (str[i] == c)
            return (&str[i]);
    return (NULL);
}

int main(int ac, char **av)
{
	int		fd;
	char	*lin;

	if (ac != 2)
		return (0);
	fd = open(av[1], O_RDONLY);
	lin = get_next_line(fd);
	while (lin)
	{
		printf("%s", lin);
		free(lin);
		lin = get_next_line(fd);
	}
	close(fd);
	// write(1, "\n", 1);
	return (0);
}
