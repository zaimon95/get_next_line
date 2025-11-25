/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 10:10:13 by sla-gran          #+#    #+#             */
/*   Updated: 2025/11/13 10:10:13 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static char	*update_stash(int fd, char *stash)
{
	char	*buf;
	char	*tmp;
	ssize_t	r;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	r = 1;
	while (r > 0 && (!stash || !ft_strchr(stash, '\n')))
	{
		r = read(fd, buf, BUFFER_SIZE);
		if (r <= 0)
			break ;
		buf[r] = '\0';
		if (stash)
			tmp = ft_strjoin(stash, buf);
		else
			tmp = ft_strdup(buf);
		free(stash);
		if (!tmp)
			return (free(buf), NULL);
		stash = tmp;
	}
	free(buf);
	return (stash);
}

static char	*extract_line(char *stash)
{
	size_t	len;
	size_t	i;
	int		has_nl;
	char	*out;

	if (!stash || !stash[0])
		return (NULL);
	len = 0;
	while (stash[len] && stash[len] != '\n')
		len++;
	has_nl = (stash[len] == '\n');
	out = malloc(len + 1 + has_nl);
	if (!out)
		return (NULL);
	i = 0;
	while (i < len)
	{
		out[i] = stash[i];
		i++;
	}
	if (has_nl)
		out[i++] = '\n';
	out[i] = '\0';
	return (out);
}

static char	*get_rest(char *stash)
{
	char	*rest;
	size_t	i;
	size_t	j;
	size_t	len;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (free(stash), NULL);
	i++;
	len = ft_strlen(stash + i);
	if (len == 0)
		return (free(stash), NULL);
	rest = malloc(len + 1);
	if (!rest)
		return (free(stash), NULL);
	j = 0;
	while (stash[i])
		rest[j++] = stash[i++];
	rest[j] = '\0';
	free(stash);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = update_stash(fd, stash);
	if (!stash || !*stash)
		return (free(stash), stash = NULL, NULL);
	line = extract_line(stash);
	stash = get_rest(stash);
	return (line);
}

/*int	main(void)
{
	int		fd;
	char	*line;

	fd = open("hp.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (EXIT_SUCCESS);
}*/
