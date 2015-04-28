# Getting started

## Compilation
```bash
mkdir build
cd build
cmake ..
make -j 4 main
```

## Execution
Syntax:
```
./main [-j <number of threads>] <remainder>
```

Each element of remainder can be either a `pgm` file, or a directory containing `pgm`
files in its sub-directories.

The number of elements determines the mode of the program.

#### Getting information about some class

```
./main [-j <number of threads>] <directory>
```

Will print statistics about the distances between distinct elements of the class
represented by the directory. Does not make sense if there is only one file.


#### Computing the distances between two classes

```
./main [-j <number of threads>] <directory 1> <directory 2>
```

Will print statistics about the distances between elements of the two classes
represented by the directories. In particular, if one directory consists of a single
file, it will print the distances between the image and the whole class.


#### Retrieving a class

```
./main [-j <number of threads>] <directory 1> <directory 2> ... <directory n>
```

Among the n-1 classes represented by the directories 2...n, will print the class
which is the nearest to the class represented by the directory 1. Classical use case:
the directory 1 only contains one image.

#### Example

The following will compare the image `foo.pgm` to the 71 classes, using 4 threads
(supposing that the working directory is `build`, that `foo.pgm` is in the working
directory and that the classes are in folder `database`).
```bash
./../makeClassesFolders.py database # build a sub-directory for each class
./main -j 4 foo.pgm database/* # run the program
```
