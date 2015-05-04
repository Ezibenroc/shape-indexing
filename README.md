# Getting started

## Installing DGtal

These programs depend of [DGtal](http://dgtal.org/), a library providing tools
and algorithms for digital geometry.

You can install this library as follows:

```bash
git clone git://github.com/DGtal-team/DGtal.git
mkdir build && cd build && cmake .. && make
```

## Compilation

```bash
mkdir build
cd build
cmake .. -DDGtal_DIR=/path/to/DGtal/build
make main
```

## Learn

Compute the signature vectors of all the images of the given database and store
them in some file. This program is multithreaded: you can highly reduce the execution
time on a multi-core machine.

Basic usage:
```
./learn --nbthread <number of threads> --input <image database folder> --output <output file>
```

## Classify

Print the probability of the given image to belong to each of the classes, in the
order provided by the CSV file.

Basic usage:
```
./classify --classes <classes CSV file> --descriptors <descriptors file> --image <image file>
```

## Distance

Print the distance between the signature vectors of the two given images.

Basic usage:

```
./distance <image file 1> <image file 2>
```

## Example

```bash
# Compute the file descriptors.info using 4 threads
./learn -j 4 -i database -o descriptors.info
# For each class, print the probability that the image apple-3.pgm belongs to it.
./classify -c classes.csv -d descriptors.info -i database/apple-3.pgm
```
