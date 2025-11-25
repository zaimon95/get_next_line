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

int	find_newline(char *stash)
{
	size_t	i;

	i = 0;
	if (!stash)
		return (0);
	while (stash[i])
	{
		if (stash[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*extract_line(char *stash)
{
	char	*buff;
	size_t	len;
	size_t	i;
	int		has_newline;

	if (!stash || stash[0] == '\0')
		return (NULL);
	len = 0;
	while (stash[len] && stash[len] != '\n')
		len++;
	has_newline = (stash[len] == '\n');
	buff = malloc(len + 1 + has_newline);
	if (!buff)
		return (NULL);
	i = 0;
	while (i < len)
	{
		buff[i] = stash[i];
		i++;
	}
	if (has_newline)
		buff[i++] = '\n';
	buff[i] = '\0';
	return (buff);
}

char	*get_rest(char *stash)
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

static char	*update_stash(char *stash, int fd)
{
	char	buf[BUFFER_SIZE + 1];
	char	*tmp;
	ssize_t	r;

	while (!stash || !ft_strchr(stash, '\n'))
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
		stash = tmp;
	}
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = update_stash(stash, fd);
	if (!stash || !*stash)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = extract_line(stash);
	stash = get_rest(stash);
	return (line);
}
