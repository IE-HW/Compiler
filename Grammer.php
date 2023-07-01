<program> ::= <statement-list>

<statement-list> ::= <statement>
            | <statement-list> <statement>

<statement> ::= <variable-definition>
        | <assignment>

<variable-definition> ::= "type" <type-specifier> <identifier-list> ";"

<type-specifier> ::= "int"

<identifier-list> ::= <identifier>
            | <identifier-list> "," <identifier>

<assignment> ::= <identifier> "=" <expression> ";"

<expression> ::= <term>
        | <expression> "+" <term>
        | <expression> "-" <term>

<term> ::= <factor>
        | <term> "*" <factor>
        | <term> "/" <factor>

<factor> ::= <identifier>
    | <number>
    | "(" <expression> ")"

<identifier> ::= <letter> <identifier>*
<number> ::= <digit> <number>*
<letter> ::= [a-zA-Z]
<digit> ::= [0-9]
