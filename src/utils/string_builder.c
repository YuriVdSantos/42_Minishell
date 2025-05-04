#include "minishell.h"
#include <stdlib.h>

void init_string_builder(t_string_builder *sb)
{
    sb->buffer = malloc(1);
    if (!sb->buffer)
        return;
    sb->buffer[0] = '\0';
    sb->size = 0;
    sb->capacity = 1;
}

static void resize_buffer(t_string_builder *sb, size_t min_capacity)
{
    size_t new_capacity = sb->capacity;
    char *new_buffer;
    
    while (new_capacity < min_capacity)
        new_capacity *= 2;
    
    new_buffer = malloc(new_capacity);
    if (!new_buffer)
        return;
    
    ft_memcpy(new_buffer, sb->buffer, sb->size + 1);
    free(sb->buffer);
    sb->buffer = new_buffer;
    sb->capacity = new_capacity;
}

void append_char(t_string_builder *sb, char c)
{
    if (sb->size + 1 >= sb->capacity)
        resize_buffer(sb, sb->size + 2);
    
    if (sb->size + 1 < sb->capacity)
    {
        sb->buffer[sb->size++] = c;
        sb->buffer[sb->size] = '\0';
    }
}

void append_string(t_string_builder *sb, const char *str)
{
    size_t str_len;
    
    if (!str)
        return;
        
    str_len = ft_strlen(str);
    if (sb->size + str_len + 1 > sb->capacity)
        resize_buffer(sb, sb->size + str_len + 1);
    
    ft_memcpy(sb->buffer + sb->size, str, str_len);
    sb->size += str_len;
    sb->buffer[sb->size] = '\0';
}

void append_number(t_string_builder *sb, int num)
{
    char num_str[12]; // Suficiente para int (-2147483648 até 2147483647)
    
    ft_itoa_buf(num, num_str);
    append_string(sb, num_str);
}

void free_string_builder(t_string_builder *sb)
{
    if (sb->buffer)
        free(sb->buffer);
    sb->buffer = NULL;
    sb->size = 0;
    sb->capacity = 0;
}