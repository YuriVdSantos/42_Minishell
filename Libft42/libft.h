/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 19:02:00 by jhualves          #+#    #+#             */
/*   Updated: 2025/04/15 18:37:41 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H

# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>

int			ft_isalpha(int c);
int			ft_isascii(int c);
int			ft_isdigit(int c);
int			ft_isprint(int c);
int			ft_isalnum(int c);
size_t		ft_strlen(const char *str);
void		*ft_memset(void *ptr, int value, size_t num);
void		ft_bzero(void *ptr, size_t num);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
size_t		ft_strlcpy(char *dest, const char *src, size_t n);
size_t		ft_strlcat(char *dest, const char *src, size_t n);
int			ft_toupper(int c);
int			ft_tolower(int c);
char		*ft_strchr(const char *str, int c);
char		*ft_strrchr(const char *str, int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
char		*ft_strnstr(const char *s1, const char *s2, size_t len);
char		*ft_strdup(const char *src);
char		*ft_strndup(const char *src, size_t size);
void		*ft_calloc(size_t num, size_t size);
int			ft_atoi(const char *ptr);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
char		*ft_itoa(long n);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *str, char c);
int			ft_putchar(char c);
int			ft_putstr(char *s);
int			ft_putptr_fd(void *ptr);
int			ft_puthex_fd(unsigned int hex, char format);
int			ft_putnbr(int n);
int			ft_putunbr_fd(unsigned int n);
int			ft_printf(const char *format, ...);
int			ft_strcmp(const char *s1, const char *s2);
double		ft_atodbl(char *s);

#endif