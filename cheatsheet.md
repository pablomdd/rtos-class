## System
### List directory and modifiers
List with file size in bytes
`ls -l`

List with file size in KiloBytes
`ls -lh`

List with file size in KiloBytes + ordered by most recent
`ls -lht`

List with file size in KiloBytes + reverse ordered by most recent (Reverse by date)
`ls -lhtr`

### Type of file

Install File lib
`sudo apt get file`

See type of file
`file my_file.ext`


## Code
Compile
`gcc my_program.c -o executable_program.xtn`

Execute 
`./executable_program.xtn`

Print contents
`echo $?`

### Step by step compilation
Preprocessing
`gcc -E hola.c -o hola.i`

Compile (Assembly code)
`gcc -S hola.i -o hola.s`

Assemble file (executable, aka Machine code)
`as hola.s -o hola.o`
