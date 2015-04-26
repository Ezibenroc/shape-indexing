# Getting started

## Compilation
```bash
mkdir build
cd build
cmake ..
make -j 4 main
```

## Execution
Compare the given image to the given classes of images:
```
./main <number of threads> <image> <class folder 1> ... <class folder n>
```

In particular, the following will compare the image `foo.pgm` to the 71 classes,
using 4 threads (supposing that the working directory is `build`, that `foo.pgm`
is in the working directory and that the classes are in folder `database`).
```bash
./../makeClassesFolders.py database # build a sub-directory for each class
./main 4 foo.pgm database/* # run the program
```
