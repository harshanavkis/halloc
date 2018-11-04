# halloc

A memory allocation library written in C.

## Usage

```bash
make
./main 2
```

Where 2 is the size in KB of the memory requested from the OS initially. This is the region where allocation and deallocation takes place.

To create a dynamically linkable library, run the following after running the above commands:

```bash
make create-shared
```

## Output

Sample output for the sample main code can be found in the output.txt file.
