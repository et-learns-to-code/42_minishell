/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etien <etien@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:16:29 by etien             #+#    #+#             */
/*   Updated: 2024/11/05 10:35:37 by etien            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

// Error messages
# define UNCLOSED_QUOTES "Syntax error: Unclosed quotes."

char	*expand_var(char *s);
char	*sub_in_var(char **s, char *expanded_s);
char	*append_str(char **s, char *expanded_s, char initial_quote);

#endif
