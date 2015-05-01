# Getting started

## Compilation
```bash
mkdir build
cd build
cmake ..
make -j 4 main
```

## Learn

Basic usage:
```
./learn --nbthread <number of threads> --input <image database folder> --output <output file>
```

## Classify

Basic usage:
```
./classify --classes <classes CSV file> --descriptors <descriptors file> --image <image file>
```

## Distance

TODO

## Example

```bash
# Compute the file descriptors.info using 4 threads
./learn -j 4 -i database -o descriptors.info
# For each class, print the probability that the image apple-3.pgm belongs to it.
./classify -c classes.csv -d descriptors.info -i database/apple-3.pgm
```
