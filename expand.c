/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: souita <souita@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 21:50:49 by aabdenou          #+#    #+#             */
/*   Updated: 2024/07/08 02:12:36 by souita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../minishell.h"

char *get_env_value(t_env *env, char *key) {
    while (env) {
        if (ft_strcmp(key, env->key) == 0)
            return env->value;
        env = env->next;
    }
    return ("");
}

// Extract the key starting with '$' from the string
char *to_expand(char *str) {
    int i = 1;
    int start = i;
    while (str[i] && ft_isalpha(str[i]))
        i++;
    int end = i;
    return (ft_substr(str, start, end - start));
}

// Function to expand variables in lexer tokens
void expand(t_lexer *lexer, t_env *env) 
{
    while (lexer) 
    {   
        // if $a="ls -al'" => should [ls] [-al']
        // echo $=HOME ==> $HOME
        int i = 0;
        char *str_to_expand = NULL;
        if (lexer->tokens == WORD && lexer->str[i] != '\'') 
        {
            while (lexer->str[i]) 
            {
                if(ft_strcmp(lexer->str,"$") == 0)// $"USER" ==> USER
                {
                    if(!lexer->next)// echo $
                    {
                        printf("hi1\n");
                        str_to_expand = "$";
                        break;
                    }
                    if (lexer->next->str[i] == '"' || lexer->str[i+1] == '\'') //echo $ "a" != echo a
                        lexer->str = "";
                    lexer = lexer->next;
                    // printf("hi%s\n",lexer->str);
                }
                else if(lexer->str[i] == '$' && lexer->str[i+1] == '$') //echo $$
                {
                        printf("hi2\n");
                    str_to_expand = ft_strjoin(str_to_expand, "$$");
                    i += 2;
                }
                else if(lexer->str[i] == '$' && (lexer->str[i+1] == '"' || lexer->str[i+1] == '\'')) //"$'USER'" ==> $'USER' || "$"USER"" ==> $"USER"
                {
                        printf("hi3\n");
                    str_to_expand = ft_strjoin(str_to_expand, "$");
                    i += 1;
                }
                else if(lexer->str[i] == '$' && ft_isdigit(lexer->str[i+1])) // $9HOME => HOME || $99HOME => 9HOME skip ony the first number
                {
                        printf("hi4\n");
                    i+=2;//skip $ and num
                }
                else if (lexer->str[i] == '$' && (!ft_isalpha(lexer->str[i+1]) && lexer->str[i+1]))//echo $=HOME ==> $HOME
                {
                    printf("hi5\n");
                    str_to_expand = ft_strjoin(str_to_expand,lexer->str);
                    // lexer = lexer->next;
                    break;
                }
                    
                else if (lexer->str[i] == '$' && (ft_isalpha(lexer->str[i+1]) && lexer->str[i+1])) 
                {          
                        printf("hi7\n"); 
                    char *key = to_expand(&lexer->str[i]);
                    char *value = get_env_value(env, key);
                    i += ft_strlen(key) + 1;
                    if (value)
                        str_to_expand = ft_strjoin(str_to_expand, value);
                    free(key);
                }
                 else // mzl mafahmha 
                {
                        printf("hi8\n");
                    char temp[2] = { lexer->str[i], '\0' };
                    str_to_expand = ft_strjoin(str_to_expand, temp);
                    i++;
                }
            }
            lexer->str = str_to_expand;
            // free(to_expand);
        }
        lexer = lexer->next;
    }
}
