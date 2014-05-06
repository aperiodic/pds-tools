# PDS lbl Grammar

The lbl files consist of key-value associations namespaced by an object.
The namespacing means that multiple objects can define the same key.

Types are as follows:
  * strings (leading-alpha values, or quoted ones)
    - spaces need quoting
    - multiline always quoted, trim whitespace after newline
  * numbers, including
    - integers
    - floating point
    - Alternate radix: r#dd..# where r is radix, d is digit
    any number can be tagged with a unit inside of angle brackets (e.g. '<KM>')
  * tuples using '(x,y)' syntax.

Comments with /* */, but do not seem to be multi-line (always start and end on same).

## Psuedo-BNF:

            lbl: <version> <metadata>* <object>* END
        version: PDS_VERSION_ID = <string>\n
       metadata: <association>\n
         object: OBJECT = <string|"name">\n<association>* END_OBJECT = <string|"name">
    association: <key> = <value> | <key> = <tuple>
            key: ^?[A-Z_]+
          tuple: (<value|t>,<value|t>)
          value: <string> | -?<number>
         string: [A-Z][A-Za-z0-9_]* | "[^"]*"
         number: <float> | <integer>
          float: [0-9]+\.[0-9]+
        integer: [0-9]+ | <radix|base>#<digit>+# ;; digits are each up to base, 10 = A, 11 = B, ...
          radix: [0-9]+
          digit: [0-9A-Z]
