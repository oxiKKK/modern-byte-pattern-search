# :mag: modern-byte-pattern-search
A very easy byte-pattern search implementation using modern c++ & automatic mask creation.

# How does it work?

## Overall introduction
When you write a program, the program is firstly ran through another program called a `compiler`. `compiler` turns text-based program into an executable, which can be ran by the OS. This is executable is nothing else then just `sequence of bytecodes`.
If a module is loaded into process memory, that module is also a sequece of bytecodes. Knowing this fact, you can search for a specific sequence of bytecodes inside that module to locate a specific address.
This method works everytime, unless there are some changes made to the specific module area we're searching in.

## Code explanation
The class uses `automatic string length deduction` using templates. Having a power of pattern length knowledge is good, because we can utilize it for automatic creation of mask, which is necessary for the search process.

# Well tested
The `main.cpp` file demonstrates the bacic usage of the byte pattern scan. It searches for the `MessageBoxA` function located inside `user32.dll` module.

